#ifndef TRACKSIMULATOR_H
#define TRACKSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QList>
#include "core/Track.h"

namespace CounterUAS {
class TrackManager;

/**
 * @brief Parameters for manual target injection
 */
struct ManualTargetParams {
    double rangeM = 2000.0;         // Distance from base position (meters)
    double bearingDeg = 0.0;        // Bearing from base (degrees, 0=North)
    double altitudeM = 150.0;       // Altitude above ground (meters)
    double speedMps = 15.0;         // Speed in meters per second
    double headingDeg = 180.0;      // Target heading (degrees, 0=North)
    double climbRateMps = 0.0;      // Vertical speed (positive = climbing)
    TrackClassification classification = TrackClassification::Pending;
};

struct SimulatedTarget {
    QString id;
    GeoPosition position;
    VelocityVector velocity;
    TrackClassification classification;
    bool active = true;
};

class TrackSimulator : public QObject {
    Q_OBJECT
public:
    explicit TrackSimulator(TrackManager* manager, QObject* parent = nullptr);
    void start();
    void stop();
    void setBasePosition(const GeoPosition& pos) { m_basePosition = pos; }
    GeoPosition basePosition() const { return m_basePosition; }
    
    // Target management
    void addTarget(const SimulatedTarget& target);
    void clearTargets();
    int targetCount() const { return m_targets.size(); }
    
    // Manual target injection with parameters
    QString injectTarget(const ManualTargetParams& params);
    
    // Auto-spawn control
    void setAutoSpawnEnabled(bool enabled);
    bool autoSpawnEnabled() const { return m_autoSpawnEnabled; }
    
    // Spawn settings
    void setSpawnInterval(int msec);
    int spawnInterval() const { return m_spawnInterval; }
    void setMaxTargets(int max) { m_maxTargets = max; }
    int maxTargets() const { return m_maxTargets; }
    
signals:
    void targetInjected(const QString& targetId, const GeoPosition& position);
    void targetRemoved(const QString& targetId);
    
private slots:
    void updateTargets();
    void spawnTarget();
    
private:
    TrackManager* m_trackManager;
    QTimer* m_updateTimer;
    QTimer* m_spawnTimer;
    GeoPosition m_basePosition;
    QList<SimulatedTarget> m_targets;
    
    bool m_autoSpawnEnabled = true;
    int m_spawnInterval = 5000;  // milliseconds
    int m_maxTargets = 10;
};

} // namespace CounterUAS
#endif
