#ifndef RTSPVIDEOSOURCE_H
#define RTSPVIDEOSOURCE_H

#include "video/VideoSource.h"
#include <QMediaPlayer>
#include <QVideoSink>
#include <QVideoFrame>

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
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onErrorOccurred(QMediaPlayer::Error error, const QString& errorString);
    void onVideoFrameChanged(const QVideoFrame& frame);
    
private:
    QUrl buildStreamUrl(const QUrl& baseUrl);
    
    QMediaPlayer* m_player;
    QVideoSink* m_videoSink;
    RTSPConfig m_config;
    
    bool m_isOpen = false;
    QVideoFrame m_lastFrame;
};

} // namespace CounterUAS

#endif // RTSPVIDEOSOURCE_H
