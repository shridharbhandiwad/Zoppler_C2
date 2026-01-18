#include "simulators/TrackSimulator.h"
#include "core/TrackManager.h"
#include <QRandomGenerator>
#include <QtMath>

namespace CounterUAS {

TrackSimulator::TrackSimulator(TrackManager* manager, QObject* parent)
    : QObject(parent), m_trackManager(manager),
      m_updateTimer(new QTimer(this)), m_spawnTimer(new QTimer(this)) {
    m_basePosition.latitude = 34.0522;
    m_basePosition.longitude = -118.2437;
    m_basePosition.altitude = 100.0;
    
    connect(m_updateTimer, &QTimer::timeout, this, &TrackSimulator::updateTargets);
    connect(m_spawnTimer, &QTimer::timeout, this, &TrackSimulator::spawnTarget);
}

void TrackSimulator::start() {
    m_updateTimer->start(100);  // Update at 10Hz
    if (m_autoSpawnEnabled) {
        m_spawnTimer->start(m_spawnInterval);
        spawnTarget();  // Create initial target
    }
}

void TrackSimulator::stop() {
    m_updateTimer->stop();
    m_spawnTimer->stop();
}

void TrackSimulator::setAutoSpawnEnabled(bool enabled) {
    m_autoSpawnEnabled = enabled;
    if (m_updateTimer->isActive()) {
        if (enabled && !m_spawnTimer->isActive()) {
            m_spawnTimer->start(m_spawnInterval);
        } else if (!enabled && m_spawnTimer->isActive()) {
            m_spawnTimer->stop();
        }
    }
}

void TrackSimulator::setSpawnInterval(int msec) {
    m_spawnInterval = msec;
    if (m_spawnTimer->isActive()) {
        m_spawnTimer->setInterval(msec);
    }
}

void TrackSimulator::addTarget(const SimulatedTarget& target) {
    if (m_targets.size() >= m_maxTargets) return;
    m_targets.append(target);
    emit targetInjected(target.id, target.position);
}

void TrackSimulator::clearTargets() {
    for (const auto& target : m_targets) {
        emit targetRemoved(target.id);
    }
    m_targets.clear();
}

QString TrackSimulator::injectTarget(const ManualTargetParams& params) {
    if (m_targets.size() >= m_maxTargets) {
        return QString();  // At max capacity
    }
    
    auto* gen = QRandomGenerator::global();
    
    SimulatedTarget target;
    target.id = QString("MAN-%1").arg(gen->bounded(10000));
    
    // Calculate position from range and bearing
    double bearingRad = qDegreesToRadians(params.bearingDeg);
    target.position.latitude = m_basePosition.latitude + 
                               (params.rangeM * std::cos(bearingRad)) / 111000.0;
    target.position.longitude = m_basePosition.longitude + 
                                (params.rangeM * std::sin(bearingRad)) / 
                                (111000.0 * std::cos(qDegreesToRadians(m_basePosition.latitude)));
    target.position.altitude = m_basePosition.altitude + params.altitudeM;
    
    // Calculate velocity from speed and heading
    double headingRad = qDegreesToRadians(params.headingDeg);
    target.velocity.north = params.speedMps * std::cos(headingRad);
    target.velocity.east = params.speedMps * std::sin(headingRad);
    target.velocity.down = -params.climbRateMps;  // Positive down = descending
    
    target.classification = params.classification;
    target.active = true;
    
    m_targets.append(target);
    emit targetInjected(target.id, target.position);
    
    return target.id;
}

void TrackSimulator::updateTargets() {
    if (!m_trackManager) return;
    
    double dt = 0.1;  // 100ms
    QList<QString> removedIds;
    
    for (auto& target : m_targets) {
        if (!target.active) continue;
        
        // Update position
        target.position.latitude += (target.velocity.north * dt) / 111000.0;
        target.position.longitude += (target.velocity.east * dt) / 
                                      (111000.0 * std::cos(qDegreesToRadians(target.position.latitude)));
        target.position.altitude -= target.velocity.down * dt;
        
        // Add some noise
        auto* gen = QRandomGenerator::global();
        target.position.latitude += gen->generateDouble() * 0.00002 - 0.00001;
        target.position.longitude += gen->generateDouble() * 0.00002 - 0.00001;
        
        // Report to track manager
        m_trackManager->processRadarDetection(target.position, target.velocity, 
                                              0.9, QDateTime::currentMSecsSinceEpoch());
        
        // Check if target has passed through base or left range
        Track tempTrack("temp");
        tempTrack.setPosition(m_basePosition);
        double dist = tempTrack.distanceTo(target.position);
        
        if (dist < 100.0 || dist > 5000.0) {
            target.active = false;
            removedIds.append(target.id);
        }
    }
    
    // Remove inactive targets and emit signals
    m_targets.erase(std::remove_if(m_targets.begin(), m_targets.end(),
        [](const SimulatedTarget& t) { return !t.active; }), m_targets.end());
    
    for (const QString& id : removedIds) {
        emit targetRemoved(id);
    }
}

void TrackSimulator::spawnTarget() {
    if (!m_autoSpawnEnabled) return;
    if (m_targets.size() >= m_maxTargets) return;
    
    auto* gen = QRandomGenerator::global();
    
    SimulatedTarget target;
    target.id = QString("SIM-%1").arg(gen->bounded(10000));
    
    // Spawn at edge of range
    double range = 2000.0 + gen->generateDouble() * 1000.0;
    double bearing = gen->generateDouble() * 360.0;
    double bearingRad = qDegreesToRadians(bearing);
    
    target.position.latitude = m_basePosition.latitude + (range * std::cos(bearingRad)) / 111000.0;
    target.position.longitude = m_basePosition.longitude + 
                                 (range * std::sin(bearingRad)) / 
                                 (111000.0 * std::cos(qDegreesToRadians(m_basePosition.latitude)));
    target.position.altitude = m_basePosition.altitude + 50.0 + gen->generateDouble() * 250.0;
    
    // Velocity toward base
    double speed = 8.0 + gen->generateDouble() * 12.0;
    double velBearing = bearing + 180.0 + (gen->generateDouble() * 40.0 - 20.0);
    
    target.velocity.north = speed * std::cos(qDegreesToRadians(velBearing));
    target.velocity.east = speed * std::sin(qDegreesToRadians(velBearing));
    target.velocity.down = gen->generateDouble() * 2.0 - 1.0;
    
    target.classification = gen->bounded(100) < 70 ? 
                            TrackClassification::Hostile : TrackClassification::Pending;
    target.active = true;
    
    m_targets.append(target);
    emit targetInjected(target.id, target.position);
}

} // namespace CounterUAS
