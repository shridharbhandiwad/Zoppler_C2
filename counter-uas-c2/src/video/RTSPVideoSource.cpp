#include "video/RTSPVideoSource.h"
#include "utils/Logger.h"
#include <QUrlQuery>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QMediaContent>
#endif

namespace CounterUAS {

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
// Qt5 VideoFrameGrabber implementation
VideoFrameGrabber::VideoFrameGrabber(QObject* parent)
    : QAbstractVideoSurface(parent)
{
}

QList<QVideoFrame::PixelFormat> VideoFrameGrabber::supportedPixelFormats(
    QAbstractVideoBuffer::HandleType type) const
{
    Q_UNUSED(type)
    return QList<QVideoFrame::PixelFormat>()
        << QVideoFrame::Format_ARGB32
        << QVideoFrame::Format_ARGB32_Premultiplied
        << QVideoFrame::Format_RGB32
        << QVideoFrame::Format_RGB24
        << QVideoFrame::Format_RGB565
        << QVideoFrame::Format_RGB555
        << QVideoFrame::Format_BGRA32
        << QVideoFrame::Format_BGRA32_Premultiplied
        << QVideoFrame::Format_BGR32
        << QVideoFrame::Format_BGR24
        << QVideoFrame::Format_BGR565
        << QVideoFrame::Format_BGR555
        << QVideoFrame::Format_YUV420P
        << QVideoFrame::Format_YV12
        << QVideoFrame::Format_UYVY
        << QVideoFrame::Format_YUYV
        << QVideoFrame::Format_NV12
        << QVideoFrame::Format_NV21;
}

bool VideoFrameGrabber::present(const QVideoFrame& frame)
{
    if (frame.isValid()) {
        emit frameAvailable(frame);
        return true;
    }
    return false;
}
#endif

RTSPVideoSource::RTSPVideoSource(const QString& sourceId, QObject* parent)
    : VideoSource(sourceId, parent)
    , m_player(new QMediaPlayer(this))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    , m_videoSink(new QVideoSink(this))
#else
    , m_videoSurface(new VideoFrameGrabber(this))
#endif
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    m_player->setVideoSink(m_videoSink);
    
    QObject::connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &RTSPVideoSource::onMediaStatusChanged);
    QObject::connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, &RTSPVideoSource::onPlaybackStateChanged);
    QObject::connect(m_player, &QMediaPlayer::errorOccurred,
            this, &RTSPVideoSource::onErrorOccurred);
    QObject::connect(m_videoSink, &QVideoSink::videoFrameChanged,
            this, &RTSPVideoSource::onVideoFrameChanged);
#else
    m_player->setVideoOutput(m_videoSurface);
    
    QObject::connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &RTSPVideoSource::onMediaStatusChanged);
    QObject::connect(m_player, &QMediaPlayer::stateChanged,
            this, &RTSPVideoSource::onStateChanged);
    QObject::connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
            this, &RTSPVideoSource::onError);
    QObject::connect(m_videoSurface, &VideoFrameGrabber::frameAvailable,
            this, &RTSPVideoSource::onFrameAvailable);
#endif
    
    // Connect frame timer
    QObject::connect(m_frameTimer, &QTimer::timeout, this, &RTSPVideoSource::processFrame);
}

RTSPVideoSource::~RTSPVideoSource() {
    close();
}

void RTSPVideoSource::setConfig(const RTSPConfig& config) {
    m_config = config;
}

bool RTSPVideoSource::open(const QUrl& url) {
    if (m_isOpen) {
        close();
    }
    
    m_url = url;
    QUrl streamUrl = buildStreamUrl(url);
    
    setStatus(VideoSourceStatus::Connecting);
    
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    m_player->setSource(streamUrl);
#else
    m_player->setMedia(QMediaContent(streamUrl));
#endif
    m_isOpen = true;
    
    Logger::instance().info("RTSPVideoSource",
                           QString("%1 opening: %2")
                               .arg(m_sourceId)
                               .arg(streamUrl.toString(QUrl::RemovePassword)));
    
    return true;
}

void RTSPVideoSource::close() {
    if (!m_isOpen) return;
    
    stop();
    m_player->stop();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    m_player->setSource(QUrl());
#else
    m_player->setMedia(QMediaContent());
#endif
    m_isOpen = false;
    
    setStatus(VideoSourceStatus::Disconnected);
    
    Logger::instance().info("RTSPVideoSource", m_sourceId + " closed");
}

bool RTSPVideoSource::isOpen() const {
    return m_isOpen;
}

QSize RTSPVideoSource::resolution() const {
    return QSize(m_stats.width, m_stats.height);
}

QString RTSPVideoSource::codecName() const {
    // Could be extracted from media metadata
    return "H.264";
}

void RTSPVideoSource::processFrame() {
    // Frame processing is handled by onVideoFrameChanged/onFrameAvailable
    // This timer just ensures we don't exceed target FPS
}

void RTSPVideoSource::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    switch (status) {
        case QMediaPlayer::LoadingMedia:
            setStatus(VideoSourceStatus::Connecting);
            break;
            
        case QMediaPlayer::LoadedMedia:
            setStatus(VideoSourceStatus::Connected);
            Logger::instance().info("RTSPVideoSource",
                                   m_sourceId + " media loaded");
            break;
            
        case QMediaPlayer::BufferingMedia:
        case QMediaPlayer::BufferedMedia:
            if (m_streaming) {
                setStatus(VideoSourceStatus::Streaming);
            }
            break;
            
        case QMediaPlayer::EndOfMedia:
            Logger::instance().warning("RTSPVideoSource",
                                      m_sourceId + " end of media");
            if (m_autoReconnect) {
                setStatus(VideoSourceStatus::Reconnecting);
                m_reconnectTimer->start(m_reconnectIntervalMs);
            }
            break;
            
        case QMediaPlayer::InvalidMedia:
            setError("Invalid media");
            break;
            
        default:
            break;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void RTSPVideoSource::onPlaybackStateChanged(QMediaPlayer::PlaybackState state) {
    switch (state) {
        case QMediaPlayer::PlayingState:
            if (m_streaming) {
                setStatus(VideoSourceStatus::Streaming);
            }
            break;
            
        case QMediaPlayer::PausedState:
            setStatus(VideoSourceStatus::Paused);
            break;
            
        case QMediaPlayer::StoppedState:
            if (m_isOpen && m_streaming) {
                // Unexpected stop - may need reconnection
                Logger::instance().warning("RTSPVideoSource",
                                          m_sourceId + " playback stopped unexpectedly");
            }
            break;
    }
}

void RTSPVideoSource::onErrorOccurred(QMediaPlayer::Error error, const QString& errorString) {
    Q_UNUSED(error)
    setError(errorString);
}

void RTSPVideoSource::onVideoFrameChanged(const QVideoFrame& frame) {
    if (!frame.isValid()) return;
    
    m_lastFrame = frame;
    
    // Convert to QImage
    QVideoFrame f = frame;
    f.map(QVideoFrame::ReadOnly);
    
    QImage image = f.toImage();
    
    f.unmap();
    
    if (!image.isNull()) {
        emitFrame(image);
    }
}
#else
void RTSPVideoSource::onStateChanged(QMediaPlayer::State state) {
    switch (state) {
        case QMediaPlayer::PlayingState:
            if (m_streaming) {
                setStatus(VideoSourceStatus::Streaming);
            }
            break;
            
        case QMediaPlayer::PausedState:
            setStatus(VideoSourceStatus::Paused);
            break;
            
        case QMediaPlayer::StoppedState:
            if (m_isOpen && m_streaming) {
                // Unexpected stop - may need reconnection
                Logger::instance().warning("RTSPVideoSource",
                                          m_sourceId + " playback stopped unexpectedly");
            }
            break;
    }
}

void RTSPVideoSource::onError(QMediaPlayer::Error error) {
    Q_UNUSED(error)
    setError(m_player->errorString());
}

void RTSPVideoSource::onFrameAvailable(const QVideoFrame& frame) {
    if (!frame.isValid()) return;
    
    m_lastFrame = frame;
    
    // Convert to QImage
    QVideoFrame f = frame;
    if (f.map(QAbstractVideoBuffer::ReadOnly)) {
        QImage image(f.bits(),
                     f.width(),
                     f.height(),
                     f.bytesPerLine(),
                     QVideoFrame::imageFormatFromPixelFormat(f.pixelFormat()));
        
        if (!image.isNull()) {
            emitFrame(image.copy());  // copy() because original data will be unmapped
        }
        f.unmap();
    }
}
#endif

QUrl RTSPVideoSource::buildStreamUrl(const QUrl& baseUrl) {
    QUrl url = baseUrl;
    
    // Add credentials if provided
    if (!m_config.username.isEmpty()) {
        url.setUserName(m_config.username);
        url.setPassword(m_config.password);
    }
    
    // Add transport parameter
    QUrlQuery query(url.query());
    if (m_config.useTCP) {
        query.addQueryItem("rtsp_transport", "tcp");
    } else {
        query.addQueryItem("rtsp_transport", "udp");
    }
    
    url.setQuery(query);
    
    return url;
}

} // namespace CounterUAS
