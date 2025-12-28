#ifndef CAMERASLEWCONTROLLER_H
#define CAMERASLEWCONTROLLER_H

#include <QObject>
#include "core/Track.h"
#include "video/PTZController.h"

namespace CounterUAS {

class TrackManager;
class VideoStreamManager;

/**
 * @brief Camera slew controller for automatic track following
 */
class CameraSlewController : public QObject {
    Q_OBJECT
    
public:
    explicit CameraSlewController(QObject* parent = nullptr);
    ~CameraSlewController() override = default;
    
    void setTrackManager(TrackManager* manager);
    void setVideoStreamManager(VideoStreamManager* manager);
    
    // Auto-slew to track
    void slewToTrack(const QString& cameraId, const QString& trackId);
    void startAutoTracking(const QString& cameraId, const QString& trackId);
    void stopAutoTracking(const QString& cameraId);
    
    // Slew to position
    void slewToPosition(const QString& cameraId, const GeoPosition& target);
    void slewNearestCamera(const GeoPosition& target);
    
    // Status
    bool isTracking(const QString& cameraId) const;
    QString trackedTrack(const QString& cameraId) const;
    
signals:
    void slewStarted(const QString& cameraId, const GeoPosition& target);
    void slewComplete(const QString& cameraId);
    void trackingStarted(const QString& cameraId, const QString& trackId);
    void trackingStopped(const QString& cameraId);
    void trackLost(const QString& cameraId, const QString& trackId);
    
private slots:
    void onTrackUpdated(const QString& trackId);
    void onTrackDropped(const QString& trackId);
    void updateTracking();
    
private:
    TrackManager* m_trackManager = nullptr;
    VideoStreamManager* m_videoManager = nullptr;
    
    QHash<QString, QString> m_cameraTrackMap;  // cameraId -> trackId
    QHash<QString, PTZController*> m_ptzControllers;
    QTimer* m_updateTimer;
};

} // namespace CounterUAS

#endif // CAMERASLEWCONTROLLER_H
