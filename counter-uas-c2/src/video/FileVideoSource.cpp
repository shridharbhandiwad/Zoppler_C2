#include "video/FileVideoSource.h"
#include "video/RTSPVideoSource.h"  // For VideoFrameGrabber in Qt5
#include "utils/Logger.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QMediaContent>
#endif

namespace CounterUAS {

FileVideoSource::FileVideoSource(const QString& sourceId, QObject* parent)
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
    
    QObject::connect(m_videoSink, &QVideoSink::videoFrameChanged,
            this, &FileVideoSource::onVideoFrameChanged);
#else
    m_player->setVideoOutput(m_videoSurface);
    
    QObject::connect(m_videoSurface, &VideoFrameGrabber::frameAvailable,
            this, &FileVideoSource::onFrameAvailable);
#endif
    QObject::connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, &FileVideoSource::onMediaStatusChanged);
    QObject::connect(m_player, &QMediaPlayer::positionChanged,
            this, &FileVideoSource::onPositionChanged);
    QObject::connect(m_player, &QMediaPlayer::durationChanged,
            this, &FileVideoSource::onDurationChanged);
    
    QObject::connect(m_frameTimer, &QTimer::timeout, this, &FileVideoSource::processFrame);
}

FileVideoSource::~FileVideoSource() {
    close();
}

bool FileVideoSource::open(const QUrl& url) {
    if (m_isOpen) {
        close();
    }
    
    m_url = url;
    
    setStatus(VideoSourceStatus::Connecting);
    
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    m_player->setSource(url);
#else
    m_player->setMedia(QMediaContent(url));
#endif
    m_isOpen = true;
    
    Logger::instance().info("FileVideoSource",
                           QString("%1 opening: %2")
                               .arg(m_sourceId)
                               .arg(url.toLocalFile()));
    
    return true;
}

void FileVideoSource::close() {
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
    
    Logger::instance().info("FileVideoSource", m_sourceId + " closed");
}

bool FileVideoSource::isOpen() const {
    return m_isOpen;
}

void FileVideoSource::setPlaybackSpeed(double speed) {
    m_playbackSpeed = qBound(0.1, speed, 10.0);
    m_player->setPlaybackRate(m_playbackSpeed);
}

void FileVideoSource::seek(qint64 positionMs) {
    m_player->setPosition(positionMs);
}

qint64 FileVideoSource::position() const {
    return m_player->position();
}

qint64 FileVideoSource::duration() const {
    return m_duration;
}

void FileVideoSource::processFrame() {
    // Frame processing handled by onVideoFrameChanged/onFrameAvailable
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void FileVideoSource::onVideoFrameChanged(const QVideoFrame& frame) {
    if (!frame.isValid()) return;
    
    QVideoFrame f = frame;
    f.map(QVideoFrame::ReadOnly);
    
    QImage image = f.toImage();
    
    f.unmap();
    
    if (!image.isNull()) {
        emitFrame(image);
    }
}
#else
void FileVideoSource::onFrameAvailable(const QVideoFrame& frame) {
    if (!frame.isValid()) return;
    
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

void FileVideoSource::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    switch (status) {
        case QMediaPlayer::LoadedMedia:
            setStatus(VideoSourceStatus::Connected);
            Logger::instance().info("FileVideoSource",
                                   m_sourceId + " media loaded");
            break;
            
        case QMediaPlayer::BufferedMedia:
            if (m_streaming) {
                setStatus(VideoSourceStatus::Streaming);
                m_player->play();
            }
            break;
            
        case QMediaPlayer::EndOfMedia:
            if (m_looping) {
                m_player->setPosition(0);
                m_player->play();
            } else {
                setStatus(VideoSourceStatus::Connected);
            }
            break;
            
        case QMediaPlayer::InvalidMedia:
            setError("Invalid media file");
            break;
            
        default:
            break;
    }
}

void FileVideoSource::onPositionChanged(qint64 position) {
    Q_UNUSED(position)
    // Could emit progress signal here
}

void FileVideoSource::onDurationChanged(qint64 duration) {
    m_duration = duration;
}

} // namespace CounterUAS
