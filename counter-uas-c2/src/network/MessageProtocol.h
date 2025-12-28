#ifndef MESSAGEPROTOCOL_H
#define MESSAGEPROTOCOL_H

#include <QByteArray>
#include <QVariantMap>
#include <QString>
#include <QJsonObject>

namespace CounterUAS {

/**
 * @brief Message types
 */
enum class MessageType : quint16 {
    Heartbeat = 0x0001,
    
    // Track messages
    TrackUpdate = 0x0100,
    TrackCreate = 0x0101,
    TrackDelete = 0x0102,
    TrackClassify = 0x0103,
    
    // Sensor messages
    SensorStatus = 0x0200,
    SensorDetection = 0x0201,
    SensorConfig = 0x0202,
    
    // Effector messages
    EffectorStatus = 0x0300,
    EffectorCommand = 0x0301,
    EffectorResult = 0x0302,
    
    // Video messages
    VideoFrame = 0x0400,
    VideoConfig = 0x0401,
    PTZCommand = 0x0402,
    
    // System messages
    Alert = 0x0500,
    Config = 0x0501,
    Log = 0x0502,
    
    // Command messages
    EngagementRequest = 0x0600,
    EngagementAuthorize = 0x0601,
    EngagementAbort = 0x0602,
    
    Unknown = 0xFFFF
};

/**
 * @brief Message structure
 */
struct Message {
    MessageType type = MessageType::Unknown;
    quint32 sequenceNumber = 0;
    qint64 timestamp = 0;
    QString sourceId;
    QVariantMap payload;
    
    QJsonObject toJson() const;
    static Message fromJson(const QJsonObject& json);
};

/**
 * @brief Message protocol for serialization/deserialization
 */
class MessageProtocol {
public:
    MessageProtocol();
    
    // Serialization
    QByteArray serialize(const Message& message) const;
    int deserialize(const QByteArray& data, Message& message) const;
    
    // Convenience methods
    static Message createHeartbeat(const QString& sourceId);
    static Message createTrackUpdate(const QString& trackId, const QVariantMap& data);
    static Message createSensorDetection(const QString& sensorId, const QVariantMap& data);
    static Message createEffectorCommand(const QString& effectorId, const QString& command, 
                                          const QVariantMap& params);
    static Message createAlert(const QString& alertId, int level, const QString& message);
    
    // Protocol settings
    void setCompression(bool enable) { m_compression = enable; }
    bool compression() const { return m_compression; }
    
private:
    static quint32 s_sequenceCounter;
    bool m_compression = false;
    
    static constexpr quint32 MAGIC = 0x43554153;  // "CUAS"
    static constexpr int HEADER_SIZE = 20;  // magic(4) + type(2) + seq(4) + timestamp(8) + length(4) - 2
};

} // namespace CounterUAS

#endif // MESSAGEPROTOCOL_H
