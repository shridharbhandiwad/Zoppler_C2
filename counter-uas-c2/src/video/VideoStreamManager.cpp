#include "video/VideoStreamManager.h"
#include "video/RTSPVideoSource.h"
#include "video/GigEVideoSource.h"
#include "video/FileVideoSource.h"
#include "video/VideoRecorder.h"
#include "utils/Logger.h"
#include <QMutexLocker>
#include <cmath>

namespace CounterUAS {

VideoStreamManager::VideoStreamManager(QObject* parent)
    : QObject(parent)
{
}

VideoStreamManager::~VideoStreamManager() {
    removeAllStreams();
}

QString VideoStreamManager::addStream(const CameraDefinition& camera) {
    QMutexLocker locker(&m_mutex);
    
    if (m_streams.size() >= MAX_STREAMS) {
        Logger::instance().warning("VideoStreamManager",
                                  "Maximum stream count reached");
        return QString();
    }
    
    if (m_streams.contains(camera.cameraId)) {
        Logger::instance().warning("VideoStreamManager",
                                  "Stream already exists: " + camera.cameraId);
        return camera.cameraId;
    }
    
    VideoSource* source = createSource(camera);
    if (!source) {
        return QString();
    }
    
    m_streams[camera.cameraId] = source;
    m_cameras[camera.cameraId] = camera;
    
    // Connect signals
    connect(source, &VideoSource::frameReady,
            this, &VideoStreamManager::onStreamFrameReady);
    connect(source, &VideoSource::statusChanged,
            this, &VideoStreamManager::onStreamStatusChanged);
    
    locker.unlock();
    
    Logger::instance().info("VideoStreamManager",
                           "Added stream: " + camera.cameraId);
    
    emit streamAdded(camera.cameraId);
    emit activeStreamCountChanged(m_streams.size());
    
    return camera.cameraId;
}

void VideoStreamManager::removeStream(const QString& cameraId) {
    QMutexLocker locker(&m_mutex);
    
    VideoSource* source = m_streams.take(cameraId);
    if (!source) return;
    
    // Stop recording if active
    if (m_recorders.contains(cameraId)) {
        m_recorders[cameraId]->stop();
        delete m_recorders.take(cameraId);
    }
    
    source->stop();
    source->close();
    delete source;
    
    m_cameras.remove(cameraId);
    
    // Clear track associations
    QStringList tracksToRemove;
    for (auto it = m_trackCameraMap.begin(); it != m_trackCameraMap.end(); ++it) {
        if (it.value() == cameraId) {
            tracksToRemove.append(it.key());
        }
    }
    for (const QString& trackId : tracksToRemove) {
        m_trackCameraMap.remove(trackId);
    }
    
    if (m_primaryStreamId == cameraId) {
        m_primaryStreamId.clear();
        if (!m_streams.isEmpty()) {
            m_primaryStreamId = m_streams.keys().first();
        }
    }
    
    locker.unlock();
    
    Logger::instance().info("VideoStreamManager",
                           "Removed stream: " + cameraId);
    
    emit streamRemoved(cameraId);
    emit activeStreamCountChanged(m_streams.size());
}

void VideoStreamManager::removeAllStreams() {
    QStringList ids = streamIds();
    for (const QString& id : ids) {
        removeStream(id);
    }
}

VideoSource* VideoStreamManager::stream(const QString& cameraId) const {
    QMutexLocker locker(&m_mutex);
    return m_streams.value(cameraId, nullptr);
}

QList<VideoSource*> VideoStreamManager::allStreams() const {
    QMutexLocker locker(&m_mutex);
    return m_streams.values();
}

QList<QString> VideoStreamManager::streamIds() const {
    QMutexLocker locker(&m_mutex);
    return m_streams.keys();
}

int VideoStreamManager::activeStreamCount() const {
    QMutexLocker locker(&m_mutex);
    return m_streams.size();
}

void VideoStreamManager::startStream(const QString& cameraId) {
    VideoSource* source = stream(cameraId);
    if (!source) return;
    
    if (!source->isOpen()) {
        CameraDefinition camera = m_cameras.value(cameraId);
        source->open(QUrl(camera.streamUrl));
    }
    source->start();
}

void VideoStreamManager::stopStream(const QString& cameraId) {
    VideoSource* source = stream(cameraId);
    if (source) {
        source->stop();
    }
}

void VideoStreamManager::startAllStreams() {
    QList<QString> ids = streamIds();
    for (const QString& id : ids) {
        startStream(id);
    }
}

void VideoStreamManager::stopAllStreams() {
    QList<QString> ids = streamIds();
    for (const QString& id : ids) {
        stopStream(id);
    }
}

QImage VideoStreamManager::currentFrame(const QString& cameraId) const {
    VideoSource* source = stream(cameraId);
    if (source) {
        return source->currentFrame();
    }
    return QImage();
}

void VideoStreamManager::setPrimaryStream(const QString& cameraId) {
    QMutexLocker locker(&m_mutex);
    
    if (m_streams.contains(cameraId) && m_primaryStreamId != cameraId) {
        m_primaryStreamId = cameraId;
        
        locker.unlock();
        
        emit primaryStreamChanged(cameraId);
    }
}

VideoSource* VideoStreamManager::primaryStream() const {
    return stream(m_primaryStreamId);
}

void VideoStreamManager::startRecording(const QString& cameraId, const QString& outputPath) {
    QMutexLocker locker(&m_mutex);
    
    VideoSource* source = m_streams.value(cameraId);
    if (!source) return;
    
    if (m_recorders.contains(cameraId)) {
        m_recorders[cameraId]->stop();
        delete m_recorders[cameraId];
    }
    
    VideoRecorder* recorder = new VideoRecorder(this);
    m_recorders[cameraId] = recorder;
    
    // Connect source to recorder
    connect(source, &VideoSource::frameReady,
            recorder, &VideoRecorder::addFrame);
    
    recorder->start(outputPath);
    
    locker.unlock();
    
    Logger::instance().info("VideoStreamManager",
                           QString("Started recording: %1 -> %2")
                               .arg(cameraId)
                               .arg(outputPath));
    
    emit recordingStarted(cameraId);
}

void VideoStreamManager::stopRecording(const QString& cameraId) {
    QMutexLocker locker(&m_mutex);
    
    VideoRecorder* recorder = m_recorders.take(cameraId);
    if (recorder) {
        recorder->stop();
        delete recorder;
        
        locker.unlock();
        
        Logger::instance().info("VideoStreamManager",
                               "Stopped recording: " + cameraId);
        
        emit recordingStopped(cameraId);
    }
}

void VideoStreamManager::startAllRecording(const QString& outputDir) {
    QList<QString> ids = streamIds();
    for (const QString& id : ids) {
        QString path = QString("%1/%2_%3.mp4")
                           .arg(outputDir)
                           .arg(id)
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
        startRecording(id, path);
    }
}

void VideoStreamManager::stopAllRecording() {
    QList<QString> ids = streamIds();
    for (const QString& id : ids) {
        stopRecording(id);
    }
}

bool VideoStreamManager::isRecording(const QString& cameraId) const {
    QMutexLocker locker(&m_mutex);
    return m_recorders.contains(cameraId);
}

void VideoStreamManager::slewCamera(const QString& cameraId, const GeoPosition& target) {
    // Would send PTZ command to camera
    emit cameraSlewing(cameraId, target);
    
    // In a real implementation, this would send PTZ commands
    // and emit cameraSlewComplete when done
    
    Logger::instance().info("VideoStreamManager",
                           QString("Slewing camera %1 to lat=%2, lon=%3")
                               .arg(cameraId)
                               .arg(target.latitude, 0, 'f', 6)
                               .arg(target.longitude, 0, 'f', 6));
}

void VideoStreamManager::slewNearestCamera(const GeoPosition& target) {
    QString nearest = findNearestCamera(target);
    if (!nearest.isEmpty()) {
        slewCamera(nearest, target);
    }
}

QString VideoStreamManager::findNearestCamera(const GeoPosition& target) const {
    QMutexLocker locker(&m_mutex);
    
    QString nearest;
    double minDist = std::numeric_limits<double>::max();
    
    Track tempTrack("temp");
    tempTrack.setPosition(target);
    
    for (auto it = m_cameras.begin(); it != m_cameras.end(); ++it) {
        if (!it.value().hasPTZ) continue;
        
        double dist = tempTrack.distanceTo(it.value().position);
        if (dist < minDist) {
            minDist = dist;
            nearest = it.key();
        }
    }
    
    return nearest;
}

void VideoStreamManager::associateTrackWithCamera(const QString& trackId, const QString& cameraId) {
    QMutexLocker locker(&m_mutex);
    m_trackCameraMap[trackId] = cameraId;
}

QString VideoStreamManager::cameraForTrack(const QString& trackId) const {
    QMutexLocker locker(&m_mutex);
    return m_trackCameraMap.value(trackId);
}

QList<VideoStreamManager::StreamStatus> VideoStreamManager::allStreamStatus() const {
    QMutexLocker locker(&m_mutex);
    
    QList<StreamStatus> result;
    
    for (auto it = m_streams.begin(); it != m_streams.end(); ++it) {
        StreamStatus status;
        status.cameraId = it.key();
        status.status = it.value()->status();
        status.recording = m_recorders.contains(it.key());
        status.fps = it.value()->stats().fps;
        status.resolution = QSize(it.value()->stats().width, it.value()->stats().height);
        result.append(status);
    }
    
    return result;
}

void VideoStreamManager::onTrackUpdated(const QString& trackId, const GeoPosition& pos) {
    QString cameraId = cameraForTrack(trackId);
    if (!cameraId.isEmpty()) {
        slewCamera(cameraId, pos);
    }
}

void VideoStreamManager::onTrackDropped(const QString& trackId) {
    QMutexLocker locker(&m_mutex);
    m_trackCameraMap.remove(trackId);
}

void VideoStreamManager::onStreamFrameReady(const QImage& frame, qint64 timestamp) {
    Q_UNUSED(timestamp)
    
    VideoSource* source = qobject_cast<VideoSource*>(sender());
    if (source) {
        emit frameReady(source->sourceId(), frame);
    }
}

void VideoStreamManager::onStreamStatusChanged(VideoSourceStatus status) {
    VideoSource* source = qobject_cast<VideoSource*>(sender());
    if (source) {
        emit streamStatusChanged(source->sourceId(), status);
    }
}

VideoSource* VideoStreamManager::createSource(const CameraDefinition& camera) {
    VideoSource* source = nullptr;
    
    if (camera.sourceType == "RTSP" || camera.sourceType.isEmpty()) {
        source = new RTSPVideoSource(camera.cameraId, this);
    } else if (camera.sourceType == "GigE") {
        source = new GigEVideoSource(camera.cameraId, this);
    } else if (camera.sourceType == "FILE") {
        source = new FileVideoSource(camera.cameraId, this);
    } else {
        Logger::instance().warning("VideoStreamManager",
                                  "Unknown source type: " + camera.sourceType);
        return nullptr;
    }
    
    return source;
}

} // namespace CounterUAS
