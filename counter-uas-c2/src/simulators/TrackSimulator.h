#ifndef TRACKSIMULATOR_H
#define TRACKSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QList>
#include "core/Track.h"

namespace CounterUAS {
class TrackManager;

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
    void addTarget(const SimulatedTarget& target);
    void clearTargets();
    
private slots:
    void updateTargets();
    void spawnTarget();
    
private:
    TrackManager* m_trackManager;
    QTimer* m_updateTimer;
    QTimer* m_spawnTimer;
    GeoPosition m_basePosition;
    QList<SimulatedTarget> m_targets;
};

} // namespace CounterUAS
#endif
