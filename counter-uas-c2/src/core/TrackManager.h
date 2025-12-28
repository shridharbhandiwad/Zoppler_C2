#ifndef TRACKMANAGER_H
#define TRACKMANAGER_H

#include <QObject>
#include <QHash>
#include <QList>
#include <QTimer>
#include <QMutex>
#include <QReadWriteLock>
#include <memory>

#include "core/Track.h"
#include "utils/KalmanFilter.h"

namespace CounterUAS {

/**
 * @brief Configuration for track management
 */
struct TrackManagerConfig {
    int updateRateHz = 10;               // Track update rate
    double correlationDistanceM = 100.0; // Max distance to correlate tracks
    double correlationVelocityMps = 10.0;// Max velocity difference for correlation
    int coastingTimeoutMs = 5000;        // Time before track starts coasting
    int dropTimeoutMs = 15000;           // Time before track is dropped
    int maxCoastCount = 10;              // Max coast cycles before drop
    bool enableKalmanFilter = true;      // Enable position smoothing
    int maxTracks = 200;                 // Maximum concurrent tracks
    int historyRetentionMs = 60000;      // Position history retention
};

/**
 * @brief Track Manager Engine for multi-sensor track fusion and lifecycle management
 */
class TrackManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int trackCount READ trackCount NOTIFY trackCountChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    
public:
    explicit TrackManager(QObject* parent = nullptr);
    ~TrackManager() override;
    
    // Configuration
    void setConfig(const TrackManagerConfig& config);
    TrackManagerConfig config() const { return m_config; }
    
    // Lifecycle
    void start();
    void stop();
    bool isRunning() const { return m_running; }
    
    // Track access
    int trackCount() const;
    QList<Track*> allTracks() const;
    Track* track(const QString& trackId) const;
    QList<Track*> tracksByClassification(TrackClassification cls) const;
    QList<Track*> tracksByThreatLevel(int minLevel) const;
    QList<Track*> tracksInRadius(const GeoPosition& center, double radiusM) const;
    QList<Track*> hostileTracks() const;
    QList<Track*> pendingTracks() const;
    Track* highestThreatTrack() const;
    
    // Track creation and update
    QString createTrack(const GeoPosition& pos, DetectionSource source);
    void updateTrack(const QString& trackId, const GeoPosition& pos);
    void updateTrackVelocity(const QString& trackId, const VelocityVector& vel);
    void setTrackClassification(const QString& trackId, TrackClassification cls, double confidence = 1.0);
    void setTrackThreatLevel(const QString& trackId, int level);
    void setTrackBoundingBox(const QString& trackId, const BoundingBox& box);
    void associateCamera(const QString& trackId, const QString& cameraId);
    
    // Manual track management
    void dropTrack(const QString& trackId);
    void promoteTrack(const QString& trackId);  // Manual confirmation
    void mergeTrack(const QString& sourceId, const QString& targetId);
    
    // Multi-sensor fusion
    void processRadarDetection(const GeoPosition& pos, const VelocityVector& vel, 
                               double quality, qint64 timestamp);
    void processRFDetection(const GeoPosition& pos, double signalStrength,
                            qint64 timestamp);
    void processCameraDetection(const QString& cameraId, const BoundingBox& box,
                                const GeoPosition& estimatedPos, qint64 timestamp);
    
    // Batch operations
    void clearAllTracks();
    void pruneDroppedTracks();
    
    // Statistics
    struct Statistics {
        int totalTracksCreated = 0;
        int totalTracksDropped = 0;
        int currentActiveCount = 0;
        int currentCoastingCount = 0;
        int correlationSuccessCount = 0;
        qint64 lastUpdateTimeMs = 0;
    };
    Statistics statistics() const { return m_stats; }
    
signals:
    void trackCreated(const QString& trackId);
    void trackUpdated(const QString& trackId);
    void trackClassificationChanged(const QString& trackId, TrackClassification cls);
    void trackThreatLevelChanged(const QString& trackId, int level);
    void trackStateChanged(const QString& trackId, TrackState state);
    void trackDropped(const QString& trackId);
    void trackCountChanged(int count);
    void highThreatDetected(const QString& trackId, int level);
    void runningChanged(bool running);
    
public slots:
    void onSensorData(const GeoPosition& pos, const VelocityVector& vel,
                      DetectionSource source, qint64 timestamp);
    
private slots:
    void processTrackCycle();
    
private:
    // Track correlation
    Track* findCorrelatedTrack(const GeoPosition& pos, const VelocityVector& vel,
                                DetectionSource source);
    double calculateCorrelationScore(Track* track, const GeoPosition& pos,
                                     const VelocityVector& vel);
    
    // Track lifecycle
    void updateTrackState(Track* track);
    void applyKalmanFilter(Track* track, const GeoPosition& measurement);
    QString generateTrackId();
    
    mutable QReadWriteLock m_lock;
    QHash<QString, Track*> m_tracks;
    QHash<QString, std::shared_ptr<KalmanFilter2D>> m_kalmanFilters;
    
    TrackManagerConfig m_config;
    QTimer* m_updateTimer;
    bool m_running = false;
    
    Statistics m_stats;
    int m_nextTrackNumber = 1;
};

} // namespace CounterUAS

#endif // TRACKMANAGER_H
