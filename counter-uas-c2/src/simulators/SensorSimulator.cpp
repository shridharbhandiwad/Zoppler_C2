#include "simulators/SensorSimulator.h"
#include "core/TrackManager.h"
#include <QRandomGenerator>
#include <QtMath>

namespace CounterUAS {

SensorSimulator::SensorSimulator(TrackManager* manager, QObject* parent)
    : QObject(parent), m_trackManager(manager), m_timer(new QTimer(this)) {
    m_basePosition.latitude = 34.0522;
    m_basePosition.longitude = -118.2437;
    m_basePosition.altitude = 100.0;
    
    connect(m_timer, &QTimer::timeout, this, &SensorSimulator::generateDetection);
}

void SensorSimulator::start() {
    m_timer->start(500);  // Generate detection every 500ms
}

void SensorSimulator::stop() {
    m_timer->stop();
}

void SensorSimulator::setBasePosition(const GeoPosition& pos) {
    m_basePosition = pos;
}

void SensorSimulator::generateDetection() {
    if (!m_trackManager) return;
    
    auto* gen = QRandomGenerator::global();
    
    // Random position within 2km of base
    double range = gen->bounded(500.0, 2000.0);
    double bearing = gen->bounded(360.0);
    
    double bearingRad = qDegreesToRadians(bearing);
    double latOffset = (range * std::cos(bearingRad)) / 111000.0;
    double lonOffset = (range * std::sin(bearingRad)) / (111000.0 * std::cos(qDegreesToRadians(m_basePosition.latitude)));
    
    GeoPosition pos;
    pos.latitude = m_basePosition.latitude + latOffset;
    pos.longitude = m_basePosition.longitude + lonOffset;
    pos.altitude = m_basePosition.altitude + gen->bounded(50.0, 200.0);
    
    // Random velocity toward base
    double speed = gen->bounded(5.0, 25.0);
    double velBearing = bearing + 180.0 + gen->bounded(-30.0, 30.0);
    if (velBearing >= 360.0) velBearing -= 360.0;
    if (velBearing < 0.0) velBearing += 360.0;
    
    VelocityVector vel;
    vel.north = speed * std::cos(qDegreesToRadians(velBearing));
    vel.east = speed * std::sin(qDegreesToRadians(velBearing));
    vel.down = gen->bounded(-2.0, 2.0);
    
    m_trackManager->processRadarDetection(pos, vel, gen->bounded(0.6, 1.0), 
                                          QDateTime::currentMSecsSinceEpoch());
}

} // namespace CounterUAS
