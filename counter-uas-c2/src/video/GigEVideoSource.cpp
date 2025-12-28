#include "video/GigEVideoSource.h"
#include "utils/Logger.h"
#include <QImage>
#include <QRandomGenerator>
#include <QPainter>
#include <QFont>

namespace CounterUAS {

GigEVideoSource::GigEVideoSource(const QString& sourceId, QObject* parent)
    : VideoSource(sourceId, parent)
{
    QObject::connect(m_frameTimer, &QTimer::timeout, this, &GigEVideoSource::processFrame);
}

GigEVideoSource::~GigEVideoSource() {
    close();
}

void GigEVideoSource::setConfig(const GigEConfig& config) {
    m_config = config;
}

bool GigEVideoSource::open(const QUrl& url) {
    if (m_isOpen) {
        close();
    }
    
    m_url = url;
    
    // Extract device ID from URL if provided
    QString deviceId = url.path();
    if (deviceId.startsWith('/')) {
        deviceId = deviceId.mid(1);
    }
    if (!deviceId.isEmpty()) {
        m_config.deviceId = deviceId;
    }
    
    setStatus(VideoSourceStatus::Connecting);
    
    // In a real implementation, this would initialize the GigE SDK and connect to camera
    // For simulation, we just mark as open
    
    Logger::instance().info("GigEVideoSource",
                           QString("%1 opening device: %2")
                               .arg(m_sourceId)
                               .arg(m_config.deviceId));
    
    // Simulate successful connection
    m_isOpen = true;
    setStatus(VideoSourceStatus::Connected);
    
    return true;
}

void GigEVideoSource::close() {
    if (!m_isOpen) return;
    
    stop();
    
    // Stop capture thread
    m_stopCapture = true;
    if (m_captureThread) {
        m_captureThread->quit();
        m_captureThread->wait(1000);
        delete m_captureThread;
        m_captureThread = nullptr;
    }
    
    // Clean up camera handle
    m_cameraHandle = nullptr;
    m_isOpen = false;
    
    setStatus(VideoSourceStatus::Disconnected);
    
    Logger::instance().info("GigEVideoSource", m_sourceId + " closed");
}

bool GigEVideoSource::isOpen() const {
    return m_isOpen;
}

void GigEVideoSource::setExposure(double ms) {
    m_config.exposureMs = qBound(0.01, ms, 1000.0);
    // Would send command to camera here
}

void GigEVideoSource::setGain(double db) {
    m_config.gainDb = qBound(0.0, db, 48.0);
    // Would send command to camera here
}

void GigEVideoSource::setAutoExposure(bool enable) {
    m_config.autoExposure = enable;
    // Would send command to camera here
}

void GigEVideoSource::setAutoGain(bool enable) {
    m_config.autoGain = enable;
    // Would send command to camera here
}

QStringList GigEVideoSource::availableDevices() {
    // In a real implementation, this would enumerate GigE cameras
    QStringList devices;
    devices << "GigE-Camera-001" << "GigE-Camera-002";
    return devices;
}

void GigEVideoSource::processFrame() {
    if (!m_isOpen || !m_streaming) return;
    
    // In a real implementation, this would grab frames from the camera
    // For simulation, generate a test pattern
    
    int width = 1920;
    int height = 1080;
    
    QImage frame(width, height, QImage::Format_RGB888);
    frame.fill(Qt::darkGray);
    
    // Draw some test pattern
    QPainter painter(&frame);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 24));
    
    QString text = QString("GigE Camera: %1\nFrame: %2")
                       .arg(m_config.deviceId)
                       .arg(m_stats.framesReceived);
    
    painter.drawText(frame.rect(), Qt::AlignCenter, text);
    
    // Add timestamp
    painter.setFont(QFont("Arial", 12));
    painter.drawText(10, 30, QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
    
    painter.end();
    
    emitFrame(frame);
}

void GigEVideoSource::initializeCamera() {
    // Would initialize GigE camera here using SDK
}

void GigEVideoSource::captureLoop() {
    // Would run continuous capture loop in separate thread
}

} // namespace CounterUAS
