#include "core/TrackManager.h"
#include "utils/Logger.h"
#include <QReadLocker>
#include <QWriteLocker>
#include <algorithm>
#include <cmath>

namespace CounterUAS {

TrackManager::TrackManager(QObject* parent)
    : QObject(parent)
    , m_updateTimer(new QTimer(this))
{
    connect(m_updateTimer, &QTimer::timeout, this, &TrackManager::processTrackCycle);
}

TrackManager::~TrackManager() {
    stop();
    clearAllTracks();
}

void TrackManager::setConfig(const TrackManagerConfig& config) {
    m_config = config;
    if (m_running) {
        m_updateTimer->setInterval(1000 / m_config.updateRateHz);
    }
}

void TrackManager::start() {
    if (m_running) return;
    
    m_updateTimer->setInterval(1000 / m_config.updateRateHz);
    m_updateTimer->start();
    m_running = true;
    
    Logger::instance().info("TrackManager", "Started with update rate: " + 
                           QString::number(m_config.updateRateHz) + " Hz");
    emit runningChanged(true);
}

void TrackManager::stop() {
    if (!m_running) return;
    
    m_updateTimer->stop();
    m_running = false;
    
    Logger::instance().info("TrackManager", "Stopped");
    emit runningChanged(false);
}

int TrackManager::trackCount() const {
    QReadLocker locker(&m_lock);
    return m_tracks.size();
}

QList<Track*> TrackManager::allTracks() const {
    QReadLocker locker(&m_lock);
    return m_tracks.values();
}

Track* TrackManager::track(const QString& trackId) const {
    QReadLocker locker(&m_lock);
    return m_tracks.value(trackId, nullptr);
}

QList<Track*> TrackManager::tracksByClassification(TrackClassification cls) const {
    QReadLocker locker(&m_lock);
    QList<Track*> result;
    for (auto* t : m_tracks) {
        if (t->classification() == cls && t->state() != TrackState::Dropped) {
            result.append(t);
        }
    }
    return result;
}

QList<Track*> TrackManager::tracksByThreatLevel(int minLevel) const {
    QReadLocker locker(&m_lock);
    QList<Track*> result;
    for (auto* t : m_tracks) {
        if (t->threatLevel() >= minLevel && t->state() != TrackState::Dropped) {
            result.append(t);
        }
    }
    // Sort by threat level descending
    std::sort(result.begin(), result.end(), [](Track* a, Track* b) {
        return a->threatLevel() > b->threatLevel();
    });
    return result;
}

QList<Track*> TrackManager::tracksInRadius(const GeoPosition& center, double radiusM) const {
    QReadLocker locker(&m_lock);
    QList<Track*> result;
    
    // Create temporary track for distance calculation
    Track tempTrack("temp");
    tempTrack.setPosition(center);
    
    for (auto* t : m_tracks) {
        if (t->state() != TrackState::Dropped && 
            tempTrack.distanceTo(t->position()) <= radiusM) {
            result.append(t);
        }
    }
    return result;
}

QList<Track*> TrackManager::hostileTracks() const {
    return tracksByClassification(TrackClassification::Hostile);
}

QList<Track*> TrackManager::pendingTracks() const {
    return tracksByClassification(TrackClassification::Pending);
}

Track* TrackManager::highestThreatTrack() const {
    QReadLocker locker(&m_lock);
    Track* highest = nullptr;
    int maxThreat = 0;
    
    for (auto* t : m_tracks) {
        if (t->state() != TrackState::Dropped && 
            t->classification() == TrackClassification::Hostile &&
            t->threatLevel() > maxThreat) {
            maxThreat = t->threatLevel();
            highest = t;
        }
    }
    return highest;
}

QString TrackManager::createTrack(const GeoPosition& pos, DetectionSource source) {
    QWriteLocker locker(&m_lock);
    
    if (m_tracks.size() >= m_config.maxTracks) {
        Logger::instance().warning("TrackManager", "Maximum track limit reached");
        return QString();
    }
    
    QString trackId = generateTrackId();
    Track* newTrack = new Track(trackId, this);
    newTrack->setPosition(pos);
    newTrack->addDetectionSource(source);
    newTrack->setState(TrackState::Initiated);
    newTrack->setClassification(TrackClassification::Pending);
    
    m_tracks.insert(trackId, newTrack);
    
    // Create Kalman filter for this track
    if (m_config.enableKalmanFilter) {
        auto filter = std::make_shared<KalmanFilter2D>();
        filter->initialize(pos.latitude, pos.longitude);
        m_kalmanFilters.insert(trackId, filter);
    }
    
    m_stats.totalTracksCreated++;
    m_stats.currentActiveCount = m_tracks.size();
    
    locker.unlock();
    
    Logger::instance().info("TrackManager", "Created track: " + trackId);
    emit trackCreated(trackId);
    emit trackCountChanged(m_tracks.size());
    
    return trackId;
}

void TrackManager::updateTrack(const QString& trackId, const GeoPosition& pos) {
    QWriteLocker locker(&m_lock);
    
    Track* t = m_tracks.value(trackId);
    if (!t) return;
    
    GeoPosition filteredPos = pos;
    
    // Apply Kalman filter if enabled
    if (m_config.enableKalmanFilter && m_kalmanFilters.contains(trackId)) {
        auto it = m_kalmanFilters.find(trackId);
        if (it != m_kalmanFilters.end()) {
            it.value()->update(pos.latitude, pos.longitude);
            filteredPos.latitude = it.value()->stateX();
            filteredPos.longitude = it.value()->stateY();
        }
    }
    
    t->setPosition(filteredPos);
    t->addPositionHistory(filteredPos, QDateTime::currentMSecsSinceEpoch());
    t->resetCoastCount();
    
    if (t->state() == TrackState::Initiated || t->state() == TrackState::Coasting) {
        t->setState(TrackState::Active);
    }
    
    m_stats.lastUpdateTimeMs = QDateTime::currentMSecsSinceEpoch();
    
    locker.unlock();
    
    emit trackUpdated(trackId);
}

void TrackManager::updateTrackVelocity(const QString& trackId, const VelocityVector& vel) {
    QWriteLocker locker(&m_lock);
    
    Track* t = m_tracks.value(trackId);
    if (!t) return;
    
    t->setVelocity(vel);
    
    locker.unlock();
    
    emit trackUpdated(trackId);
}

void TrackManager::setTrackClassification(const QString& trackId, TrackClassification cls, double confidence) {
    QWriteLocker locker(&m_lock);
    
    Track* t = m_tracks.value(trackId);
    if (!t) return;
    
    t->setClassification(cls);
    t->setClassificationConfidence(confidence);
    
    locker.unlock();
    
    emit trackClassificationChanged(trackId, cls);
}

void TrackManager::setTrackThreatLevel(const QString& trackId, int level) {
    QWriteLocker locker(&m_lock);
    
    Track* t = m_tracks.value(trackId);
    if (!t) return;
    
    int oldLevel = t->threatLevel();
    t->setThreatLevel(level);
    
    locker.unlock();
    
    emit trackThreatLevelChanged(trackId, level);
    
    if (level >= 4 && level > oldLevel) {
        emit highThreatDetected(trackId, level);
    }
}

void TrackManager::setTrackBoundingBox(const QString& trackId, const BoundingBox& box) {
    QWriteLocker locker(&m_lock);
    
    Track* t = m_tracks.value(trackId);
    if (!t) return;
    
    t->setBoundingBox(box);
}

void TrackManager::associateCamera(const QString& trackId, const QString& cameraId) {
    QWriteLocker locker(&m_lock);
    
    Track* t = m_tracks.value(trackId);
    if (!t) return;
    
    t->setAssociatedCameraId(cameraId);
    t->setVisuallyTracked(true);
}

void TrackManager::dropTrack(const QString& trackId) {
    QWriteLocker locker(&m_lock);
    
    Track* t = m_tracks.value(trackId);
    if (!t) return;
    
    t->setState(TrackState::Dropped);
    m_stats.totalTracksDropped++;
    
    locker.unlock();
    
    Logger::instance().info("TrackManager", "Dropped track: " + trackId);
    emit trackDropped(trackId);
    emit trackStateChanged(trackId, TrackState::Dropped);
}

void TrackManager::promoteTrack(const QString& trackId) {
    QWriteLocker locker(&m_lock);
    
    Track* t = m_tracks.value(trackId);
    if (!t) return;
    
    t->setTrackQuality(1.0);
    t->setState(TrackState::Active);
    
    locker.unlock();
    
    emit trackStateChanged(trackId, TrackState::Active);
}

void TrackManager::mergeTrack(const QString& sourceId, const QString& targetId) {
    QWriteLocker locker(&m_lock);
    
    Track* source = m_tracks.value(sourceId);
    Track* target = m_tracks.value(targetId);
    
    if (!source || !target) return;
    
    // Merge detection sources
    for (auto src : source->detectionSources()) {
        target->addDetectionSource(src);
    }
    
    // Drop source track
    source->setState(TrackState::Dropped);
    m_stats.totalTracksDropped++;
    m_stats.correlationSuccessCount++;
    
    locker.unlock();
    
    Logger::instance().info("TrackManager", 
                           QString("Merged track %1 into %2").arg(sourceId, targetId));
    emit trackDropped(sourceId);
}

void TrackManager::processRadarDetection(const GeoPosition& pos, const VelocityVector& vel,
                                         double quality, qint64 timestamp) {
    Q_UNUSED(timestamp)
    
    Track* correlated = findCorrelatedTrack(pos, vel, DetectionSource::Radar);
    
    if (correlated) {
        updateTrack(correlated->trackId(), pos);
        updateTrackVelocity(correlated->trackId(), vel);
        correlated->addDetectionSource(DetectionSource::Radar);
        correlated->setTrackQuality(qMax(correlated->trackQuality(), quality));
    } else {
        QString newId = createTrack(pos, DetectionSource::Radar);
        if (!newId.isEmpty()) {
            updateTrackVelocity(newId, vel);
        }
    }
}

void TrackManager::processRFDetection(const GeoPosition& pos, double signalStrength,
                                      qint64 timestamp) {
    Q_UNUSED(timestamp)
    
    VelocityVector emptyVel;
    Track* correlated = findCorrelatedTrack(pos, emptyVel, DetectionSource::RFDetector);
    
    if (correlated) {
        updateTrack(correlated->trackId(), pos);
        correlated->addDetectionSource(DetectionSource::RFDetector);
        // RF detection increases confidence it's a drone
        if (signalStrength > 0.7 && 
            correlated->classification() == TrackClassification::Pending) {
            setTrackClassification(correlated->trackId(), 
                                   TrackClassification::Hostile, 0.6);
        }
    } else {
        createTrack(pos, DetectionSource::RFDetector);
    }
}

void TrackManager::processCameraDetection(const QString& cameraId, const BoundingBox& box,
                                          const GeoPosition& estimatedPos, qint64 timestamp) {
    Q_UNUSED(timestamp)
    
    VelocityVector emptyVel;
    Track* correlated = findCorrelatedTrack(estimatedPos, emptyVel, DetectionSource::Camera);
    
    if (correlated) {
        correlated->setBoundingBox(box);
        correlated->setAssociatedCameraId(cameraId);
        correlated->setVisuallyTracked(true);
        correlated->addDetectionSource(DetectionSource::Camera);
    } else {
        QString newId = createTrack(estimatedPos, DetectionSource::Camera);
        if (!newId.isEmpty()) {
            Track* t = track(newId);
            if (t) {
                t->setBoundingBox(box);
                t->setAssociatedCameraId(cameraId);
                t->setVisuallyTracked(true);
            }
        }
    }
}

void TrackManager::clearAllTracks() {
    // First, collect all track IDs while holding the lock
    QList<QString> trackIds;
    {
        QReadLocker locker(&m_lock);
        trackIds = m_tracks.keys();
    }
    
    // Emit trackDropped signals outside of write lock to allow connected slots
    // to safely access track data before deletion
    for (const QString& id : trackIds) {
        emit trackDropped(id);
    }
    
    // Now acquire write lock and delete all tracks
    {
        QWriteLocker locker(&m_lock);
        for (auto* t : m_tracks) {
            delete t;
        }
        m_tracks.clear();
        m_kalmanFilters.clear();
    }
    
    emit trackCountChanged(0);
}

void TrackManager::pruneDroppedTracks() {
    QWriteLocker locker(&m_lock);
    
    QList<QString> toRemove;
    for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
        if (it.value()->state() == TrackState::Dropped) {
            toRemove.append(it.key());
        }
    }
    
    for (const QString& id : toRemove) {
        delete m_tracks.take(id);
        m_kalmanFilters.remove(id);
    }
    
    int newCount = m_tracks.size();
    locker.unlock();
    
    if (!toRemove.isEmpty()) {
        emit trackCountChanged(newCount);
    }
}

void TrackManager::onSensorData(const GeoPosition& pos, const VelocityVector& vel,
                                DetectionSource source, qint64 timestamp) {
    switch (source) {
        case DetectionSource::Radar:
            processRadarDetection(pos, vel, 0.8, timestamp);
            break;
        case DetectionSource::RFDetector:
            processRFDetection(pos, 0.5, timestamp);
            break;
        default:
            break;
    }
}

void TrackManager::processTrackCycle() {
    QWriteLocker locker(&m_lock);
    
    int coastingCount = 0;
    QList<QString> toUpdate;
    
    for (auto* t : m_tracks) {
        if (t->state() == TrackState::Dropped) continue;
        
        updateTrackState(t);
        
        if (t->state() == TrackState::Coasting) {
            coastingCount++;
        }
        
        toUpdate.append(t->trackId());
    }
    
    m_stats.currentCoastingCount = coastingCount;
    m_stats.currentActiveCount = m_tracks.size() - coastingCount;
    
    locker.unlock();
    
    // Emit updates outside of lock
    for (const QString& id : toUpdate) {
        emit trackUpdated(id);
    }
}

Track* TrackManager::findCorrelatedTrack(const GeoPosition& pos, const VelocityVector& vel,
                                         DetectionSource source) {
    QReadLocker locker(&m_lock);
    
    Track* bestMatch = nullptr;
    double bestScore = 0.0;
    
    for (auto* t : m_tracks) {
        if (t->state() == TrackState::Dropped) continue;
        
        double score = calculateCorrelationScore(t, pos, vel);
        if (score > bestScore && score > 0.5) {  // Minimum correlation threshold
            bestScore = score;
            bestMatch = t;
        }
    }
    
    return bestMatch;
}

double TrackManager::calculateCorrelationScore(Track* track, const GeoPosition& pos,
                                               const VelocityVector& vel) {
    // Distance component
    double distance = track->distanceTo(pos);
    double distanceScore = 1.0;
    if (distance > m_config.correlationDistanceM) {
        distanceScore = 0.0;
    } else {
        distanceScore = 1.0 - (distance / m_config.correlationDistanceM);
    }
    
    // Velocity component (if available)
    double velocityScore = 1.0;
    VelocityVector trackVel = track->velocity();
    double velDiff = std::sqrt(
        std::pow(trackVel.north - vel.north, 2) +
        std::pow(trackVel.east - vel.east, 2) +
        std::pow(trackVel.down - vel.down, 2)
    );
    if (velDiff > m_config.correlationVelocityMps) {
        velocityScore = 0.5;  // Penalize but don't eliminate
    } else {
        velocityScore = 1.0 - (velDiff / (2 * m_config.correlationVelocityMps));
    }
    
    // Time since update component (prefer recent tracks)
    double timeScore = 1.0;
    qint64 timeSince = track->timeSinceUpdate();
    if (timeSince > m_config.coastingTimeoutMs) {
        timeScore = 0.3;
    } else {
        timeScore = 1.0 - (static_cast<double>(timeSince) / m_config.coastingTimeoutMs * 0.5);
    }
    
    return distanceScore * 0.5 + velocityScore * 0.3 + timeScore * 0.2;
}

void TrackManager::updateTrackState(Track* track) {
    qint64 timeSinceUpdate = track->timeSinceUpdate();
    
    if (track->state() == TrackState::Active) {
        if (timeSinceUpdate > m_config.coastingTimeoutMs) {
            track->setState(TrackState::Coasting);
            track->incrementCoastCount();
            emit trackStateChanged(track->trackId(), TrackState::Coasting);
        }
    } else if (track->state() == TrackState::Coasting) {
        if (timeSinceUpdate > m_config.dropTimeoutMs || 
            track->coastCount() > m_config.maxCoastCount) {
            track->setState(TrackState::Dropped);
            m_stats.totalTracksDropped++;
            emit trackStateChanged(track->trackId(), TrackState::Dropped);
            emit trackDropped(track->trackId());
        } else {
            track->incrementCoastCount();
        }
    } else if (track->state() == TrackState::Initiated) {
        // Auto-promote to active if we have updates
        if (track->timeSinceUpdate() < m_config.coastingTimeoutMs) {
            track->setState(TrackState::Active);
            emit trackStateChanged(track->trackId(), TrackState::Active);
        }
    }
}

void TrackManager::applyKalmanFilter(Track* track, const GeoPosition& measurement) {
    if (!m_config.enableKalmanFilter) return;
    
    QString trackId = track->trackId();
    if (!m_kalmanFilters.contains(trackId)) {
        auto filter = std::make_shared<KalmanFilter2D>();
        filter->initialize(measurement.latitude, measurement.longitude);
        m_kalmanFilters.insert(trackId, filter);
        return;
    }
    
    auto it = m_kalmanFilters.find(trackId);
    if (it != m_kalmanFilters.end()) {
        it.value()->update(measurement.latitude, measurement.longitude);
    }
}

QString TrackManager::generateTrackId() {
    return QString("TRK-%1").arg(m_nextTrackNumber++, 4, 10, QChar('0'));
}

} // namespace CounterUAS
