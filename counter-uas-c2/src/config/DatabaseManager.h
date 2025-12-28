#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "core/Track.h"
#include "core/EngagementManager.h"

namespace CounterUAS {

class DatabaseManager : public QObject {
    Q_OBJECT
    
public:
    static DatabaseManager& instance();
    
    bool initialize(const QString& path);
    void close();
    bool isOpen() const;
    
    // Track history
    void saveTrack(const Track& track);
    void saveTrackHistory(const QString& trackId, const GeoPosition& pos, qint64 timestamp);
    QList<Track*> loadTrackHistory(const QDateTime& start, const QDateTime& end);
    
    // Engagements
    void saveEngagement(const EngagementRecord& record);
    QList<EngagementRecord> loadEngagements(const QDateTime& start, const QDateTime& end);
    
    // Operator actions
    void logOperatorAction(const QString& operatorId, const QString& action, const QVariantMap& details);
    
    // Video clips
    void saveVideoClipMetadata(const QString& clipId, const QString& path, const QVariantMap& metadata);
    
    // Cleanup
    void cleanup(int retentionDays);
    
signals:
    void databaseError(const QString& error);
    
private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
    
    bool createTables();
    
    QSqlDatabase m_db;
    QString m_dbPath;
};

} // namespace CounterUAS

#endif // DATABASEMANAGER_H
