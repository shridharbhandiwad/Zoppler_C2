#include "video/RTSPVideoSource.h"
#include "utils/Logger.h"
#include <QUrlQuery>

namespace CounterUAS {

RTSPVideoSource::RTSPVideoSource(const QString& sourceId, QObject* parent)
    : VideoSource(sourceId, parent)
    , m_player(new QMediaPlayer(this))
    , m_videoSink(new QVideoSink(this))
{
    m_player->setVideoSink(m_videoSink);
    
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &RTSPVideoSource::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, &RTSPVideoSource::onPlaybackStateChanged);
    connect(m_player, &QMediaPlayer::errorOccurred,
            this, &RTSPVideoSource::onErrorOccurred);
    connect(m_videoSink, &QVideoSink::videoFrameChanged,
            this, &RTSPVideoSource::onVideoFrameChanged);
    
    // Connect frame timer
    connect(m_frameTimer, &QTimer::timeout, this, &RTSPVideoSource::processFrame);
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
    
    m_player->setSource(streamUrl);
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
    m_player->setSource(QUrl());
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
    // Frame processing is handled by onVideoFrameChanged
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
