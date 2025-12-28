#include "video/VideoSource.h"
#include "utils/Logger.h"
#include <QMutexLocker>
#include <QDateTime>

namespace CounterUAS {

VideoSource::VideoSource(const QString& sourceId, QObject* parent)
    : QObject(parent)
    , m_sourceId(sourceId)
    , m_frameTimer(new QTimer(this))
    , m_statsTimer(new QTimer(this))
    , m_reconnectTimer(new QTimer(this))
{
    connect(m_statsTimer, &QTimer::timeout, this, &VideoSource::updateStats);
    m_statsTimer->setInterval(1000);
    
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &VideoSource::attemptReconnect);
}

VideoSource::~VideoSource() {
    stop();
    // Note: close() is pure virtual and must be called by derived class destructors
    // Calling it here would invoke undefined behavior since derived class is already destroyed
}

void VideoSource::start() {
    if (m_streaming) return;
    
    if (!isOpen()) {
        setError("Cannot start: source not open");
        return;
    }
    
    m_streaming = true;
    m_frameTimer->start(static_cast<int>(1000.0 / m_targetFPS));
    m_statsTimer->start();
    
    setStatus(VideoSourceStatus::Streaming);
    emit streamingChanged(true);
    
    Logger::instance().info("VideoSource", m_sourceId + " started streaming");
}

void VideoSource::stop() {
    if (!m_streaming) return;
    
    m_streaming = false;
    m_frameTimer->stop();
    m_statsTimer->stop();
    
    if (m_status == VideoSourceStatus::Streaming) {
        setStatus(VideoSourceStatus::Connected);
    }
    
    emit streamingChanged(false);
    
    Logger::instance().info("VideoSource", m_sourceId + " stopped streaming");
}

void VideoSource::pause() {
    if (!m_streaming) return;
    
    m_frameTimer->stop();
    setStatus(VideoSourceStatus::Paused);
    
    Logger::instance().info("VideoSource", m_sourceId + " paused");
}

void VideoSource::resume() {
    if (m_status != VideoSourceStatus::Paused) return;
    
    m_frameTimer->start();
    setStatus(VideoSourceStatus::Streaming);
    
    Logger::instance().info("VideoSource", m_sourceId + " resumed");
}

QImage VideoSource::currentFrame() const {
    QMutexLocker locker(&m_frameMutex);
    return m_currentFrame;
}

void VideoSource::setTargetFPS(double fps) {
    m_targetFPS = qBound(1.0, fps, 120.0);
    if (m_streaming) {
        m_frameTimer->setInterval(static_cast<int>(1000.0 / m_targetFPS));
    }
}

void VideoSource::setStatus(VideoSourceStatus status) {
    if (m_status != status) {
        m_status = status;
        emit statusChanged(status);
        
        if (status == VideoSourceStatus::Connected) {
            emit connected();
        } else if (status == VideoSourceStatus::Disconnected) {
            emit disconnected();
        }
    }
}

void VideoSource::setError(const QString& message) {
    m_errorString = message;
    setStatus(VideoSourceStatus::Error);
    emit error(message);
    
    Logger::instance().error("VideoSource", m_sourceId + ": " + message);
    
    // Attempt reconnection if enabled
    if (m_autoReconnect && m_streaming) {
        setStatus(VideoSourceStatus::Reconnecting);
        m_reconnectTimer->start(m_reconnectIntervalMs);
    }
}

void VideoSource::emitFrame(const QImage& frame) {
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    
    {
        QMutexLocker locker(&m_frameMutex);
        m_currentFrame = frame;
        m_currentTimestamp = now;
    }
    
    m_stats.framesReceived++;
    m_stats.lastFrameTime = now;
    m_stats.width = frame.width();
    m_stats.height = frame.height();
    
    emit frameReady(frame, now);
}

void VideoSource::updateStats() {
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 elapsed = now - m_lastStatsTime;
    
    if (elapsed > 0) {
        qint64 framesDelta = m_stats.framesReceived - m_framesAtLastStats;
        m_stats.fps = (framesDelta * 1000.0) / elapsed;
    }
    
    m_lastStatsTime = now;
    m_framesAtLastStats = m_stats.framesReceived;
    
    // Calculate latency
    if (m_stats.lastFrameTime > 0) {
        m_stats.latencyMs = now - m_stats.lastFrameTime;
    }
    
    emit statsUpdated(m_stats);
}

void VideoSource::attemptReconnect() {
    if (m_status != VideoSourceStatus::Reconnecting) return;
    
    Logger::instance().info("VideoSource", m_sourceId + " attempting reconnection...");
    
    close();
    if (open(m_url)) {
        if (m_streaming) {
            start();
        }
    } else {
        // Retry
        m_reconnectTimer->start(m_reconnectIntervalMs);
    }
}

} // namespace CounterUAS
