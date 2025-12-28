#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include <QObject>
#include <QImage>
#include <QMutex>
#include <QTimer>
#include <QUrl>

namespace CounterUAS {

/**
 * @brief Video source status
 */
enum class VideoSourceStatus {
    Disconnected = 0,
    Connecting,
    Connected,
    Streaming,
    Paused,
    Error,
    Reconnecting
};

/**
 * @brief Video source statistics
 */
struct VideoSourceStats {
    qint64 framesReceived = 0;
    qint64 framesDropped = 0;
    double fps = 0.0;
    double bitrate = 0.0;
    int width = 0;
    int height = 0;
    qint64 latencyMs = 0;
    qint64 lastFrameTime = 0;
};

/**
 * @brief Abstract base class for video sources
 */
class VideoSource : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString sourceId READ sourceId CONSTANT)
    Q_PROPERTY(VideoSourceStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool streaming READ isStreaming NOTIFY streamingChanged)
    
public:
    explicit VideoSource(const QString& sourceId, QObject* parent = nullptr);
    ~VideoSource() override;
    
    // Identity
    QString sourceId() const { return m_sourceId; }
    virtual QString sourceType() const = 0;
    
    // Connection
    virtual bool open(const QUrl& url) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    
    // Streaming control
    virtual void start();
    virtual void stop();
    virtual void pause();
    virtual void resume();
    bool isStreaming() const { return m_streaming; }
    
    // Frame access
    QImage currentFrame() const;
    qint64 currentFrameNumber() const { return m_stats.framesReceived; }
    qint64 currentTimestamp() const { return m_currentTimestamp; }
    
    // Status
    VideoSourceStatus status() const { return m_status; }
    VideoSourceStats stats() const { return m_stats; }
    QString errorString() const { return m_errorString; }
    
    // Configuration
    void setTargetFPS(double fps);
    double targetFPS() const { return m_targetFPS; }
    
    void setBufferSize(int frames) { m_bufferSize = frames; }
    int bufferSize() const { return m_bufferSize; }
    
    void setAutoReconnect(bool enable) { m_autoReconnect = enable; }
    bool autoReconnect() const { return m_autoReconnect; }
    
    void setReconnectInterval(int ms) { m_reconnectIntervalMs = ms; }
    int reconnectInterval() const { return m_reconnectIntervalMs; }
    
signals:
    void frameReady(const QImage& frame, qint64 timestamp);
    void statusChanged(VideoSourceStatus status);
    void streamingChanged(bool streaming);
    void error(const QString& message);
    void statsUpdated(const VideoSourceStats& stats);
    void connected();
    void disconnected();
    
protected slots:
    virtual void processFrame() = 0;
    void updateStats();
    void attemptReconnect();
    
protected:
    void setStatus(VideoSourceStatus status);
    void setError(const QString& message);
    void emitFrame(const QImage& frame);
    
    QString m_sourceId;
    QUrl m_url;
    VideoSourceStatus m_status = VideoSourceStatus::Disconnected;
    VideoSourceStats m_stats;
    QString m_errorString;
    
    mutable QMutex m_frameMutex;
    QImage m_currentFrame;
    qint64 m_currentTimestamp = 0;
    
    QTimer* m_frameTimer;
    QTimer* m_statsTimer;
    QTimer* m_reconnectTimer;
    
    double m_targetFPS = 30.0;
    int m_bufferSize = 3;
    bool m_streaming = false;
    bool m_autoReconnect = true;
    int m_reconnectIntervalMs = 5000;
    
    qint64 m_lastStatsTime = 0;
    qint64 m_framesAtLastStats = 0;
};

} // namespace CounterUAS

Q_DECLARE_METATYPE(CounterUAS::VideoSourceStatus)

#endif // VIDEOSOURCE_H
