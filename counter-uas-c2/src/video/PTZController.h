#ifndef PTZCONTROLLER_H
#define PTZCONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

namespace CounterUAS {

/**
 * @brief PTZ protocol types
 */
enum class PTZProtocol {
    ONVIF = 0,
    Pelco_D,
    Pelco_P,
    VISCA,
    HTTP_CGI,
    Custom
};

/**
 * @brief PTZ controller configuration
 */
struct PTZConfig {
    PTZProtocol protocol = PTZProtocol::ONVIF;
    QString host;
    int port = 80;
    QString username;
    QString password;
    int cameraAddress = 1;  // For Pelco protocols
    
    double panSpeed = 50.0;   // degrees/second
    double tiltSpeed = 30.0;  // degrees/second
    double zoomSpeed = 5.0;   // zoom levels/second
};

/**
 * @brief PTZ controller for camera pan/tilt/zoom operations
 */
class PTZController : public QObject {
    Q_OBJECT
    
public:
    explicit PTZController(QObject* parent = nullptr);
    ~PTZController() override;
    
    // Configuration
    void setConfig(const PTZConfig& config);
    PTZConfig config() const { return m_config; }
    
    // Connection
    bool connect();
    void disconnect();
    bool isConnected() const { return m_connected; }
    
    // Absolute positioning
    void setPan(double degrees);
    void setTilt(double degrees);
    void setZoom(double level);
    void setPTZ(double pan, double tilt, double zoom);
    
    // Relative movement
    void panLeft(double speed = 0.5);
    void panRight(double speed = 0.5);
    void tiltUp(double speed = 0.5);
    void tiltDown(double speed = 0.5);
    void zoomIn(double speed = 0.5);
    void zoomOut(double speed = 0.5);
    void stop();
    
    // Presets
    void goToPreset(int presetNumber);
    void setPreset(int presetNumber);
    void clearPreset(int presetNumber);
    QList<int> availablePresets() const { return m_presets; }
    
    // Home position
    void goHome();
    void setHome();
    
    // Query
    void queryPosition();
    double currentPan() const { return m_currentPan; }
    double currentTilt() const { return m_currentTilt; }
    double currentZoom() const { return m_currentZoom; }
    
signals:
    void connected();
    void disconnected();
    void positionChanged(double pan, double tilt, double zoom);
    void movementComplete();
    void error(const QString& message);
    
private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void updatePosition();
    
private:
    void sendCommand(const QByteArray& command);
    QByteArray buildPelcoCommand(quint8 cmd1, quint8 cmd2, quint8 data1, quint8 data2);
    QByteArray buildONVIFRequest(const QString& action, const QString& body);
    void parseONVIFResponse(const QByteArray& response);
    
    PTZConfig m_config;
    QTcpSocket* m_socket;
    QTimer* m_positionTimer;
    
    bool m_connected = false;
    double m_currentPan = 0.0;
    double m_currentTilt = 0.0;
    double m_currentZoom = 1.0;
    
    double m_targetPan = 0.0;
    double m_targetTilt = 0.0;
    double m_targetZoom = 1.0;
    bool m_moving = false;
    
    QList<int> m_presets;
};

} // namespace CounterUAS

#endif // PTZCONTROLLER_H
