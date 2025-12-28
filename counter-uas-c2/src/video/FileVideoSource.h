#ifndef FILEVIDEOSOURCE_H
#define FILEVIDEOSOURCE_H

#include "video/VideoSource.h"
#include <QMediaPlayer>
#include <QVideoSink>

namespace CounterUAS {

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
    void onVideoFrameChanged(const QVideoFrame& frame);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    
private:
    QMediaPlayer* m_player;
    QVideoSink* m_videoSink;
    
    bool m_isOpen = false;
    bool m_looping = true;
    double m_playbackSpeed = 1.0;
    qint64 m_duration = 0;
};

} // namespace CounterUAS

#endif // FILEVIDEOSOURCE_H
