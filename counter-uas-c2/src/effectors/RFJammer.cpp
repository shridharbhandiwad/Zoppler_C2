#include "effectors/RFJammer.h"
#include "utils/Logger.h"
#include <QDataStream>
#include <QDateTime>

namespace CounterUAS {

RFJammer::RFJammer(const QString& effectorId, QObject* parent)
    : EffectorInterface(effectorId, parent)
    , m_socket(new QTcpSocket(this))
    , m_engagementTimer(new QTimer(this))
    , m_cooldownTimer(new QTimer(this))
{
    connect(m_socket, &QTcpSocket::connected, this, &RFJammer::onSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &RFJammer::onSocketDisconnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &RFJammer::onSocketError);
    
    m_engagementTimer->setSingleShot(true);
    connect(m_engagementTimer, &QTimer::timeout, this, &RFJammer::onEngagementTimeout);
    
    m_cooldownTimer->setSingleShot(true);
    connect(m_cooldownTimer, &QTimer::timeout, this, &RFJammer::onCooldownComplete);
    
    setDisplayName("RF Jammer");
}

RFJammer::~RFJammer() {
    disengage();
    disconnectFromJammer();
}

void RFJammer::setConfig(const RFJammerConfig& config) {
    m_config = config;
}

bool RFJammer::engage(const GeoPosition& target) {
    if (!canEngage(target)) {
        Logger::instance().warning("RFJammer",
                                  m_effectorId + " cannot engage target - out of range or not ready");
        return false;
    }
    
    m_currentTarget = target;
    
    // Connect to jammer if not connected
    if (!m_socket->state() == QAbstractSocket::ConnectedState) {
        if (!connectToJammer()) {
            // For simulation, continue anyway
            Logger::instance().warning("RFJammer",
                                      m_effectorId + " could not connect - simulating engagement");
        }
    }
    
    // Start jamming
    setStatus(EffectorStatus::Engaged);
    m_currentPowerW = m_config.defaultPowerW;
    m_activeFrequencies = m_config.jamFrequenciesMHz;
    m_engagementStartTime = QDateTime::currentMSecsSinceEpoch();
    
    sendJamCommand(true, m_activeFrequencies, m_currentPowerW);
    
    m_engagementTimer->start(m_config.engagementTimeMs);
    m_health.totalEngagements++;
    m_health.lastEngagementTime = QDateTime::currentDateTimeUtc();
    
    Logger::instance().info("RFJammer",
                           QString("%1 engaging target at %2m range, power %3W")
                               .arg(m_effectorId)
                               .arg(distanceToTarget(target), 0, 'f', 0)
                               .arg(m_currentPowerW, 0, 'f', 0));
    
    emit engagementStarted(target);
    emit jamming(true, m_currentPowerW);
    emit healthUpdated(m_health);
    
    return true;
}

void RFJammer::disengage() {
    if (!isEngaged()) return;
    
    m_engagementTimer->stop();
    
    sendJamCommand(false, {}, 0);
    
    m_currentPowerW = 0.0;
    m_activeFrequencies.clear();
    
    Logger::instance().info("RFJammer", m_effectorId + " disengaging");
    
    emit jamming(false, 0);
    emit engagementComplete(true);
    
    // Start cooldown
    setStatus(EffectorStatus::Cooldown);
    m_cooldownTimer->start(m_config.cooldownTimeMs);
}

void RFJammer::setJamFrequencies(const QList<double>& frequenciesMHz) {
    m_config.jamFrequenciesMHz = frequenciesMHz;
    
    if (isEngaged()) {
        m_activeFrequencies = frequenciesMHz;
        sendJamCommand(true, m_activeFrequencies, m_currentPowerW);
        emit frequencyChanged(frequenciesMHz);
    }
}

void RFJammer::setPower(double watts) {
    watts = qBound(0.0, watts, m_config.maxPowerW);
    m_currentPowerW = watts;
    
    if (isEngaged()) {
        sendJamCommand(true, m_activeFrequencies, m_currentPowerW);
        emit jamming(true, m_currentPowerW);
    }
}

void RFJammer::setEngagementTime(int ms) {
    m_config.engagementTimeMs = ms;
}

int RFJammer::remainingEngagementTimeMs() const {
    if (!isEngaged() || m_engagementStartTime == 0) return 0;
    
    qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - m_engagementStartTime;
    return qMax(0LL, static_cast<qint64>(m_config.engagementTimeMs) - elapsed);
}

void RFJammer::onEngagementTimeout() {
    Logger::instance().info("RFJammer", m_effectorId + " engagement timeout");
    disengage();
}

void RFJammer::onCooldownComplete() {
    Logger::instance().info("RFJammer", m_effectorId + " cooldown complete");
    setStatus(EffectorStatus::Ready);
}

void RFJammer::onSocketConnected() {
    Logger::instance().info("RFJammer",
                           m_effectorId + " connected to jammer controller");
}

void RFJammer::onSocketDisconnected() {
    Logger::instance().warning("RFJammer",
                              m_effectorId + " disconnected from jammer controller");
}

void RFJammer::onSocketError(QAbstractSocket::SocketError error) {
    Q_UNUSED(error)
    Logger::instance().warning("RFJammer",
                              m_effectorId + " socket error: " + m_socket->errorString());
}

void RFJammer::sendJamCommand(bool engage, const QList<double>& frequencies, double power) {
    if (m_socket->state() != QAbstractSocket::ConnectedState) return;
    
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    
    stream << static_cast<quint8>(engage ? 0x01 : 0x00);
    stream << static_cast<quint8>(frequencies.size());
    
    for (double freq : frequencies) {
        stream << static_cast<float>(freq);
    }
    
    stream << static_cast<float>(power);
    
    m_socket->write(data);
}

bool RFJammer::connectToJammer() {
    m_socket->connectToHost(m_config.host, m_config.port);
    return m_socket->waitForConnected(3000);
}

void RFJammer::disconnectFromJammer() {
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
    }
}

} // namespace CounterUAS
