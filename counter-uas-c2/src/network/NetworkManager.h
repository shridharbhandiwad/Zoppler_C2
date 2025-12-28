#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QHash>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include "network/MessageProtocol.h"

namespace CounterUAS {

/**
 * @brief Connection status
 */
enum class ConnectionStatus {
    Disconnected = 0,
    Connecting,
    Connected,
    Reconnecting,
    Error
};

/**
 * @brief Connection definition
 */
struct ConnectionConfig {
    QString connectionId;
    QString name;
    QString host;
    int port;
    bool useTcp = true;
    bool autoReconnect = true;
    int reconnectIntervalMs = 5000;
    int timeoutMs = 3000;
    QString username;
    QString password;
};

/**
 * @brief Network manager for all communications
 */
class NetworkManager : public QObject {
    Q_OBJECT
    
public:
    explicit NetworkManager(QObject* parent = nullptr);
    ~NetworkManager() override;
    
    // Connection management
    QString addConnection(const ConnectionConfig& config);
    void removeConnection(const QString& connectionId);
    void connectTo(const QString& connectionId);
    void disconnect(const QString& connectionId);
    void disconnectAll();
    
    // Status
    ConnectionStatus connectionStatus(const QString& connectionId) const;
    QList<QString> connectionIds() const;
    bool isConnected(const QString& connectionId) const;
    
    // Send
    void send(const QString& connectionId, const Message& message);
    void broadcast(const Message& message);
    
    // Bandwidth monitoring
    struct BandwidthStats {
        qint64 bytesSent = 0;
        qint64 bytesReceived = 0;
        double sendRateBps = 0.0;
        double receiveRateBps = 0.0;
    };
    BandwidthStats bandwidth(const QString& connectionId) const;
    BandwidthStats totalBandwidth() const;
    
signals:
    void connectionStatusChanged(const QString& connectionId, ConnectionStatus status);
    void messageReceived(const QString& connectionId, const Message& message);
    void connectionError(const QString& connectionId, const QString& error);
    void bandwidthUpdated(const BandwidthStats& stats);
    
private slots:
    void onTcpConnected();
    void onTcpDisconnected();
    void onTcpReadyRead();
    void onTcpError(QAbstractSocket::SocketError error);
    void onUdpReadyRead();
    void attemptReconnect();
    void updateBandwidth();
    
private:
    struct Connection {
        ConnectionConfig config;
        QTcpSocket* tcpSocket = nullptr;
        QUdpSocket* udpSocket = nullptr;
        ConnectionStatus status = ConnectionStatus::Disconnected;
        QByteArray buffer;
        BandwidthStats bandwidth;
        qint64 lastBandwidthCheck = 0;
        qint64 bytesAtLastCheck = 0;
    };
    
    void setConnectionStatus(const QString& id, ConnectionStatus status);
    void processReceivedData(const QString& id, const QByteArray& data);
    
    QHash<QString, Connection> m_connections;
    QTimer* m_reconnectTimer;
    QTimer* m_bandwidthTimer;
    MessageProtocol m_protocol;
};

} // namespace CounterUAS

#endif // NETWORKMANAGER_H
