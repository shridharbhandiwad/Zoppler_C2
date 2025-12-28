#include "video/VideoRecorder.h"
#include "utils/Logger.h"
#include <QDateTime>
#include <QFileInfo>
#include <QDir>

namespace CounterUAS {

VideoRecorder::VideoRecorder(QObject* parent)
    : QObject(parent)
{
}

VideoRecorder::~VideoRecorder() {
    stop();
}

void VideoRecorder::setConfig(const RecorderConfig& config) {
    m_config = config;
}

bool VideoRecorder::start(const QString& outputPath) {
    if (m_recording) {
        stop();
    }
    
    m_outputPath = outputPath;
    
    // Ensure output directory exists
    QFileInfo fileInfo(outputPath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            emit error("Failed to create output directory");
            return false;
        }
    }
    
    // In a real implementation, initialize video encoder here
    // For now, we just open a placeholder file
    m_outputFile = new QFile(outputPath);
    if (!m_outputFile->open(QIODevice::WriteOnly)) {
        emit error("Failed to open output file: " + m_outputFile->errorString());
        delete m_outputFile;
        m_outputFile = nullptr;
        return false;
    }
    
    m_recording = true;
    m_frameCount = 0;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    
    Logger::instance().info("VideoRecorder",
                           "Started recording to: " + outputPath);
    
    emit recordingChanged(true);
    return true;
}

void VideoRecorder::stop() {
    if (!m_recording) return;
    
    m_recording = false;
    
    finalizeRecording();
    
    if (m_outputFile) {
        m_outputFile->close();
        delete m_outputFile;
        m_outputFile = nullptr;
    }
    
    Logger::instance().info("VideoRecorder",
                           QString("Stopped recording. Frames: %1, Duration: %2s")
                               .arg(m_frameCount)
                               .arg(recordedDuration() / 1000.0, 0, 'f', 1));
    
    emit recordingChanged(false);
}

qint64 VideoRecorder::recordedDuration() const {
    if (!m_recording) return 0;
    return QDateTime::currentMSecsSinceEpoch() - m_startTime;
}

qint64 VideoRecorder::fileSize() const {
    if (m_outputFile) {
        return m_outputFile->size();
    }
    return 0;
}

void VideoRecorder::startEventRecording(const QString& eventId) {
    if (m_eventRecording) return;
    
    m_eventRecording = true;
    m_currentEventId = eventId;
    
    // Copy pre-buffer frames to recording
    QMutexLocker locker(&m_mutex);
    while (!m_preBuffer.isEmpty()) {
        auto frame = m_preBuffer.dequeue();
        m_frameQueue.enqueue(frame);
    }
    
    Logger::instance().info("VideoRecorder",
                           "Started event recording: " + eventId);
    
    emit eventRecordingStarted(eventId);
}

void VideoRecorder::stopEventRecording() {
    if (!m_eventRecording) return;
    
    m_eventRecording = false;
    
    QString clipPath = m_outputPath;  // Would create separate clip file
    
    Logger::instance().info("VideoRecorder",
                           "Stopped event recording: " + m_currentEventId);
    
    emit eventRecordingStopped(m_currentEventId, clipPath);
    m_currentEventId.clear();
}

void VideoRecorder::setMetadata(const FrameMetadata& metadata) {
    m_currentMetadata = metadata;
}

void VideoRecorder::addFrame(const QImage& frame, qint64 timestamp) {
    if (!m_recording && !m_eventRecording) {
        // Add to pre-buffer for event recording
        QMutexLocker locker(&m_mutex);
        m_preBuffer.enqueue(qMakePair(frame, timestamp));
        
        // Trim pre-buffer
        int maxPreBufferFrames = static_cast<int>(m_config.preBufferSeconds * m_config.fps);
        while (m_preBuffer.size() > maxPreBufferFrames) {
            m_preBuffer.dequeue();
        }
        return;
    }
    
    QMutexLocker locker(&m_mutex);
    m_frameQueue.enqueue(qMakePair(frame, timestamp));
    
    // Process queue
    locker.unlock();
    processWriteQueue();
}

void VideoRecorder::processWriteQueue() {
    QMutexLocker locker(&m_mutex);
    
    while (!m_frameQueue.isEmpty()) {
        auto framePair = m_frameQueue.dequeue();
        locker.unlock();
        
        writeFrame(framePair.first, framePair.second);
        
        locker.relock();
    }
}

void VideoRecorder::writeFrame(const QImage& frame, qint64 timestamp) {
    Q_UNUSED(timestamp)
    
    if (!m_outputFile || !m_outputFile->isOpen()) return;
    
    // In a real implementation, encode and write frame
    // For simulation, just write raw image data
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    frame.save(&buffer, "PNG");
    
    m_outputFile->write(data);
    
    m_frameCount++;
    emit frameRecorded(m_frameCount);
}

void VideoRecorder::initializeEncoder() {
    // Would initialize video encoder (FFmpeg, GStreamer, etc.)
}

void VideoRecorder::finalizeRecording() {
    // Would finalize video encoding (write trailer, close codec)
}

} // namespace CounterUAS
