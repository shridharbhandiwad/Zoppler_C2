#ifndef FILEVIDEOSOURCE_H
#define FILEVIDEOSOURCE_H

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
// Forward declaration - use the same VideoFrameGrabber from RTSPVideoSource
class VideoFrameGrabber;
#endif

/**
 * @brief File-based video source for simulation and replay
 */
class FileVideoSource : public VideoSource {
    Q_OBJECT
    
public:
    explicit FileVideoSource(const QString& sourceId, QObject* parent = nullptr);
    ~FileVideoSource() override;
    
    QString sourceType() const override { return "FILE"; }
    
    // VideoSource implementation
    bool open(const QUrl& url) override;
    void close() override;
    bool isOpen() const override;
    
    // Playback control
    void setLooping(bool loop) { m_looping = loop; }
    bool isLooping() const { return m_looping; }
    
    void setPlaybackSpeed(double speed);
    double playbackSpeed() const { return m_playbackSpeed; }
    
    void seek(qint64 positionMs);
    qint64 position() const;
    qint64 duration() const;
    
protected slots:
    void processFrame() override;
    
private slots:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void onVideoFrameChanged(const QVideoFrame& frame);
#else
    void onFrameAvailable(const QVideoFrame& frame);
#endif
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    
private:
    QMediaPlayer* m_player;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QVideoSink* m_videoSink;
#else
    VideoFrameGrabber* m_videoSurface;
#endif
    
    bool m_isOpen = false;
    bool m_looping = true;
    double m_playbackSpeed = 1.0;
    qint64 m_duration = 0;
};

} // namespace CounterUAS

#endif // FILEVIDEOSOURCE_H
