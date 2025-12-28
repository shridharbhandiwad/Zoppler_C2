#include "effectors/EffectorInterface.h"
#include "utils/Logger.h"

namespace CounterUAS {

EffectorInterface::EffectorInterface(const QString& effectorId, QObject* parent)
    : QObject(parent)
    , m_effectorId(effectorId)
    , m_displayName(effectorId)
{
    m_health.status = EffectorStatus::Offline;
}

bool EffectorInterface::canEngage(const GeoPosition& target) const {
    if (!isReady()) return false;
    
    double dist = distanceToTarget(target);
    return dist >= minRange() && dist <= maxRange();
}

void EffectorInterface::initialize() {
    setStatus(EffectorStatus::Initializing);
    
    Logger::instance().info("Effector", m_effectorId + " initializing");
    
    // Simulate initialization delay
    QTimer::singleShot(2000, this, [this]() {
        setStatus(EffectorStatus::Ready);
        Logger::instance().info("Effector", m_effectorId + " ready");
    });
}

void EffectorInterface::shutdown() {
    if (isEngaged()) {
        disengage();
    }
    
    setStatus(EffectorStatus::Offline);
    
    Logger::instance().info("Effector", m_effectorId + " shutdown");
}

void EffectorInterface::reset() {
    if (m_health.status == EffectorStatus::Fault) {
        m_health.faultMessage.clear();
        setStatus(EffectorStatus::Initializing);
        
        QTimer::singleShot(1000, this, [this]() {
            setStatus(EffectorStatus::Ready);
        });
    }
}

void EffectorInterface::setStatus(EffectorStatus status) {
    if (m_health.status != status) {
        bool wasReady = isReady();
        bool wasEngaged = isEngaged();
        
        m_health.status = status;
        emit statusChanged(status);
        emit healthUpdated(m_health);
        
        if (wasReady != isReady()) {
            emit readyChanged(isReady());
        }
        if (wasEngaged != isEngaged()) {
            emit engagedChanged(isEngaged());
        }
    }
}

void EffectorInterface::reportFault(const QString& message) {
    m_health.faultMessage = message;
    setStatus(EffectorStatus::Fault);
    
    Logger::instance().error("Effector", m_effectorId + " fault: " + message);
    emit fault(message);
}

double EffectorInterface::distanceToTarget(const GeoPosition& target) const {
    // Simplified distance calculation
    Track tempTrack("temp");
    tempTrack.setPosition(m_position);
    return tempTrack.distanceTo(target);
}

} // namespace CounterUAS
