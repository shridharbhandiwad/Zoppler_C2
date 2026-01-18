#include "simulators/VideoSimulator.h"
#include "video/SimulationVideoSource.h"
#include "video/VideoStreamManager.h"
#include "utils/Logger.h"
#include <QPainter>
#include <QDateTime>
#include <QRandomGenerator>
#include <QtMath>

namespace CounterUAS {

VideoSimulator::VideoSimulator(QObject* parent) 
    : QObject(parent)
    , m_timer(new QTimer(this)) 
{
    connect(m_timer, &QTimer::timeout, this, &VideoSimulator::generateFrame);
}

VideoSimulator::~VideoSimulator() {
    stop();
    destroySimulationSources();
}

void VideoSimulator::setVideoManager(VideoStreamManager* manager) {
    m_videoManager = manager;
}

void VideoSimulator::start() {
    if (m_running) return;
    
    m_running = true;
    m_frameCount = 0;
    
    if (m_legacyMode) {
        // Legacy mode: just emit frames from timer
        m_timer->start(33);  // ~30 fps
    } else {
        // Modern mode: create simulation video sources
        createSimulationSources();
        
        // Start all simulation sources
        for (auto* source : m_sources) {
            source->start();
        }
    }
    
    Logger::instance().info("VideoSimulator", "Simulation video started");
    emit simulationStarted();
}

void VideoSimulator::stop() {
    if (!m_running) return;
    
    m_running = false;
    m_timer->stop();
    
    // Stop all simulation sources
    for (auto* source : m_sources) {
        source->stop();
    }
    
    Logger::instance().info("VideoSimulator", "Simulation video stopped");
    emit simulationStopped();
}

void VideoSimulator::setResolution(int width, int height) {
    m_width = width;
    m_height = height;
}

void VideoSimulator::addSimulatedCamera(const SimulatedCamera& camera) {
    // Check if camera already exists
    for (auto& cam : m_cameraConfigs) {
        if (cam.cameraId == camera.cameraId) {
            cam = camera;
            return;
        }
    }
    m_cameraConfigs.append(camera);
}

void VideoSimulator::removeSimulatedCamera(const QString& cameraId) {
    for (int i = 0; i < m_cameraConfigs.size(); i++) {
        if (m_cameraConfigs[i].cameraId == cameraId) {
            m_cameraConfigs.removeAt(i);
            break;
        }
    }
}

void VideoSimulator::clearSimulatedCameras() {
    m_cameraConfigs.clear();
}

QList<VideoSimulator::SimulatedCamera> VideoSimulator::simulatedCameras() const {
    return m_cameraConfigs.toList();
}

void VideoSimulator::setupDefaultCameras() {
    clearSimulatedCameras();
    
    // Day Camera - Electro-Optical for daytime visibility
    SimulatedCamera dayCam;
    dayCam.cameraId = "SIM-DAY-001";
    dayCam.name = "Day Camera";
    dayCam.scenarioType = 0;  // EO scenario
    dayCam.enabled = true;
    addSimulatedCamera(dayCam);
    
    // Night Camera - Thermal/IR for nighttime visibility
    SimulatedCamera nightCam;
    nightCam.cameraId = "SIM-NIGHT-001";
    nightCam.name = "Night Camera";
    nightCam.scenarioType = 1;  // Thermal scenario
    nightCam.enabled = true;
    addSimulatedCamera(nightCam);
    
    Logger::instance().info("VideoSimulator", "Default cameras configured (Day/Night cameras)");
}

void VideoSimulator::createSimulationSources() {
    destroySimulationSources();
    
    // If no cameras configured, setup defaults
    if (m_cameraConfigs.isEmpty()) {
        setupDefaultCameras();
    }
    
    for (const auto& config : m_cameraConfigs) {
        if (!config.enabled) continue;
        
        // Create simulation source
        auto* source = new SimulationVideoSource(config.cameraId, this);
        source->setCameraName(config.name);
        source->setScenarioType(config.scenarioType);
        source->setShowOverlay(true);
        source->setTargetVisible(true);
        source->setTargetFPS(30.0);
        
        // Open in generated mode
        source->open(QUrl("simulation://generated"));
        
        // Connect frame signal
        connect(source, &SimulationVideoSource::frameReady,
                this, &VideoSimulator::onSourceFrameReady);
        
        m_sources[config.cameraId] = source;
        
        // Register with VideoStreamManager if available
        if (m_videoManager) {
            CameraDefinition camDef;
            camDef.cameraId = config.cameraId;
            camDef.name = config.name;
            camDef.sourceType = "SIMULATION";
            camDef.streamUrl = "simulation://generated";
            camDef.hasPTZ = true;
            
            m_videoManager->addStream(camDef);
        }
        
        Logger::instance().info("VideoSimulator", 
                               QString("Created simulation source: %1 (%2)")
                                   .arg(config.cameraId)
                                   .arg(config.name));
    }
}

void VideoSimulator::destroySimulationSources() {
    for (auto* source : m_sources) {
        source->stop();
        source->close();
        
        // Remove from video manager
        if (m_videoManager) {
            m_videoManager->removeStream(source->sourceId());
        }
        
        source->deleteLater();
    }
    m_sources.clear();
}

void VideoSimulator::onSourceFrameReady(const QImage& frame, qint64 timestamp) {
    auto* source = qobject_cast<SimulationVideoSource*>(sender());
    if (!source) return;
    
    emit cameraFrameReady(source->sourceId(), frame, timestamp);
    
    // For legacy compatibility, emit the first camera's frame as the primary
    if (source->sourceId() == m_cameraConfigs.first().cameraId) {
        emit frameReady(frame, timestamp);
    }
}

void VideoSimulator::generateFrame() {
    // Legacy frame generation for backward compatibility
    QImage frame(m_width, m_height, QImage::Format_RGB888);
    frame.fill(QColor(30, 30, 40));
    
    QPainter painter(&frame);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw grid
    painter.setPen(QPen(QColor(50, 50, 60), 1));
    for (int x = 0; x < m_width; x += 50) {
        painter.drawLine(x, 0, x, m_height);
    }
    for (int y = 0; y < m_height; y += 50) {
        painter.drawLine(0, y, m_width, y);
    }
    
    // Draw horizon line
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawLine(0, m_height / 2, m_width, m_height / 2);
    
    // Draw crosshairs
    painter.setPen(QPen(Qt::green, 2));
    int cx = m_width / 2;
    int cy = m_height / 2;
    painter.drawLine(cx - 30, cy, cx - 10, cy);
    painter.drawLine(cx + 10, cy, cx + 30, cy);
    painter.drawLine(cx, cy - 30, cx, cy - 10);
    painter.drawLine(cx, cy + 10, cx, cy + 30);
    
    // Draw simulated target (moving)
    double t = m_frameCount * 0.03;
    int targetX = cx + static_cast<int>(100 * std::sin(t));
    int targetY = cy + static_cast<int>(50 * std::cos(t * 0.7));
    
    painter.setPen(QPen(Qt::red, 2));
    painter.drawRect(targetX - 20, targetY - 15, 40, 30);
    painter.drawLine(targetX - 25, targetY, targetX + 25, targetY);
    painter.drawLine(targetX, targetY - 20, targetX, targetY + 20);
    
    // Draw telemetry text
    painter.setPen(Qt::white);
    painter.setFont(QFont("Courier", 10));
    painter.drawText(10, 20, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    painter.drawText(10, 40, QString("Frame: %1").arg(m_frameCount));
    painter.drawText(10, m_height - 10, QString("Az: %1%2  El: %3%4  Zoom: 1.0x")
                                            .arg(180 + 10 * std::sin(t * 0.1), 0, 'f', 1)
                                            .arg(QChar(0x00B0))
                                            .arg(5 * std::cos(t * 0.15), 0, 'f', 1)
                                            .arg(QChar(0x00B0)));
    
    // Simulated camera name
    painter.drawText(m_width - 100, 20, "SIM-CAM-001");
    
    // Status indicator
    painter.setPen(QColor(0, 255, 0));
    painter.drawText(m_width - 150, 40, "SIMULATION");
    
    painter.end();
    
    m_frameCount++;
    emit frameReady(frame, QDateTime::currentMSecsSinceEpoch());
}

} // namespace CounterUAS
