#ifndef SENSORSIMULATOR_H
#define SENSORSIMULATOR_H

#include <QObject>
#include <QTimer>
#include "core/Track.h"

namespace CounterUAS {
class TrackManager;

class SensorSimulator : public QObject {
    Q_OBJECT
public:
    explicit SensorSimulator(TrackManager* manager, QObject* parent = nullptr);
    void start();
    void stop();
    void setBasePosition(const GeoPosition& pos);
    
private slots:
    void generateDetection();
    
private:
    TrackManager* m_trackManager;
    QTimer* m_timer;
    GeoPosition m_basePosition;
};

} // namespace CounterUAS
#endif
