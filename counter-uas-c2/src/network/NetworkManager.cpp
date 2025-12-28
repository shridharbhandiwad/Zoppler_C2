#include "network/NetworkManager.h"
#include "utils/Logger.h"
#include <QDateTime>

namespace CounterUAS {

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
    , m_reconnectTimer(new QTimer(this))
    , m_bandwidthTimer(new QTimer(this))
{
    m_reconnectTimer->setInterval(5000);
    connect(m_reconnectTimer, &QTimer::timeout, this, &NetworkManager::attemptReconnect);
    
    m_bandwidthTimer->setInterval(1000);
    connect(m_bandwidthTimer, &QTimer::timeout, this, &NetworkManager::updateBandwidth);
    m_bandwidthTimer->start();
}

NetworkManager::~NetworkManager() {
    disconnectAll();
}

QString NetworkManager::addConnection(const ConnectionConfig& config) {
    Connection conn;
    conn.config = config;
    conn.status = ConnectionStatus::Disconnected;
    
    if (config.useTcp) {
        conn.tcpSocket = new QTcpSocket(this);
        connect(conn.tcpSocket, &QTcpSocket::connected, this, &NetworkManager::onTcpConnected);
        connect(conn.tcpSocket, &QTcpSocket::disconnected, this, &NetworkManager::onTcpDisconnected);
        connect(conn.tcpSocket, &QTcpSocket::readyRead, this, &NetworkManager::onTcpReadyRead);
        connect(conn.tcpSocket, &QTcpSocket::errorOccurred, this, &NetworkManager::onTcpError);
    } else {
        conn.udpSocket = new QUdpSocket(this);
        connect(conn.udpSocket, &QUdpSocket::readyRead, this, &NetworkManager::onUdpReadyRead);
    }
    
    m_connections[config.connectionId] = conn;
    
    Logger::instance().info("NetworkManager", "Added connection: " + config.name);
    
    return config.connectionId;
}

void NetworkManager::removeConnection(const QString& connectionId) {
    if (!m_connections.contains(connectionId)) return;
    
    disconnect(connectionId);
    
    Connection& conn = m_connections[connectionId];
    if (conn.tcpSocket) {
        delete conn.tcpSocket;
    }
    if (conn.udpSocket) {
        delete conn.udpSocket;
    }
    
    m_connections.remove(connectionId);
    
    Logger::instance().info("NetworkManager", "Removed connection: " + connectionId);
}

void NetworkManager::connectTo(const QString& connectionId) {
    if (!m_connections.contains(connectionId)) return;
    
    Connection& conn = m_connections[connectionId];
    
    if (conn.status == ConnectionStatus::Connected) return;
    
    setConnectionStatus(connectionId, ConnectionStatus::Connecting);
    
    if (conn.tcpSocket) {
        conn.tcpSocket->setProperty("connectionId", connectionId);
        conn.tcpSocket->connectToHost(conn.config.host, conn.config.port);
    } else if (conn.udpSocket) {
        conn.udpSocket->setProperty("connectionId", connectionId);
        if (conn.udpSocket->bind(QHostAddress::Any, conn.config.port)) {
            setConnectionStatus(connectionId, ConnectionStatus::Connected);
        } else {
            setConnectionStatus(connectionId, ConnectionStatus::Error);
            emit connectionError(connectionId, "Failed to bind UDP socket");
        }
    }
}

void NetworkManager::disconnect(const QString& connectionId) {
    if (!m_connections.contains(connectionId)) return;
    
    Connection& conn = m_connections[connectionId];
    
    if (conn.tcpSocket && conn.tcpSocket->state() != QAbstractSocket::UnconnectedState) {
        conn.tcpSocket->disconnectFromHost();
    }
    if (conn.udpSocket) {
        conn.udpSocket->close();
    }
    
    setConnectionStatus(connectionId, ConnectionStatus::Disconnected);
}

void NetworkManager::disconnectAll() {
    for (const QString& id : m_connections.keys()) {
        disconnect(id);
    }
}

ConnectionStatus NetworkManager::connectionStatus(const QString& connectionId) const {
    if (!m_connections.contains(connectionId)) {
        return ConnectionStatus::Disconnected;
    }
    return m_connections[connectionId].status;
}

QList<QString> NetworkManager::connectionIds() const {
    return m_connections.keys();
}

bool NetworkManager::isConnected(const QString& connectionId) const {
    return connectionStatus(connectionId) == ConnectionStatus::Connected;
}

void NetworkManager::send(const QString& connectionId, const Message& message) {
    if (!m_connections.contains(connectionId)) return;
    
    Connection& conn = m_connections[connectionId];
    
    if (conn.status != ConnectionStatus::Connected) {
        Logger::instance().warning("NetworkManager",
                                  "Cannot send - not connected: " + connectionId);
        return;
    }
    
    QByteArray data = m_protocol.serialize(message);
    
    if (conn.tcpSocket) {
        conn.tcpSocket->write(data);
        conn.bandwidth.bytesSent += data.size();
    } else if (conn.udpSocket) {
        conn.udpSocket->writeDatagram(data, QHostAddress(conn.config.host), conn.config.port);
        conn.bandwidth.bytesSent += data.size();
    }
}

void NetworkManager::broadcast(const Message& message) {
    for (const QString& id : m_connections.keys()) {
        if (isConnected(id)) {
            send(id, message);
        }
    }
}

NetworkManager::BandwidthStats NetworkManager::bandwidth(const QString& connectionId) const {
    if (!m_connections.contains(connectionId)) {
        return BandwidthStats();
    }
    return m_connections[connectionId].bandwidth;
}

NetworkManager::BandwidthStats NetworkManager::totalBandwidth() const {
    BandwidthStats total;
    for (const Connection& conn : m_connections) {
        total.bytesSent += conn.bandwidth.bytesSent;
        total.bytesReceived += conn.bandwidth.bytesReceived;
        total.sendRateBps += conn.bandwidth.sendRateBps;
        total.receiveRateBps += conn.bandwidth.receiveRateBps;
    }
    return total;
}

void NetworkManager::onTcpConnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    
    QString connectionId = socket->property("connectionId").toString();
    setConnectionStatus(connectionId, ConnectionStatus::Connected);
    
    Logger::instance().info("NetworkManager",
                           "Connected: " + m_connections[connectionId].config.name);
}

void NetworkManager::onTcpDisconnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    
    QString connectionId = socket->property("connectionId").toString();
    
    if (m_connections.contains(connectionId) &&
        m_connections[connectionId].config.autoReconnect) {
        setConnectionStatus(connectionId, ConnectionStatus::Reconnecting);
        if (!m_reconnectTimer->isActive()) {
            m_reconnectTimer->start();
        }
    } else {
        setConnectionStatus(connectionId, ConnectionStatus::Disconnected);
    }
    
    Logger::instance().info("NetworkManager", "Disconnected: " + connectionId);
}

void NetworkManager::onTcpReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    
    QString connectionId = socket->property("connectionId").toString();
    QByteArray data = socket->readAll();
    
    if (m_connections.contains(connectionId)) {
        m_connections[connectionId].bandwidth.bytesReceived += data.size();
    }
    
    processReceivedData(connectionId, data);
}

void NetworkManager::onTcpError(QAbstractSocket::SocketError error) {
    Q_UNUSED(error)
    
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    
    QString connectionId = socket->property("connectionId").toString();
    setConnectionStatus(connectionId, ConnectionStatus::Error);
    
    emit connectionError(connectionId, socket->errorString());
}

void NetworkManager::onUdpReadyRead() {
    QUdpSocket* socket = qobject_cast<QUdpSocket*>(sender());
    if (!socket) return;
    
    QString connectionId = socket->property("connectionId").toString();
    
    while (socket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(), data.size());
        
        if (m_connections.contains(connectionId)) {
            m_connections[connectionId].bandwidth.bytesReceived += data.size();
        }
        
        processReceivedData(connectionId, data);
    }
}

void NetworkManager::attemptReconnect() {
    bool anyReconnecting = false;
    
    for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
        if (it.value().status == ConnectionStatus::Reconnecting) {
            anyReconnecting = true;
            connectTo(it.key());
        }
    }
    
    if (!anyReconnecting) {
        m_reconnectTimer->stop();
    }
}

void NetworkManager::updateBandwidth() {
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    
    for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
        Connection& conn = it.value();
        
        if (conn.lastBandwidthCheck > 0) {
            double elapsed = (now - conn.lastBandwidthCheck) / 1000.0;
            if (elapsed > 0) {
                qint64 bytesDelta = (conn.bandwidth.bytesSent + conn.bandwidth.bytesReceived) - 
                                    conn.bytesAtLastCheck;
                conn.bandwidth.sendRateBps = bytesDelta / elapsed;
                // Simplified - would track send/receive separately
            }
        }
        
        conn.lastBandwidthCheck = now;
        conn.bytesAtLastCheck = conn.bandwidth.bytesSent + conn.bandwidth.bytesReceived;
    }
    
    emit bandwidthUpdated(totalBandwidth());
}

void NetworkManager::setConnectionStatus(const QString& id, ConnectionStatus status) {
    if (!m_connections.contains(id)) return;
    
    if (m_connections[id].status != status) {
        m_connections[id].status = status;
        emit connectionStatusChanged(id, status);
    }
}

void NetworkManager::processReceivedData(const QString& id, const QByteArray& data) {
    if (!m_connections.contains(id)) return;
    
    Connection& conn = m_connections[id];
    conn.buffer.append(data);
    
    // Try to parse complete messages
    while (true) {
        Message msg;
        int bytesConsumed = m_protocol.deserialize(conn.buffer, msg);
        
        if (bytesConsumed > 0) {
            conn.buffer.remove(0, bytesConsumed);
            emit messageReceived(id, msg);
        } else {
            break;
        }
    }
}

} // namespace CounterUAS
