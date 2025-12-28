#include "video/PTZController.h"
#include "utils/Logger.h"

namespace CounterUAS {

PTZController::PTZController(QObject* parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_positionTimer(new QTimer(this))
{
    connect(m_socket, &QTcpSocket::connected, this, &PTZController::onSocketConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &PTZController::onSocketDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &PTZController::onSocketReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &PTZController::onSocketError);
    
    m_positionTimer->setInterval(100);
    connect(m_positionTimer, &QTimer::timeout, this, &PTZController::updatePosition);
}

PTZController::~PTZController() {
    disconnect();
}

void PTZController::setConfig(const PTZConfig& config) {
    m_config = config;
}

bool PTZController::connect() {
    if (m_connected) return true;
    
    m_socket->connectToHost(m_config.host, m_config.port);
    return m_socket->waitForConnected(5000);
}

void PTZController::disconnect() {
    m_positionTimer->stop();
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
    }
    m_connected = false;
}

void PTZController::setPan(double degrees) {
    m_targetPan = degrees;
    m_moving = true;
    m_positionTimer->start();
    
    // Send actual command based on protocol
    switch (m_config.protocol) {
        case PTZProtocol::Pelco_D:
            sendCommand(buildPelcoCommand(0x00, 0x4B, 
                static_cast<quint8>((static_cast<int>(degrees) >> 8) & 0xFF),
                static_cast<quint8>(static_cast<int>(degrees) & 0xFF)));
            break;
        case PTZProtocol::ONVIF:
            sendCommand(buildONVIFRequest("AbsoluteMove", 
                QString("<Position><PanTilt x=\"%1\" y=\"%2\"/></Position>")
                    .arg(degrees / 180.0).arg(m_currentTilt / 90.0).toUtf8()));
            break;
        default:
            break;
    }
}

void PTZController::setTilt(double degrees) {
    m_targetTilt = degrees;
    m_moving = true;
    m_positionTimer->start();
    
    switch (m_config.protocol) {
        case PTZProtocol::Pelco_D:
            sendCommand(buildPelcoCommand(0x00, 0x4D,
                static_cast<quint8>((static_cast<int>(degrees) >> 8) & 0xFF),
                static_cast<quint8>(static_cast<int>(degrees) & 0xFF)));
            break;
        default:
            break;
    }
}

void PTZController::setZoom(double level) {
    m_targetZoom = level;
    m_moving = true;
    m_positionTimer->start();
}

void PTZController::setPTZ(double pan, double tilt, double zoom) {
    m_targetPan = pan;
    m_targetTilt = tilt;
    m_targetZoom = zoom;
    m_moving = true;
    m_positionTimer->start();
}

void PTZController::panLeft(double speed) {
    quint8 speedByte = static_cast<quint8>(speed * 0x3F);
    sendCommand(buildPelcoCommand(0x00, 0x04, speedByte, 0x00));
}

void PTZController::panRight(double speed) {
    quint8 speedByte = static_cast<quint8>(speed * 0x3F);
    sendCommand(buildPelcoCommand(0x00, 0x02, speedByte, 0x00));
}

void PTZController::tiltUp(double speed) {
    quint8 speedByte = static_cast<quint8>(speed * 0x3F);
    sendCommand(buildPelcoCommand(0x00, 0x08, 0x00, speedByte));
}

void PTZController::tiltDown(double speed) {
    quint8 speedByte = static_cast<quint8>(speed * 0x3F);
    sendCommand(buildPelcoCommand(0x00, 0x10, 0x00, speedByte));
}

void PTZController::zoomIn(double speed) {
    Q_UNUSED(speed)
    sendCommand(buildPelcoCommand(0x00, 0x20, 0x00, 0x00));
}

void PTZController::zoomOut(double speed) {
    Q_UNUSED(speed)
    sendCommand(buildPelcoCommand(0x00, 0x40, 0x00, 0x00));
}

void PTZController::stop() {
    sendCommand(buildPelcoCommand(0x00, 0x00, 0x00, 0x00));
    m_moving = false;
    m_positionTimer->stop();
}

void PTZController::goToPreset(int presetNumber) {
    sendCommand(buildPelcoCommand(0x00, 0x07, 0x00, static_cast<quint8>(presetNumber)));
}

void PTZController::setPreset(int presetNumber) {
    sendCommand(buildPelcoCommand(0x00, 0x03, 0x00, static_cast<quint8>(presetNumber)));
    if (!m_presets.contains(presetNumber)) {
        m_presets.append(presetNumber);
    }
}

void PTZController::clearPreset(int presetNumber) {
    sendCommand(buildPelcoCommand(0x00, 0x05, 0x00, static_cast<quint8>(presetNumber)));
    m_presets.removeAll(presetNumber);
}

void PTZController::goHome() {
    goToPreset(0);  // Preset 0 is typically home
}

void PTZController::setHome() {
    setPreset(0);
}

void PTZController::queryPosition() {
    sendCommand(buildPelcoCommand(0x00, 0x51, 0x00, 0x00));
}

void PTZController::onSocketConnected() {
    m_connected = true;
    Logger::instance().info("PTZController", "Connected to " + m_config.host);
    emit connected();
}

void PTZController::onSocketDisconnected() {
    m_connected = false;
    Logger::instance().info("PTZController", "Disconnected");
    emit disconnected();
}

void PTZController::onSocketReadyRead() {
    QByteArray data = m_socket->readAll();
    
    if (m_config.protocol == PTZProtocol::ONVIF) {
        parseONVIFResponse(data);
    }
    // Parse other protocol responses as needed
}

void PTZController::onSocketError(QAbstractSocket::SocketError error) {
    Q_UNUSED(error)
    emit this->error("Socket error: " + m_socket->errorString());
}

void PTZController::updatePosition() {
    // Simulate PTZ movement
    double dt = 0.1;  // 100ms timer
    
    double panDiff = m_targetPan - m_currentPan;
    double tiltDiff = m_targetTilt - m_currentTilt;
    double zoomDiff = m_targetZoom - m_currentZoom;
    
    double panStep = m_config.panSpeed * dt;
    double tiltStep = m_config.tiltSpeed * dt;
    double zoomStep = m_config.zoomSpeed * dt;
    
    bool changed = false;
    
    if (std::abs(panDiff) > panStep) {
        m_currentPan += (panDiff > 0 ? panStep : -panStep);
        changed = true;
    } else if (std::abs(panDiff) > 0.1) {
        m_currentPan = m_targetPan;
        changed = true;
    }
    
    if (std::abs(tiltDiff) > tiltStep) {
        m_currentTilt += (tiltDiff > 0 ? tiltStep : -tiltStep);
        changed = true;
    } else if (std::abs(tiltDiff) > 0.1) {
        m_currentTilt = m_targetTilt;
        changed = true;
    }
    
    if (std::abs(zoomDiff) > zoomStep) {
        m_currentZoom += (zoomDiff > 0 ? zoomStep : -zoomStep);
        changed = true;
    } else if (std::abs(zoomDiff) > 0.01) {
        m_currentZoom = m_targetZoom;
        changed = true;
    }
    
    if (changed) {
        emit positionChanged(m_currentPan, m_currentTilt, m_currentZoom);
    }
    
    // Check if movement complete
    if (std::abs(panDiff) <= 0.1 && 
        std::abs(tiltDiff) <= 0.1 && 
        std::abs(zoomDiff) <= 0.01) {
        m_moving = false;
        m_positionTimer->stop();
        emit movementComplete();
    }
}

void PTZController::sendCommand(const QByteArray& command) {
    if (!m_connected) return;
    m_socket->write(command);
}

QByteArray PTZController::buildPelcoCommand(quint8 cmd1, quint8 cmd2, quint8 data1, quint8 data2) {
    QByteArray cmd;
    cmd.append(static_cast<char>(0xFF));  // Sync
    cmd.append(static_cast<char>(m_config.cameraAddress));
    cmd.append(static_cast<char>(cmd1));
    cmd.append(static_cast<char>(cmd2));
    cmd.append(static_cast<char>(data1));
    cmd.append(static_cast<char>(data2));
    
    // Calculate checksum
    quint8 checksum = (m_config.cameraAddress + cmd1 + cmd2 + data1 + data2) & 0xFF;
    cmd.append(static_cast<char>(checksum));
    
    return cmd;
}

QByteArray PTZController::buildONVIFRequest(const QString& action, const QString& body) {
    // Simplified ONVIF SOAP request
    QString request = QString(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\">"
        "<s:Body><%1>%2</%1></s:Body>"
        "</s:Envelope>").arg(action, body);
    
    return request.toUtf8();
}

void PTZController::parseONVIFResponse(const QByteArray& response) {
    Q_UNUSED(response)
    // Parse ONVIF XML response
}

} // namespace CounterUAS
