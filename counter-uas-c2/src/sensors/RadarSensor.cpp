#include "sensors/RadarSensor.h"
#include "utils/Logger.h"
#include "utils/CoordinateUtils.h"
#include <QtMath>

namespace CounterUAS {

// RadarTrackReport implementation
GeoPosition RadarTrackReport::toGeoPosition(const GeoPosition& radarPos) const {
    // Convert radar spherical coordinates to geographic
    double azRad = qDegreesToRadians(azimuthDeg);
    double elRad = qDegreesToRadians(elevationDeg);
    
    // Calculate horizontal range and altitude
    double horizontalRange = rangeM * std::cos(elRad);
    double altitude = rangeM * std::sin(elRad);
    
    // Calculate offset in meters
    double northOffset = horizontalRange * std::cos(azRad);
    double eastOffset = horizontalRange * std::sin(azRad);
    
    // Convert to lat/lon
    GeoPosition result;
    result.latitude = radarPos.latitude + (northOffset / 111000.0);
    result.longitude = radarPos.longitude + (eastOffset / (111000.0 * std::cos(qDegreesToRadians(radarPos.latitude))));
    result.altitude = radarPos.altitude + altitude;
    
    return result;
}

VelocityVector RadarTrackReport::toVelocityVector(const GeoPosition& radarPos) const {
    Q_UNUSED(radarPos)
    
    double azRad = qDegreesToRadians(azimuthDeg);
    double elRad = qDegreesToRadians(elevationDeg);
    
    VelocityVector vel;
    vel.north = rangeRateMps * std::cos(azRad) * std::cos(elRad);
    vel.east = rangeRateMps * std::sin(azRad) * std::cos(elRad);
    vel.down = -rangeRateMps * std::sin(elRad);
    
    return vel;
}

// RadarSensor implementation
RadarSensor::RadarSensor(const QString& sensorId, QObject* parent)
    : SensorInterface(sensorId, parent)
    , m_socket(new QTcpSocket(this))
    , m_reconnectTimer(new QTimer(this))
{
    QObject::connect(m_socket, &QTcpSocket::connected, this, &RadarSensor::onConnected);
    QObject::connect(m_socket, &QTcpSocket::disconnected, this, &RadarSensor::onDisconnected);
    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &RadarSensor::onReadyRead);
    QObject::connect(m_socket, &QAbstractSocket::errorOccurred, this, &RadarSensor::onError);
    
    m_reconnectTimer->setSingleShot(true);
    QObject::connect(m_reconnectTimer, &QTimer::timeout, this, &RadarSensor::attemptReconnect);
}

RadarSensor::~RadarSensor() {
    disconnect();
}

void RadarSensor::setConfig(const RadarConfig& config) {
    m_config = config;
}

bool RadarSensor::connect() {
    if (isConnected()) return true;
    
    setStatus(SensorStatus::Initializing);
    
    m_socket->connectToHost(m_config.host, m_config.port);
    
    if (!m_socket->waitForConnected(m_config.timeoutMs)) {
        reportError("Connection timeout: " + m_socket->errorString());
        m_reconnectTimer->start(m_config.reconnectIntervalMs);
        return false;
    }
    
    return true;
}

void RadarSensor::disconnect() {
    m_reconnectTimer->stop();
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected(1000);
        }
    }
    setStatus(SensorStatus::Offline);
}

bool RadarSensor::isConnected() const {
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void RadarSensor::sendCommand(RadarMessageType type, const QByteArray& data) {
    if (!isConnected()) return;
    
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    
    stream << HEADER_MAGIC;
    stream << static_cast<quint8>(type);
    stream << static_cast<quint32>(data.size());
    
    // Calculate checksum
    quint16 checksum = 0;
    for (char c : data) {
        checksum += static_cast<quint8>(c);
    }
    stream << checksum;
    stream << static_cast<quint8>(0);  // Reserved
    
    message.append(data);
    
    m_socket->write(message);
}

void RadarSensor::requestStatus() {
    sendCommand(RadarMessageType::StatusReport);
}

void RadarSensor::setOperationalMode(int mode) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << static_cast<quint8>(mode);
    sendCommand(RadarMessageType::Command, data);
}

void RadarSensor::processData() {
    // Request heartbeat to keep connection alive
    if (isConnected()) {
        sendCommand(RadarMessageType::Heartbeat);
    }
}

void RadarSensor::onConnected() {
    setStatus(SensorStatus::Online);
    m_health.connectionRetries = 0;
    
    Logger::instance().info("RadarSensor", 
                           QString("%1 connected to %2:%3")
                               .arg(m_sensorId)
                               .arg(m_config.host)
                               .arg(m_config.port));
    
    emit connectedChanged(true);
    
    // Request initial status
    requestStatus();
}

void RadarSensor::onDisconnected() {
    setStatus(SensorStatus::Offline);
    
    Logger::instance().warning("RadarSensor", m_sensorId + " disconnected");
    
    emit connectedChanged(false);
    
    // Attempt reconnection if we were running
    if (m_running) {
        m_reconnectTimer->start(m_config.reconnectIntervalMs);
    }
}

void RadarSensor::onReadyRead() {
    m_buffer.append(m_socket->readAll());
    
    // Process complete messages
    while (m_buffer.size() >= HEADER_SIZE) {
        QDataStream stream(m_buffer);
        stream.setByteOrder(QDataStream::BigEndian);
        
        quint32 magic;
        stream >> magic;
        
        if (magic != HEADER_MAGIC) {
            // Sync error - search for magic
            int idx = m_buffer.indexOf(QByteArray::fromRawData(
                reinterpret_cast<const char*>(&HEADER_MAGIC), 4));
            if (idx > 0) {
                m_buffer.remove(0, idx);
                m_health.droppedPackets++;
            } else {
                m_buffer.clear();
            }
            continue;
        }
        
        quint8 type;
        quint32 length;
        stream >> type >> length;
        
        if (m_buffer.size() < HEADER_SIZE + static_cast<int>(length)) {
            break;  // Wait for more data
        }
        
        // Extract message
        QByteArray message = m_buffer.mid(HEADER_SIZE, length);
        m_buffer.remove(0, HEADER_SIZE + length);
        
        parseMessage(message);
    }
}

void RadarSensor::onError(QAbstractSocket::SocketError error) {
    Q_UNUSED(error)
    reportError("Socket error: " + m_socket->errorString());
    
    m_health.connectionRetries++;
    if (m_running) {
        m_reconnectTimer->start(m_config.reconnectIntervalMs);
    }
}

void RadarSensor::attemptReconnect() {
    if (m_running && !isConnected()) {
        Logger::instance().info("RadarSensor", 
                               m_sensorId + " attempting reconnection...");
        connect();
    }
}

void RadarSensor::parseMessage(const QByteArray& data) {
    if (data.isEmpty()) return;
    
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);
    
    quint8 type = data[0];
    
    QByteArray payload = data.mid(1);
    QDataStream payloadStream(payload);
    payloadStream.setByteOrder(QDataStream::BigEndian);
    
    switch (static_cast<RadarMessageType>(type)) {
        case RadarMessageType::TrackReport:
            parseTrackReport(payloadStream);
            break;
        case RadarMessageType::StatusReport:
            parseStatusReport(payloadStream);
            break;
        case RadarMessageType::Heartbeat:
        case RadarMessageType::Ack:
            // Just acknowledge receipt
            break;
        default:
            Logger::instance().warning("RadarSensor", 
                                      QString("Unknown message type: 0x%1")
                                          .arg(type, 2, 16, QChar('0')));
            break;
    }
}

void RadarSensor::parseTrackReport(QDataStream& stream) {
    RadarTrackReport report;
    
    stream >> report.trackNumber;
    stream >> report.rangeM;
    stream >> report.azimuthDeg;
    stream >> report.elevationDeg;
    stream >> report.rangeRateMps;
    stream >> report.azimuthRateDps;
    stream >> report.elevationRateDps;
    stream >> report.rcs;
    stream >> report.quality;
    stream >> report.timestamp;
    
    // Apply range filter
    if (report.rangeM < m_config.minRangeM || report.rangeM > m_config.maxRangeM) {
        return;
    }
    
    // Apply clutter filter
    if (m_config.filterClutter && report.rcs < m_config.clutterThreshold) {
        return;
    }
    
    // Convert to detection
    SensorDetection sensorDet;
    sensorDet.sensorId = m_sensorId;
    sensorDet.position = report.toGeoPosition(m_position);
    sensorDet.velocity = report.toVelocityVector(m_position);
    sensorDet.signalStrength = report.rcs;
    sensorDet.confidence = report.quality / 100.0;
    sensorDet.timestamp = report.timestamp;
    sensorDet.sourceType = DetectionSource::Radar;
    sensorDet.metadata["trackNumber"] = report.trackNumber;
    sensorDet.metadata["rangeM"] = report.rangeM;
    sensorDet.metadata["azimuthDeg"] = report.azimuthDeg;
    sensorDet.metadata["elevationDeg"] = report.elevationDeg;
    
    recordDetection();
    
    emit trackReportReceived(report);
    emit detection(sensorDet);
}

void RadarSensor::parseStatusReport(QDataStream& stream) {
    QByteArray status;
    stream >> status;
    emit statusReceived(status);
}

} // namespace CounterUAS
