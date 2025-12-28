#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector3D>
#include <QList>
#include <QMutex>
#include <QJsonObject>

namespace CounterUAS {

/**
 * @brief Track classification enum
 */
enum class TrackClassification {
    Unknown = 0,
    Friendly,
    Hostile,
    Pending,
    Neutral
};

/**
 * @brief Track state in lifecycle
 */
enum class TrackState {
    Initiated = 0,
    Active,
    Coasting,
    Dropped
};

/**
 * @brief Detection source type
 */
enum class DetectionSource {
    Radar = 0,
    RFDetector,
    Camera,
    Combined,
    Manual
};

/**
 * @brief Position data structure
 */
struct GeoPosition {
    double latitude = 0.0;   // degrees
    double longitude = 0.0;  // degrees
    double altitude = 0.0;   // meters AGL
    
    bool isValid() const {
        return latitude >= -90.0 && latitude <= 90.0 &&
               longitude >= -180.0 && longitude <= 180.0;
    }
    
    QJsonObject toJson() const;
    static GeoPosition fromJson(const QJsonObject& json);
};

/**
 * @brief Velocity vector structure
 */
struct VelocityVector {
    double north = 0.0;  // m/s
    double east = 0.0;   // m/s
    double down = 0.0;   // m/s
    
    double speed() const;
    double heading() const;  // degrees from north
    double climbRate() const;
    
    QJsonObject toJson() const;
    static VelocityVector fromJson(const QJsonObject& json);
};

/**
 * @brief Bounding box for video overlay
 */
struct BoundingBox {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    QString cameraId;
    qint64 timestamp = 0;
    
    bool isValid() const { return width > 0 && height > 0; }
};

/**
 * @brief Track class representing a detected target
 */
class Track : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString trackId READ trackId CONSTANT)
    Q_PROPERTY(int threatLevel READ threatLevel WRITE setThreatLevel NOTIFY threatLevelChanged)
    Q_PROPERTY(TrackClassification classification READ classification WRITE setClassification NOTIFY classificationChanged)
    
public:
    explicit Track(const QString& id, QObject* parent = nullptr);
    Track(const Track& other);
    ~Track() override = default;
    
    // Unique identifier
    QString trackId() const { return m_trackId; }
    
    // Position management
    GeoPosition position() const;
    void setPosition(const GeoPosition& pos);
    GeoPosition predictedPosition(qint64 deltaMs) const;
    
    // Velocity
    VelocityVector velocity() const;
    void setVelocity(const VelocityVector& vel);
    
    // Classification
    TrackClassification classification() const { return m_classification; }
    void setClassification(TrackClassification cls);
    QString classificationString() const;
    
    // Threat level (1-5)
    int threatLevel() const { return m_threatLevel; }
    void setThreatLevel(int level);
    
    // Track state
    TrackState state() const { return m_state; }
    void setState(TrackState state);
    QString stateString() const;
    
    // Detection sources
    QList<DetectionSource> detectionSources() const { return m_detectionSources; }
    void addDetectionSource(DetectionSource source);
    void clearDetectionSources();
    bool hasSource(DetectionSource source) const;
    
    // Timestamps
    QDateTime createdTime() const { return m_createdTime; }
    QDateTime lastUpdateTime() const { return m_lastUpdateTime; }
    qint64 trackAge() const;  // milliseconds since creation
    qint64 timeSinceUpdate() const;  // milliseconds since last update
    
    // Associated camera
    QString associatedCameraId() const { return m_associatedCameraId; }
    void setAssociatedCameraId(const QString& cameraId);
    
    // Visual tracking
    bool isVisuallyTracked() const { return m_visuallyTracked; }
    void setVisuallyTracked(bool tracked);
    
    // Bounding box for video overlay
    BoundingBox boundingBox() const { return m_boundingBox; }
    void setBoundingBox(const BoundingBox& box);
    
    // Classification confidence (0.0 - 1.0)
    double classificationConfidence() const { return m_classificationConfidence; }
    void setClassificationConfidence(double conf);
    
    // Engagement status
    bool isEngaged() const { return m_engaged; }
    void setEngaged(bool engaged);
    
    // Track quality (0.0 - 1.0)
    double trackQuality() const { return m_trackQuality; }
    void setTrackQuality(double quality);
    
    // Coasting counter
    int coastCount() const { return m_coastCount; }
    void incrementCoastCount();
    void resetCoastCount();
    
    // History for smoothing
    void addPositionHistory(const GeoPosition& pos, qint64 timestamp);
    QList<QPair<GeoPosition, qint64>> positionHistory() const;
    void clearHistory();
    
    // Serialization
    QJsonObject toJson() const;
    static Track* fromJson(const QJsonObject& json, QObject* parent = nullptr);
    
    // Distance calculations
    double distanceTo(const GeoPosition& pos) const;
    double distanceTo(const Track& other) const;
    double bearingTo(const GeoPosition& pos) const;
    
    // Update from another track (for fusion)
    void updateFrom(const Track& other);
    
signals:
    void positionChanged();
    void velocityChanged();
    void classificationChanged();
    void threatLevelChanged(int level);
    void stateChanged(TrackState state);
    void boundingBoxChanged();
    void updated();
    
private:
    mutable QMutex m_mutex;
    
    QString m_trackId;
    GeoPosition m_position;
    VelocityVector m_velocity;
    TrackClassification m_classification = TrackClassification::Unknown;
    TrackState m_state = TrackState::Initiated;
    int m_threatLevel = 1;
    
    QList<DetectionSource> m_detectionSources;
    
    QDateTime m_createdTime;
    QDateTime m_lastUpdateTime;
    
    QString m_associatedCameraId;
    bool m_visuallyTracked = false;
    BoundingBox m_boundingBox;
    
    double m_classificationConfidence = 0.0;
    bool m_engaged = false;
    double m_trackQuality = 1.0;
    int m_coastCount = 0;
    
    static constexpr int MAX_HISTORY_SIZE = 100;
    QList<QPair<GeoPosition, qint64>> m_positionHistory;
};

} // namespace CounterUAS

Q_DECLARE_METATYPE(CounterUAS::TrackClassification)
Q_DECLARE_METATYPE(CounterUAS::TrackState)
Q_DECLARE_METATYPE(CounterUAS::DetectionSource)

#endif // TRACK_H
