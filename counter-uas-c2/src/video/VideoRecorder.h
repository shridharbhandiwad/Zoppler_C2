#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H

#include <QObject>
#include <QImage>
#include <QFile>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <QVariant>

namespace CounterUAS {

/**
 * @brief Video recording configuration
 */
struct RecorderConfig {
    QString codec = "H264";
    int quality = 80;           // 0-100
    int bitrateMbps = 8;
    double fps = 30.0;
    bool embedMetadata = true;
    int preBufferSeconds = 30;
    int postBufferSeconds = 30;
};

/**
 * @brief Frame metadata for recording
 */
struct FrameMetadata {
    qint64 timestamp = 0;
    QString trackId;
    QString operatorNote;
    QVariantMap customData;
};

/**
 * @brief Video recorder for saving video streams
 */
class VideoRecorder : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool recording READ isRecording NOTIFY recordingChanged)
    
public:
    explicit VideoRecorder(QObject* parent = nullptr);
    ~VideoRecorder() override;
    
    // Configuration
    void setConfig(const RecorderConfig& config);
    RecorderConfig config() const { return m_config; }
    
    // Recording control
    bool start(const QString& outputPath);
    void stop();
    bool isRecording() const { return m_recording; }
    
    // Output
    QString outputPath() const { return m_outputPath; }
    qint64 recordedDuration() const;
    qint64 recordedFrameCount() const { return m_frameCount; }
    qint64 fileSize() const;
    
    // Event-triggered recording
    void startEventRecording(const QString& eventId);
    void stopEventRecording();
    bool isEventRecording() const { return m_eventRecording; }
    
    // Metadata
    void setMetadata(const FrameMetadata& metadata);
    
signals:
    void recordingChanged(bool recording);
    void frameRecorded(qint64 frameNumber);
    void error(const QString& message);
    void eventRecordingStarted(const QString& eventId);
    void eventRecordingStopped(const QString& eventId, const QString& clipPath);
    
public slots:
    void addFrame(const QImage& frame, qint64 timestamp);
    
private slots:
    void processWriteQueue();
    
private:
    void writeFrame(const QImage& frame, qint64 timestamp);
    void initializeEncoder();
    void finalizeRecording();
    
    RecorderConfig m_config;
    QString m_outputPath;
    
    bool m_recording = false;
    bool m_eventRecording = false;
    QString m_currentEventId;
    qint64 m_frameCount = 0;
    qint64 m_startTime = 0;
    
    QMutex m_mutex;
    QQueue<QPair<QImage, qint64>> m_frameQueue;
    QQueue<QPair<QImage, qint64>> m_preBuffer;
    
    FrameMetadata m_currentMetadata;
    
    // Placeholder for actual encoder
    QFile* m_outputFile = nullptr;
    QThread* m_writerThread = nullptr;
};

} // namespace CounterUAS

#endif // VIDEORECORDER_H
