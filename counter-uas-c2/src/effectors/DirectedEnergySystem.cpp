#include "effectors/DirectedEnergySystem.h"
#include "utils/Logger.h"
#include <QDateTime>

namespace CounterUAS {

DirectedEnergySystem::DirectedEnergySystem(const QString& effectorId, QObject* parent)
    : EffectorInterface(effectorId, parent)
    , m_dwellTimer(new QTimer(this))
    , m_cooldownTimer(new QTimer(this))
    , m_trackingTimer(new QTimer(this))
{
    m_dwellTimer->setSingleShot(true);
    m_cooldownTimer->setSingleShot(true);
    
    connect(m_dwellTimer, &QTimer::timeout, this, &DirectedEnergySystem::onDwellComplete);
    connect(m_cooldownTimer, &QTimer::timeout, this, &DirectedEnergySystem::onCooldownComplete);
    
    m_trackingTimer->setInterval(100);
    connect(m_trackingTimer, &QTimer::timeout, this, &DirectedEnergySystem::updateTracking);
    
    setDisplayName("Directed Energy");
}

DirectedEnergySystem::~DirectedEnergySystem() {
    disengage();
}

void DirectedEnergySystem::setConfig(const DESystemConfig& config) {
    m_config = config;
}

bool DirectedEnergySystem::engage(const GeoPosition& target) {
    if (!canEngage(target)) {
        Logger::instance().warning("DirectedEnergySystem",
                                  m_effectorId + " cannot engage target");
        return false;
    }
    
    m_currentTarget = target;
    m_currentPowerKW = m_config.maxPowerKW;
    
    setStatus(EffectorStatus::Engaged);
    
    // Send pointing command
    sendPointingCommand(target);
    
    // Start tracking
    startTracking();
    
    // Set dwell timeout
    int dwellMs = static_cast<int>(m_config.dwellTimeRequiredS * 1000);
    m_dwellTimer->start(dwellMs);
    
    m_health.totalEngagements++;
    m_health.lastEngagementTime = QDateTime::currentDateTimeUtc();
    
    Logger::instance().info("DirectedEnergySystem",
                           QString("%1 engaging target at %2m, power %3kW")
                               .arg(m_effectorId)
                               .arg(distanceToTarget(target), 0, 'f', 0)
                               .arg(m_currentPowerKW, 0, 'f', 1));
    
    emit engagementStarted(target);
    emit powerChanged(m_currentPowerKW);
    
    return true;
}

void DirectedEnergySystem::disengage() {
    if (!isEngaged()) return;
    
    m_dwellTimer->stop();
    stopTracking();
    
    m_currentPowerKW = 0.0;
    emit powerChanged(0.0);
    
    Logger::instance().info("DirectedEnergySystem", m_effectorId + " disengaged");
    
    emit engagementComplete(false);
    
    // Start cooldown
    setStatus(EffectorStatus::Cooldown);
    m_cooldownTimer->start(m_config.cooldownTimeMs);
}

void DirectedEnergySystem::setPower(double kw) {
    m_currentPowerKW = qBound(0.0, kw, m_config.maxPowerKW);
    emit powerChanged(m_currentPowerKW);
}

double DirectedEnergySystem::dwellTimeS() const {
    if (!m_tracking || m_dwellStartTime == 0) return 0.0;
    
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    return (now - m_dwellStartTime) / 1000.0;
}

void DirectedEnergySystem::onDwellComplete() {
    if (!isEngaged()) return;
    
    Logger::instance().info("DirectedEnergySystem",
                           m_effectorId + " dwell complete - target effect achieved");
    
    stopTracking();
    
    emit targetEffect();
    emit engagementComplete(true);
    
    // Cooldown
    setStatus(EffectorStatus::Cooldown);
    m_cooldownTimer->start(m_config.cooldownTimeMs);
}

void DirectedEnergySystem::onCooldownComplete() {
    setStatus(EffectorStatus::Ready);
    Logger::instance().info("DirectedEnergySystem", m_effectorId + " ready");
}

void DirectedEnergySystem::updateTracking() {
    if (!m_tracking) return;
    
    double dwell = dwellTimeS();
    emit trackingStatus(true, dwell);
}

void DirectedEnergySystem::sendPointingCommand(const GeoPosition& target) {
    // In real implementation, send pointing command to turret
    Q_UNUSED(target)
}

void DirectedEnergySystem::startTracking() {
    m_tracking = true;
    m_dwellStartTime = QDateTime::currentMSecsSinceEpoch();
    m_trackingTimer->start();
    emit trackingStatus(true, 0.0);
}

void DirectedEnergySystem::stopTracking() {
    m_tracking = false;
    m_dwellStartTime = 0;
    m_trackingTimer->stop();
    emit trackingStatus(false, 0.0);
}

} // namespace CounterUAS
