#include "network/MessageProtocol.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QDateTime>

namespace CounterUAS {

quint32 MessageProtocol::s_sequenceCounter = 0;

MessageProtocol::MessageProtocol() {}

QJsonObject Message::toJson() const {
    QJsonObject obj;
    obj["type"] = static_cast<int>(type);
    obj["sequenceNumber"] = static_cast<qint64>(sequenceNumber);
    obj["timestamp"] = timestamp;
    obj["sourceId"] = sourceId;
    obj["payload"] = QJsonObject::fromVariantMap(payload);
    return obj;
}

Message Message::fromJson(const QJsonObject& json) {
    Message msg;
    msg.type = static_cast<MessageType>(json["type"].toInt());
    msg.sequenceNumber = static_cast<quint32>(json["sequenceNumber"].toDouble());
    msg.timestamp = static_cast<qint64>(json["timestamp"].toDouble());
    msg.sourceId = json["sourceId"].toString();
    msg.payload = json["payload"].toObject().toVariantMap();
    return msg;
}

QByteArray MessageProtocol::serialize(const Message& message) const {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    
    // Serialize payload to JSON
    QJsonDocument doc(message.toJson());
    QByteArray payload = doc.toJson(QJsonDocument::Compact);
    
    // Compress if enabled
    if (m_compression && payload.size() > 256) {
        payload = qCompress(payload);
    }
    
    // Write header
    stream << MAGIC;
    stream << static_cast<quint16>(message.type);
    stream << message.sequenceNumber;
    stream << message.timestamp;
    stream << static_cast<quint32>(payload.size());
    
    data.append(payload);
    
    return data;
}

int MessageProtocol::deserialize(const QByteArray& data, Message& message) const {
    if (data.size() < HEADER_SIZE) {
        return 0;  // Need more data
    }
    
    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian);
    
    quint32 magic;
    stream >> magic;
    
    if (magic != MAGIC) {
        return -1;  // Invalid magic
    }
    
    quint16 type;
    quint32 seq;
    qint64 timestamp;
    quint32 payloadSize;
    
    stream >> type >> seq >> timestamp >> payloadSize;
    
    if (data.size() < HEADER_SIZE + static_cast<int>(payloadSize)) {
        return 0;  // Need more data
    }
    
    QByteArray payload = data.mid(HEADER_SIZE, payloadSize);
    
    // Decompress if needed
    if (payload.size() > 0 && static_cast<quint8>(payload[0]) == 0x78) {
        payload = qUncompress(payload);
    }
    
    // Parse JSON
    QJsonDocument doc = QJsonDocument::fromJson(payload);
    if (doc.isNull()) {
        return -1;  // Invalid JSON
    }
    
    message = Message::fromJson(doc.object());
    
    return HEADER_SIZE + payloadSize;
}

Message MessageProtocol::createHeartbeat(const QString& sourceId) {
    Message msg;
    msg.type = MessageType::Heartbeat;
    msg.sequenceNumber = ++s_sequenceCounter;
    msg.timestamp = QDateTime::currentMSecsSinceEpoch();
    msg.sourceId = sourceId;
    return msg;
}

Message MessageProtocol::createTrackUpdate(const QString& trackId, const QVariantMap& data) {
    Message msg;
    msg.type = MessageType::TrackUpdate;
    msg.sequenceNumber = ++s_sequenceCounter;
    msg.timestamp = QDateTime::currentMSecsSinceEpoch();
    msg.payload = data;
    msg.payload["trackId"] = trackId;
    return msg;
}

Message MessageProtocol::createSensorDetection(const QString& sensorId, const QVariantMap& data) {
    Message msg;
    msg.type = MessageType::SensorDetection;
    msg.sequenceNumber = ++s_sequenceCounter;
    msg.timestamp = QDateTime::currentMSecsSinceEpoch();
    msg.sourceId = sensorId;
    msg.payload = data;
    return msg;
}

Message MessageProtocol::createEffectorCommand(const QString& effectorId, const QString& command,
                                                const QVariantMap& params) {
    Message msg;
    msg.type = MessageType::EffectorCommand;
    msg.sequenceNumber = ++s_sequenceCounter;
    msg.timestamp = QDateTime::currentMSecsSinceEpoch();
    msg.payload = params;
    msg.payload["effectorId"] = effectorId;
    msg.payload["command"] = command;
    return msg;
}

Message MessageProtocol::createAlert(const QString& alertId, int level, const QString& message) {
    Message msg;
    msg.type = MessageType::Alert;
    msg.sequenceNumber = ++s_sequenceCounter;
    msg.timestamp = QDateTime::currentMSecsSinceEpoch();
    msg.payload["alertId"] = alertId;
    msg.payload["level"] = level;
    msg.payload["message"] = message;
    return msg;
}

} // namespace CounterUAS
