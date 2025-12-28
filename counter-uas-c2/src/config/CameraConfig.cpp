#include "config/CameraConfig.h"
#include "utils/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>

namespace CounterUAS {

CameraConfig::CameraConfig(QObject* parent) : QObject(parent) {}

bool CameraConfig::loadFromFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        Logger::instance().error("CameraConfig", "Failed to open: " + path);
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    
    m_cameras.clear();
    QJsonArray array = doc.object()["cameras"].toArray();
    for (const auto& item : array) {
        m_cameras.append(fromJson(item.toObject()));
    }
    
    Logger::instance().info("CameraConfig", QString("Loaded %1 cameras").arg(m_cameras.size()));
    emit camerasChanged();
    return true;
}

bool CameraConfig::saveToFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        Logger::instance().error("CameraConfig", "Failed to save: " + path);
        return false;
    }
    
    QJsonArray array;
    for (const auto& cam : m_cameras) {
        array.append(toJson(cam));
    }
    
    QJsonObject root;
    root["cameras"] = array;
    
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

CameraDefinition CameraConfig::camera(const QString& cameraId) const {
    for (const auto& cam : m_cameras) {
        if (cam.cameraId == cameraId) return cam;
    }
    return CameraDefinition();
}

void CameraConfig::addCamera(const CameraDefinition& camera) {
    m_cameras.append(camera);
    emit camerasChanged();
}

void CameraConfig::updateCamera(const CameraDefinition& camera) {
    for (auto& cam : m_cameras) {
        if (cam.cameraId == camera.cameraId) {
            cam = camera;
            emit camerasChanged();
            return;
        }
    }
}

void CameraConfig::removeCamera(const QString& cameraId) {
    m_cameras.erase(std::remove_if(m_cameras.begin(), m_cameras.end(),
        [&cameraId](const CameraDefinition& c) { return c.cameraId == cameraId; }), m_cameras.end());
    emit camerasChanged();
}

CameraDefinition CameraConfig::fromJson(const QJsonObject& json) {
    CameraDefinition cam;
    cam.cameraId = json["cameraId"].toString();
    cam.name = json["name"].toString();
    cam.streamUrl = json["streamUrl"].toString();
    cam.sourceType = json["sourceType"].toString("RTSP");
    cam.position = GeoPosition::fromJson(json["position"].toObject());
    cam.hasPTZ = json["hasPTZ"].toBool();
    cam.ptzProtocol = json["ptzProtocol"].toString();
    cam.ptzAddress = json["ptzAddress"].toString();
    cam.ptzPort = json["ptzPort"].toInt(80);
    return cam;
}

QJsonObject CameraConfig::toJson(const CameraDefinition& camera) {
    QJsonObject json;
    json["cameraId"] = camera.cameraId;
    json["name"] = camera.name;
    json["streamUrl"] = camera.streamUrl;
    json["sourceType"] = camera.sourceType;
    json["position"] = camera.position.toJson();
    json["hasPTZ"] = camera.hasPTZ;
    json["ptzProtocol"] = camera.ptzProtocol;
    json["ptzAddress"] = camera.ptzAddress;
    json["ptzPort"] = camera.ptzPort;
    return json;
}

} // namespace CounterUAS
