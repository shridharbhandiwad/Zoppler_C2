#include "config/DatabaseManager.h"
#include "utils/Logger.h"
#include <QSqlError>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

namespace CounterUAS {

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::initialize(const QString& path) {
    m_dbPath = path;
    
    // Ensure directory exists
    QFileInfo fi(path);
    QDir dir = fi.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    
    if (!m_db.open()) {
        Logger::instance().error("DatabaseManager", "Failed to open database: " + m_db.lastError().text());
        emit databaseError(m_db.lastError().text());
        return false;
    }
    
    if (!createTables()) {
        return false;
    }
    
    Logger::instance().info("DatabaseManager", "Database initialized: " + path);
    return true;
}

void DatabaseManager::close() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::isOpen() const {
    return m_db.isOpen();
}

bool DatabaseManager::createTables() {
    QSqlQuery query;
    
    // Tracks table
    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS tracks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            track_id TEXT NOT NULL,
            timestamp INTEGER NOT NULL,
            latitude REAL,
            longitude REAL,
            altitude REAL,
            velocity_n REAL,
            velocity_e REAL,
            velocity_d REAL,
            classification INTEGER,
            threat_level INTEGER,
            UNIQUE(track_id, timestamp)
        )
    )")) {
        Logger::instance().error("DatabaseManager", "Failed to create tracks table: " + query.lastError().text());
        return false;
    }
    
    // Engagements table
    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS engagements (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            engagement_id TEXT UNIQUE NOT NULL,
            track_id TEXT,
            effector_id TEXT,
            operator_id TEXT,
            start_time INTEGER,
            completion_time INTEGER,
            state INTEGER,
            bda_result INTEGER,
            notes TEXT
        )
    )")) {
        Logger::instance().error("DatabaseManager", "Failed to create engagements table: " + query.lastError().text());
        return false;
    }
    
    // Operator actions table
    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS operator_actions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp INTEGER NOT NULL,
            operator_id TEXT,
            action TEXT,
            details TEXT
        )
    )")) {
        Logger::instance().error("DatabaseManager", "Failed to create operator_actions table: " + query.lastError().text());
        return false;
    }
    
    // Video clips table
    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS video_clips (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            clip_id TEXT UNIQUE NOT NULL,
            path TEXT,
            start_time INTEGER,
            duration INTEGER,
            camera_id TEXT,
            metadata TEXT
        )
    )")) {
        Logger::instance().error("DatabaseManager", "Failed to create video_clips table: " + query.lastError().text());
        return false;
    }
    
    // Create indices
    query.exec("CREATE INDEX IF NOT EXISTS idx_tracks_timestamp ON tracks(timestamp)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_engagements_time ON engagements(start_time)");
    
    return true;
}

void DatabaseManager::saveTrack(const Track& track) {
    saveTrackHistory(track.trackId(), track.position(), QDateTime::currentMSecsSinceEpoch());
}

void DatabaseManager::saveTrackHistory(const QString& trackId, const GeoPosition& pos, qint64 timestamp) {
    if (!isOpen()) return;
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO tracks (track_id, timestamp, latitude, longitude, altitude)
        VALUES (?, ?, ?, ?, ?)
    )");
    query.addBindValue(trackId);
    query.addBindValue(timestamp);
    query.addBindValue(pos.latitude);
    query.addBindValue(pos.longitude);
    query.addBindValue(pos.altitude);
    
    if (!query.exec()) {
        Logger::instance().warning("DatabaseManager", "Failed to save track: " + query.lastError().text());
    }
}

void DatabaseManager::saveEngagement(const EngagementRecord& record) {
    if (!isOpen()) return;
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO engagements 
        (engagement_id, track_id, effector_id, operator_id, start_time, completion_time, state, bda_result, notes)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");
    query.addBindValue(record.engagementId);
    query.addBindValue(record.trackId);
    query.addBindValue(record.effectorId);
    query.addBindValue(record.operatorId);
    query.addBindValue(record.startTime.toMSecsSinceEpoch());
    query.addBindValue(record.completionTime.toMSecsSinceEpoch());
    query.addBindValue(static_cast<int>(record.state));
    query.addBindValue(static_cast<int>(record.bdaResult));
    query.addBindValue(record.notes);
    
    if (!query.exec()) {
        Logger::instance().warning("DatabaseManager", "Failed to save engagement: " + query.lastError().text());
    }
}

void DatabaseManager::logOperatorAction(const QString& operatorId, const QString& action, const QVariantMap& details) {
    if (!isOpen()) return;
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO operator_actions (timestamp, operator_id, action, details)
        VALUES (?, ?, ?, ?)
    )");
    query.addBindValue(QDateTime::currentMSecsSinceEpoch());
    query.addBindValue(operatorId);
    query.addBindValue(action);
    query.addBindValue(QJsonDocument(QJsonObject::fromVariantMap(details)).toJson(QJsonDocument::Compact));
    
    query.exec();
}

void DatabaseManager::saveVideoClipMetadata(const QString& clipId, const QString& path, const QVariantMap& metadata) {
    if (!isOpen()) return;
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO video_clips (clip_id, path, metadata)
        VALUES (?, ?, ?)
    )");
    query.addBindValue(clipId);
    query.addBindValue(path);
    query.addBindValue(QJsonDocument(QJsonObject::fromVariantMap(metadata)).toJson(QJsonDocument::Compact));
    
    query.exec();
}

void DatabaseManager::cleanup(int retentionDays) {
    if (!isOpen()) return;
    
    qint64 cutoffTime = QDateTime::currentMSecsSinceEpoch() - (retentionDays * 86400000LL);
    
    QSqlQuery query;
    query.prepare("DELETE FROM tracks WHERE timestamp < ?");
    query.addBindValue(cutoffTime);
    query.exec();
    
    query.prepare("DELETE FROM operator_actions WHERE timestamp < ?");
    query.addBindValue(cutoffTime);
    query.exec();
    
    Logger::instance().info("DatabaseManager", QString("Cleaned up records older than %1 days").arg(retentionDays));
}

QList<Track*> DatabaseManager::loadTrackHistory(const QDateTime& start, const QDateTime& end) {
    Q_UNUSED(start)
    Q_UNUSED(end)
    return QList<Track*>();
}

QList<EngagementRecord> DatabaseManager::loadEngagements(const QDateTime& start, const QDateTime& end) {
    Q_UNUSED(start)
    Q_UNUSED(end)
    return QList<EngagementRecord>();
}

} // namespace CounterUAS
