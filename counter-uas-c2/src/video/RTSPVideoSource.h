#ifndef RTSPVIDEOSOURCE_H
#define RTSPVIDEOSOURCE_H

#include "video/VideoSource.h"
#include <QMediaPlayer>
#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QVideoSink>
#include <QVideoFrame>
#else
#include <QAbstractVideoSurface>
#include <QVideoFrame>
#endif

namespace CounterUAS {

/**
 * @brief RTSP video source configuration
 */
struct RTSPConfig {
    QString username;
    QString password;
    int connectionTimeoutMs = 5000;
    int bufferTimeMs = 200;
    bool useTCP = true;  // TCP vs UDP transport
    bool lowLatencyMode = true;
    QString userAgent = "CounterUAS-C2/1.0";
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
// Forward declaration for Qt5 video surface
class VideoFrameGrabber;
#endif

/**
 * @brief RTSP video source implementation using Qt Multimedia
 */
class RTSPVideoSource : public VideoSource {
    Q_OBJECT
    
public:
    explicit RTSPVideoSource(const QString& sourceId, QObject* parent = nullptr);
    ~RTSPVideoSource() override;
    
    QString sourceType() const override { return "RTSP"; }
    
    // VideoSource implementation
    bool open(const QUrl& url) override;
    void close() override;
    bool isOpen() const override;
    
    // RTSP-specific configuration
    void setConfig(const RTSPConfig& config);
    RTSPConfig config() const { return m_config; }
    
    // Stream info
    QSize resolution() const;
    QString codecName() const;
    
protected slots:
    void processFrame() override;
    
private slots:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onErrorOccurred(QMediaPlayer::Error error, const QString& errorString);
    void onVideoFrameChanged(const QVideoFrame& frame);
#else
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onStateChanged(QMediaPlayer::State state);
    void onError(QMediaPlayer::Error error);
    void onFrameAvailable(const QVideoFrame& frame);
#endif
    
private:
    QUrl buildStreamUrl(const QUrl& baseUrl);
    
    QMediaPlayer* m_player;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QVideoSink* m_videoSink;
#else
    VideoFrameGrabber* m_videoSurface;
#endif
    RTSPConfig m_config;
    
    bool m_isOpen = false;
    QVideoFrame m_lastFrame;
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
/**
 * @brief Video surface to capture frames in Qt5
 */
class VideoFrameGrabber : public QAbstractVideoSurface {
    Q_OBJECT
public:
    explicit VideoFrameGrabber(QObject* parent = nullptr);
    
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
        QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override;
    
    bool present(const QVideoFrame& frame) override;
    
signals:
    void frameAvailable(const QVideoFrame& frame);
};
#endif

} // namespace CounterUAS

#endif // RTSPVIDEOSOURCE_H
