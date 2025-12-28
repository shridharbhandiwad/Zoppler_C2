#include "sensors/SensorInterface.h"
#include "utils/Logger.h"

namespace CounterUAS {

SensorInterface::SensorInterface(const QString& sensorId, QObject* parent)
    : QObject(parent)
    , m_sensorId(sensorId)
    , m_name(sensorId)
    , m_updateTimer(new QTimer(this))
    , m_healthTimer(new QTimer(this))
{
    connect(m_updateTimer, &QTimer::timeout, this, &SensorInterface::processData);
    connect(m_healthTimer, &QTimer::timeout, this, &SensorInterface::updateHealth);
    
    m_healthTimer->setInterval(1000);  // Health check every second
}

void SensorInterface::setUpdateRate(int hz) {
    m_updateRateHz = qBound(1, hz, 100);
    if (m_running) {
        m_updateTimer->setInterval(1000 / m_updateRateHz);
    }
}

void SensorInterface::start() {
    if (m_running) return;
    
    if (!isConnected()) {
        if (!connect()) {
            reportError("Failed to connect sensor");
            return;
        }
    }
    
    m_updateTimer->setInterval(1000 / m_updateRateHz);
    m_updateTimer->start();
    m_healthTimer->start();
    m_running = true;
    
    setStatus(SensorStatus::Online);
    
    Logger::instance().info("Sensor", 
                           QString("%1 started at %2 Hz").arg(m_sensorId).arg(m_updateRateHz));
}

void SensorInterface::stop() {
    if (!m_running) return;
    
    m_updateTimer->stop();
    m_healthTimer->stop();
    m_running = false;
    
    Logger::instance().info("Sensor", m_sensorId + " stopped");
}

void SensorInterface::setStatus(SensorStatus status) {
    if (m_health.status != status) {
        m_health.status = status;
        emit statusChanged(status);
        emit healthUpdated(m_health);
    }
}

void SensorInterface::reportError(const QString& message) {
    m_health.errorMessage = message;
    setStatus(SensorStatus::Error);
    
    Logger::instance().error("Sensor", m_sensorId + ": " + message);
    emit error(message);
}

void SensorInterface::recordDetection() {
    m_health.detectionCount++;
    m_health.lastDetectionTime = QDateTime::currentMSecsSinceEpoch();
}

void SensorInterface::updateHealth() {
    // Update signal quality based on detection rate
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 timeSinceDetection = now - m_health.lastDetectionTime;
    
    if (timeSinceDetection < 1000) {
        m_health.signalQuality = 1.0;
    } else if (timeSinceDetection < 5000) {
        m_health.signalQuality = 0.8;
    } else if (timeSinceDetection < 10000) {
        m_health.signalQuality = 0.5;
    } else {
        m_health.signalQuality = 0.2;
    }
    
    // Check for degraded status
    if (m_health.status == SensorStatus::Online && 
        m_health.signalQuality < 0.5) {
        setStatus(SensorStatus::Degraded);
    } else if (m_health.status == SensorStatus::Degraded && 
               m_health.signalQuality >= 0.8) {
        setStatus(SensorStatus::Online);
    }
    
    emit healthUpdated(m_health);
}

} // namespace CounterUAS
