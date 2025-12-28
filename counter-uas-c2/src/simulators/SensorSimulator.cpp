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
    double range = 500.0 + gen->generateDouble() * 1500.0;
    double bearing = gen->generateDouble() * 360.0;
    
    double bearingRad = qDegreesToRadians(bearing);
    double latOffset = (range * std::cos(bearingRad)) / 111000.0;
    double lonOffset = (range * std::sin(bearingRad)) / (111000.0 * std::cos(qDegreesToRadians(m_basePosition.latitude)));
    
    GeoPosition pos;
    pos.latitude = m_basePosition.latitude + latOffset;
    pos.longitude = m_basePosition.longitude + lonOffset;
    pos.altitude = m_basePosition.altitude + 50.0 + gen->generateDouble() * 150.0;
    
    // Random velocity toward base
    double speed = 5.0 + gen->generateDouble() * 20.0;
    double velBearing = bearing + 180.0 + (gen->generateDouble() * 60.0 - 30.0);
    if (velBearing >= 360.0) velBearing -= 360.0;
    if (velBearing < 0.0) velBearing += 360.0;
    
    VelocityVector vel;
    vel.north = speed * std::cos(qDegreesToRadians(velBearing));
    vel.east = speed * std::sin(qDegreesToRadians(velBearing));
    vel.down = gen->generateDouble() * 4.0 - 2.0;
    
    m_trackManager->processRadarDetection(pos, vel, 0.6 + gen->generateDouble() * 0.4, 
                                          QDateTime::currentMSecsSinceEpoch());
}

} // namespace CounterUAS
