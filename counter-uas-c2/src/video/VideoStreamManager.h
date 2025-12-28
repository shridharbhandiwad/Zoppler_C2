#ifndef VIDEOSTREAMMANAGER_H
#define VIDEOSTREAMMANAGER_H

#include <QObject>
#include <QHash>
#include <QMutex>
#include "video/VideoSource.h"
#include "core/Track.h"

namespace CounterUAS {

class RTSPVideoSource;
class GigEVideoSource;
class FileVideoSource;
class VideoRecorder;

/**
 * @brief Camera definition for stream management
 */
struct CameraDefinition {
    QString cameraId;
    QString name;
    QString streamUrl;
    QString sourceType;  // "RTSP", "GigE", "FILE", "USB"
    GeoPosition position;
    bool hasPTZ = false;
    QString ptzProtocol;
    QString ptzAddress;
    int ptzPort = 80;
    QVariantMap metadata;
};

/**
 * @brief Video Stream Manager for multi-source video handling
 */
class VideoStreamManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(int activeStreamCount READ activeStreamCount NOTIFY activeStreamCountChanged)
    
public:
    static constexpr int MAX_STREAMS = 16;
    static constexpr int MAX_DISPLAY_STREAMS = 9;
    
    explicit VideoStreamManager(QObject* parent = nullptr);
    ~VideoStreamManager() override;
    
    // Stream management
    QString addStream(const CameraDefinition& camera);
    void removeStream(const QString& cameraId);
    void removeAllStreams();
    
    // Stream access
    VideoSource* stream(const QString& cameraId) const;
    QList<VideoSource*> allStreams() const;
    QList<QString> streamIds() const;
    int activeStreamCount() const;
    
    // Stream control
    void startStream(const QString& cameraId);
    void stopStream(const QString& cameraId);
    void startAllStreams();
    void stopAllStreams();
    
    // Frame access
    QImage currentFrame(const QString& cameraId) const;
    
    // Primary/selected stream
    void setPrimaryStream(const QString& cameraId);
    QString primaryStreamId() const { return m_primaryStreamId; }
    VideoSource* primaryStream() const;
    
    // Recording
    void startRecording(const QString& cameraId, const QString& outputPath);
    void stopRecording(const QString& cameraId);
    void startAllRecording(const QString& outputDir);
    void stopAllRecording();
    bool isRecording(const QString& cameraId) const;
    
    // Camera slew
    void slewCamera(const QString& cameraId, const GeoPosition& target);
    void slewNearestCamera(const GeoPosition& target);
    QString findNearestCamera(const GeoPosition& target) const;
    
    // Track association
    void associateTrackWithCamera(const QString& trackId, const QString& cameraId);
    QString cameraForTrack(const QString& trackId) const;
    
    // Status
    struct StreamStatus {
        QString cameraId;
        VideoSourceStatus status;
        bool recording;
        double fps;
        QSize resolution;
    };
    QList<StreamStatus> allStreamStatus() const;
    
signals:
    void streamAdded(const QString& cameraId);
    void streamRemoved(const QString& cameraId);
    void streamStatusChanged(const QString& cameraId, VideoSourceStatus status);
    void frameReady(const QString& cameraId, const QImage& frame);
    void primaryStreamChanged(const QString& cameraId);
    void recordingStarted(const QString& cameraId);
    void recordingStopped(const QString& cameraId);
    void activeStreamCountChanged(int count);
    void cameraSlewing(const QString& cameraId, const GeoPosition& target);
    void cameraSlewComplete(const QString& cameraId);
    
public slots:
    void onTrackUpdated(const QString& trackId, const GeoPosition& pos);
    void onTrackDropped(const QString& trackId);
    
private slots:
    void onStreamFrameReady(const QImage& frame, qint64 timestamp);
    void onStreamStatusChanged(VideoSourceStatus status);
    
private:
    VideoSource* createSource(const CameraDefinition& camera);
    
    mutable QMutex m_mutex;
    QHash<QString, VideoSource*> m_streams;
    QHash<QString, CameraDefinition> m_cameras;
    QHash<QString, VideoRecorder*> m_recorders;
    QHash<QString, QString> m_trackCameraMap;  // trackId -> cameraId
    
    QString m_primaryStreamId;
};

} // namespace CounterUAS

#endif // VIDEOSTREAMMANAGER_H
