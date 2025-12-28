#ifndef CAMERACONFIG_H
#define CAMERACONFIG_H

#include <QObject>
#include <QList>
#include <QJsonObject>
#include "video/VideoStreamManager.h"

namespace CounterUAS {

class CameraConfig : public QObject {
    Q_OBJECT
    
public:
    explicit CameraConfig(QObject* parent = nullptr);
    
    bool loadFromFile(const QString& path);
    bool saveToFile(const QString& path);
    
    QList<CameraDefinition> cameras() const { return m_cameras; }
    CameraDefinition camera(const QString& cameraId) const;
    
    void addCamera(const CameraDefinition& camera);
    void updateCamera(const CameraDefinition& camera);
    void removeCamera(const QString& cameraId);
    
    static CameraDefinition fromJson(const QJsonObject& json);
    static QJsonObject toJson(const CameraDefinition& camera);
    
signals:
    void camerasChanged();
    
private:
    QList<CameraDefinition> m_cameras;
};

} // namespace CounterUAS

#endif // CAMERACONFIG_H
