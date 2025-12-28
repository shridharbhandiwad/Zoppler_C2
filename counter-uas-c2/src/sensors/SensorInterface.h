#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QVariant>
#include "core/Track.h"

namespace CounterUAS {

/**
 * @brief Sensor status enum
 */
enum class SensorStatus {
    Unknown = 0,
    Initializing,
    Online,
    Degraded,
    Offline,
    Error,
    Maintenance
};

/**
 * @brief Sensor health data
 */
struct SensorHealth {
    SensorStatus status = SensorStatus::Unknown;
    double signalQuality = 0.0;  // 0.0 - 1.0
    int detectionCount = 0;
    qint64 lastDetectionTime = 0;
    QString errorMessage;
    double cpuUsage = 0.0;
    double memoryUsage = 0.0;
    int droppedPackets = 0;
    int connectionRetries = 0;
};

/**
 * @brief Sensor detection result
 */
struct SensorDetection {
    QString sensorId;
    GeoPosition position;
    VelocityVector velocity;
    double signalStrength = 0.0;
    double confidence = 0.0;
    qint64 timestamp = 0;
    DetectionSource sourceType;
    QVariantMap metadata;
};

/**
 * @brief Abstract base class for sensor interfaces
 */
class SensorInterface : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString sensorId READ sensorId CONSTANT)
    Q_PROPERTY(QString sensorType READ sensorType CONSTANT)
    Q_PROPERTY(SensorStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    
public:
    explicit SensorInterface(const QString& sensorId, QObject* parent = nullptr);
    ~SensorInterface() override = default;
    
    // Identity
    QString sensorId() const { return m_sensorId; }
    virtual QString sensorType() const = 0;
    virtual DetectionSource detectionSource() const = 0;
    
    // Connection
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;
    
    // Status
    SensorStatus status() const { return m_health.status; }
    SensorHealth health() const { return m_health; }
    
    // Configuration
    void setUpdateRate(int hz);
    int updateRate() const { return m_updateRateHz; }
    
    void setPosition(const GeoPosition& pos) { m_position = pos; }
    GeoPosition position() const { return m_position; }
    
    void setName(const QString& name) { m_name = name; }
    QString name() const { return m_name; }
    
    // Control
    virtual void start();
    virtual void stop();
    bool isRunning() const { return m_running; }
    
    // Coverage
    virtual double maxRange() const { return 5000.0; }  // meters
    virtual double fieldOfView() const { return 360.0; } // degrees
    virtual double azimuth() const { return 0.0; }       // pointing direction
    
signals:
    void detection(const SensorDetection& detection);
    void statusChanged(SensorStatus status);
    void connectedChanged(bool connected);
    void healthUpdated(const SensorHealth& health);
    void error(const QString& message);
    
protected slots:
    virtual void processData() = 0;
    void updateHealth();
    
protected:
    void setStatus(SensorStatus status);
    void reportError(const QString& message);
    void recordDetection();
    
    QString m_sensorId;
    QString m_name;
    GeoPosition m_position;
    SensorHealth m_health;
    
    int m_updateRateHz = 10;
    QTimer* m_updateTimer;
    QTimer* m_healthTimer;
    bool m_running = false;
};

} // namespace CounterUAS

Q_DECLARE_METATYPE(CounterUAS::SensorStatus)
Q_DECLARE_METATYPE(CounterUAS::SensorDetection)

#endif // SENSORINTERFACE_H
