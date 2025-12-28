#include "sensors/RFDetector.h"
#include "utils/Logger.h"
#include <QtMath>
#include <QDataStream>

namespace CounterUAS {

RFDetector::RFDetector(const QString& sensorId, QObject* parent)
    : SensorInterface(sensorId, parent)
    , m_udpSocket(new QUdpSocket(this))
    , m_serialPort(new QSerialPort(this))
{
    QObject::connect(m_udpSocket, &QUdpSocket::readyRead, 
            this, &RFDetector::onUdpReadyRead);
    QObject::connect(m_serialPort, &QSerialPort::readyRead,
            this, &RFDetector::onSerialReadyRead);
    QObject::connect(m_serialPort, &QSerialPort::errorOccurred,
            this, &RFDetector::onSerialError);
    
    // Add known drone protocols
    addKnownProtocol("DJI_OcuSync", QByteArray::fromHex("5aa5"));
    addKnownProtocol("DJI_Lightbridge", QByteArray::fromHex("55aa"));
    addKnownProtocol("FrSky", QByteArray::fromHex("7e"));
    addKnownProtocol("Futaba_FASST", QByteArray::fromHex("0f"));
    addKnownProtocol("Generic_2.4GHz", QByteArray());
    addKnownProtocol("Generic_5.8GHz", QByteArray());
}

RFDetector::~RFDetector() {
    disconnect();
}

void RFDetector::setConfig(const RFDetectorConfig& config) {
    m_config = config;
}

bool RFDetector::connect() {
    if (isConnected()) return true;
    
    setStatus(SensorStatus::Initializing);
    
    if (m_config.connectionType == RFDetectorConfig::ConnectionType::UDP) {
        if (!m_udpSocket->bind(QHostAddress(m_config.udpHost), m_config.udpPort)) {
            reportError("Failed to bind UDP socket: " + m_udpSocket->errorString());
            return false;
        }
        
        Logger::instance().info("RFDetector",
                               QString("%1 listening on UDP %2:%3")
                                   .arg(m_sensorId)
                                   .arg(m_config.udpHost)
                                   .arg(m_config.udpPort));
    } else {
        m_serialPort->setPortName(m_config.serialPort);
        m_serialPort->setBaudRate(m_config.baudRate);
        m_serialPort->setDataBits(QSerialPort::Data8);
        m_serialPort->setParity(QSerialPort::NoParity);
        m_serialPort->setStopBits(QSerialPort::OneStop);
        
        if (!m_serialPort->open(QIODevice::ReadOnly)) {
            reportError("Failed to open serial port: " + m_serialPort->errorString());
            return false;
        }
        
        Logger::instance().info("RFDetector",
                               QString("%1 connected to serial %2 @ %3 baud")
                                   .arg(m_sensorId)
                                   .arg(m_config.serialPort)
                                   .arg(m_config.baudRate));
    }
    
    setStatus(SensorStatus::Online);
    emit connectedChanged(true);
    return true;
}

void RFDetector::disconnect() {
    if (m_udpSocket->state() != QAbstractSocket::UnconnectedState) {
        m_udpSocket->close();
    }
    
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
    
    setStatus(SensorStatus::Offline);
    emit connectedChanged(false);
}

bool RFDetector::isConnected() const {
    if (m_config.connectionType == RFDetectorConfig::ConnectionType::UDP) {
        return m_udpSocket->state() == QAbstractSocket::BoundState;
    } else {
        return m_serialPort->isOpen();
    }
}

void RFDetector::addKnownProtocol(const QString& name, const QByteArray& signature) {
    m_knownProtocols[name] = signature;
}

QString RFDetector::identifyProtocol(const QByteArray& signature) const {
    for (auto it = m_knownProtocols.begin(); it != m_knownProtocols.end(); ++it) {
        if (!it.value().isEmpty() && signature.startsWith(it.value())) {
            return it.key();
        }
    }
    
    // Try to identify by frequency
    return "Unknown";
}

void RFDetector::processData() {
    // Periodic processing - handled by socket callbacks
}

void RFDetector::onUdpReadyRead() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        
        QHostAddress sender;
        quint16 senderPort;
        
        m_udpSocket->readDatagram(datagram.data(), datagram.size(),
                                  &sender, &senderPort);
        
        parseRFData(datagram);
    }
}

void RFDetector::onSerialReadyRead() {
    m_buffer.append(m_serialPort->readAll());
    
    // Look for complete messages (assuming newline-delimited for simplicity)
    while (m_buffer.contains('\n')) {
        int idx = m_buffer.indexOf('\n');
        QByteArray message = m_buffer.left(idx);
        m_buffer.remove(0, idx + 1);
        
        if (!message.isEmpty()) {
            parseRFData(message);
        }
    }
}

void RFDetector::onSerialError(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::NoError) {
        reportError("Serial port error: " + m_serialPort->errorString());
    }
}

void RFDetector::parseRFData(const QByteArray& data) {
    // Parse RF detection data
    // Expected format: binary structure or JSON
    
    if (data.size() < 32) return;  // Minimum message size
    
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    
    RFDetection detection;
    
    float freqMHz, signalDbm, azDeg, elDeg, bw;
    quint64 timestamp;
    quint16 sigLen;
    
    stream >> freqMHz >> signalDbm >> azDeg >> elDeg >> bw >> timestamp >> sigLen;
    
    detection.frequencyMHz = freqMHz;
    detection.signalStrengthDbm = signalDbm;
    detection.azimuthDeg = azDeg;
    detection.elevationDeg = elDeg;
    detection.bandwidth = bw;
    detection.timestamp = timestamp;
    
    if (sigLen > 0 && sigLen <= 256) {
        detection.signature.resize(sigLen);
        stream.readRawData(detection.signature.data(), sigLen);
    }
    
    // Apply filters
    if (detection.frequencyMHz < m_config.minFrequencyMHz ||
        detection.frequencyMHz > m_config.maxFrequencyMHz) {
        return;
    }
    
    if (detection.signalStrengthDbm < m_config.signalThresholdDbm) {
        return;
    }
    
    // Identify protocol
    detection.protocol = identifyProtocol(detection.signature);
    
    // Estimate position
    GeoPosition estimatedPos = estimatePosition(detection);
    
    // Create sensor detection
    SensorDetection sensorDetection;
    sensorDetection.sensorId = m_sensorId;
    sensorDetection.position = estimatedPos;
    sensorDetection.signalStrength = (detection.signalStrengthDbm + 100) / 100.0;  // Normalize
    sensorDetection.confidence = m_config.enableDirectionFinding ? 0.7 : 0.4;
    sensorDetection.timestamp = detection.timestamp;
    sensorDetection.sourceType = DetectionSource::RFDetector;
    sensorDetection.metadata["frequencyMHz"] = detection.frequencyMHz;
    sensorDetection.metadata["signalStrengthDbm"] = detection.signalStrengthDbm;
    sensorDetection.metadata["protocol"] = detection.protocol;
    sensorDetection.metadata["azimuthDeg"] = detection.azimuthDeg;
    
    recordDetection();
    
    emit rfDetection(detection);
    emit detection(sensorDetection);
}

GeoPosition RFDetector::estimatePosition(const RFDetection& detection) {
    // Estimate position based on signal strength and direction
    // This is a simplified model
    
    if (!m_config.enableDirectionFinding) {
        // Without DF, just return detector position with large uncertainty
        return m_position;
    }
    
    // Estimate range based on signal strength
    // Free space path loss model: FSPL = 20*log10(d) + 20*log10(f) + 20*log10(4*pi/c)
    // Simplified: assuming known TX power and antenna gains
    
    double txPowerDbm = 20.0;  // Typical drone TX power
    double pathLoss = txPowerDbm - detection.signalStrengthDbm;
    
    // Estimate range (very approximate)
    double estimatedRangeM = std::pow(10.0, (pathLoss - 32.44 - 20.0 * std::log10(detection.frequencyMHz)) / 20.0) * 1000.0;
    estimatedRangeM = qBound(100.0, estimatedRangeM, 3000.0);
    
    // Apply direction finding
    double azRad = qDegreesToRadians(detection.azimuthDeg);
    double elRad = qDegreesToRadians(detection.elevationDeg);
    
    double horizontalRange = estimatedRangeM * std::cos(elRad);
    
    double northOffset = horizontalRange * std::cos(azRad);
    double eastOffset = horizontalRange * std::sin(azRad);
    double altOffset = estimatedRangeM * std::sin(elRad);
    
    GeoPosition result;
    result.latitude = m_position.latitude + (northOffset / 111000.0);
    result.longitude = m_position.longitude + (eastOffset / (111000.0 * std::cos(qDegreesToRadians(m_position.latitude))));
    result.altitude = m_position.altitude + altOffset;
    
    return result;
}

} // namespace CounterUAS
