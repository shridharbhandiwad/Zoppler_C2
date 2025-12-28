#ifndef RADARSENSOR_H
#define RADARSENSOR_H

#include "sensors/SensorInterface.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>

namespace CounterUAS {

/**
 * @brief Radar protocol message types
 */
enum class RadarMessageType : quint8 {
    Heartbeat = 0x01,
    TrackReport = 0x02,
    StatusReport = 0x03,
    Configuration = 0x04,
    Command = 0x05,
    Ack = 0x06
};

/**
 * @brief Radar track report structure
 */
struct RadarTrackReport {
    quint32 trackNumber;
    float rangeM;
    float azimuthDeg;
    float elevationDeg;
    float rangeRateMps;
    float azimuthRateDps;
    float elevationRateDps;
    float rcs;  // Radar cross section
    quint8 quality;
    quint64 timestamp;
    
    GeoPosition toGeoPosition(const GeoPosition& radarPos) const;
    VelocityVector toVelocityVector(const GeoPosition& radarPos) const;
};

/**
 * @brief Radar configuration
 */
struct RadarConfig {
    QString host = "127.0.0.1";
    quint16 port = 5001;
    int reconnectIntervalMs = 5000;
    int timeoutMs = 3000;
    double minRangeM = 50.0;
    double maxRangeM = 5000.0;
    double minAzimuthDeg = 0.0;
    double maxAzimuthDeg = 360.0;
    double minElevationDeg = -10.0;
    double maxElevationDeg = 90.0;
    bool filterClutter = true;
    double clutterThreshold = 0.1;
};

/**
 * @brief Radar sensor implementation using TCP/IP socket
 */
class RadarSensor : public SensorInterface {
    Q_OBJECT
    
public:
    explicit RadarSensor(const QString& sensorId, QObject* parent = nullptr);
    ~RadarSensor() override;
    
    // SensorInterface implementation
    QString sensorType() const override { return "RADAR"; }
    DetectionSource detectionSource() const override { return DetectionSource::Radar; }
    
    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    
    double maxRange() const override { return m_config.maxRangeM; }
    double fieldOfView() const override { 
        return m_config.maxAzimuthDeg - m_config.minAzimuthDeg; 
    }
    
    // Configuration
    void setConfig(const RadarConfig& config);
    RadarConfig config() const { return m_config; }
    
    // Commands
    void sendCommand(RadarMessageType type, const QByteArray& data = QByteArray());
    void requestStatus();
    void setOperationalMode(int mode);
    
signals:
    void trackReportReceived(const RadarTrackReport& report);
    void statusReceived(const QByteArray& status);
    
protected slots:
    void processData() override;
    
private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);
    void attemptReconnect();
    
private:
    void parseMessage(const QByteArray& data);
    void parseTrackReport(QDataStream& stream);
    void parseStatusReport(QDataStream& stream);
    
    QTcpSocket* m_socket;
    RadarConfig m_config;
    QTimer* m_reconnectTimer;
    QByteArray m_buffer;
    
    static constexpr quint32 HEADER_MAGIC = 0x52414452;  // "RADR"
    static constexpr int HEADER_SIZE = 12;  // magic(4) + type(1) + length(4) + checksum(2) + reserved(1)
};

} // namespace CounterUAS

#endif // RADARSENSOR_H
