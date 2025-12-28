#ifndef RFDETECTOR_H
#define RFDETECTOR_H

#include "sensors/SensorInterface.h"
#include <QUdpSocket>
#include <QSerialPort>
#include <QHostAddress>

namespace CounterUAS {

/**
 * @brief RF detection result
 */
struct RFDetection {
    double frequencyMHz;
    double signalStrengthDbm;
    double azimuthDeg;
    double elevationDeg;
    double bandwidth;
    QString protocol;  // e.g., "DJI", "FrSky", "Generic 2.4GHz"
    quint64 timestamp;
    QByteArray signature;
};

/**
 * @brief RF Detector configuration
 */
struct RFDetectorConfig {
    enum class ConnectionType { UDP, Serial };
    
    ConnectionType connectionType = ConnectionType::UDP;
    
    // UDP settings
    QString udpHost = "127.0.0.1";
    quint16 udpPort = 5002;
    
    // Serial settings
    QString serialPort = "/dev/ttyUSB0";
    qint32 baudRate = 115200;
    
    // Detection parameters
    double minFrequencyMHz = 900.0;
    double maxFrequencyMHz = 6000.0;
    double signalThresholdDbm = -80.0;
    bool enableDirectionFinding = true;
    double dfAccuracyDeg = 5.0;
    
    // Antenna array position (for DF)
    double antennaSpacingM = 0.5;
    int antennaCount = 4;
};

/**
 * @brief RF Detector sensor for detecting drone control signals
 */
class RFDetector : public SensorInterface {
    Q_OBJECT
    
public:
    explicit RFDetector(const QString& sensorId, QObject* parent = nullptr);
    ~RFDetector() override;
    
    // SensorInterface implementation
    QString sensorType() const override { return "RF_DETECTOR"; }
    DetectionSource detectionSource() const override { return DetectionSource::RFDetector; }
    
    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    
    double maxRange() const override { return 3000.0; }  // RF detection range
    double fieldOfView() const override { return 360.0; }
    
    // Configuration
    void setConfig(const RFDetectorConfig& config);
    RFDetectorConfig config() const { return m_config; }
    
    // Protocol database
    void addKnownProtocol(const QString& name, const QByteArray& signature);
    QString identifyProtocol(const QByteArray& signature) const;
    
signals:
    void rfDetection(const RFDetection& detection);
    void protocolIdentified(const QString& trackId, const QString& protocol);
    
protected slots:
    void processData() override;
    
private slots:
    void onUdpReadyRead();
    void onSerialReadyRead();
    void onSerialError(QSerialPort::SerialPortError error);
    
private:
    void parseRFData(const QByteArray& data);
    GeoPosition estimatePosition(const RFDetection& detection);
    
    QUdpSocket* m_udpSocket;
    QSerialPort* m_serialPort;
    RFDetectorConfig m_config;
    
    QHash<QString, QByteArray> m_knownProtocols;
    QByteArray m_buffer;
};

} // namespace CounterUAS

#endif // RFDETECTOR_H
