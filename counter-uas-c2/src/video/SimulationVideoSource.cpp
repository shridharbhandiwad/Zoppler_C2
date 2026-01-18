#include "video/SimulationVideoSource.h"
#include "utils/Logger.h"
#include <QPainter>
#include <QDateTime>
#include <QRandomGenerator>
#include <QtMath>
#include <QDirIterator>

namespace CounterUAS {

SimulationVideoSource::SimulationVideoSource(const QString& sourceId, QObject* parent)
    : VideoSource(sourceId, parent)
{
    connect(m_frameTimer, &QTimer::timeout, this, &SimulationVideoSource::processFrame);
}

SimulationVideoSource::~SimulationVideoSource() {
    close();
}

bool SimulationVideoSource::open(const QUrl& url) {
    if (m_isOpen) {
        close();
    }
    
    m_url = url;
    
    // Check if URL points to a directory for image sequence
    if (url.isLocalFile()) {
        QString path = url.toLocalFile();
        QFileInfo info(path);
        if (info.isDir()) {
            if (!loadImageSequence(path)) {
                setError("Failed to load image sequence from: " + path);
                return false;
            }
            m_mode = SimulationMode::ImageSequence;
        }
    } else if (url.scheme() == "qrc") {
        // Load from Qt resources
        if (!loadFromResources(url.path())) {
            // Fall back to generated mode
            m_mode = SimulationMode::Generated;
        } else {
            m_mode = SimulationMode::Resources;
        }
    } else {
        // Default to generated mode
        m_mode = SimulationMode::Generated;
    }
    
    m_isOpen = true;
    m_currentImageIndex = 0;
    m_frameCount = 0;
    
    setStatus(VideoSourceStatus::Connected);
    
    Logger::instance().info("SimulationVideoSource",
                           QString("%1 opened in %2 mode")
                               .arg(m_sourceId)
                               .arg(m_mode == SimulationMode::Generated ? "generated" :
                                    m_mode == SimulationMode::ImageSequence ? "image sequence" : "resources"));
    
    return true;
}

void SimulationVideoSource::close() {
    if (!m_isOpen) return;
    
    stop();
    m_isOpen = false;
    m_imageFiles.clear();
    m_currentImageIndex = 0;
    
    setStatus(VideoSourceStatus::Disconnected);
    
    Logger::instance().info("SimulationVideoSource", m_sourceId + " closed");
}

bool SimulationVideoSource::isOpen() const {
    return m_isOpen;
}

bool SimulationVideoSource::loadImageSequence(const QString& directoryPath) {
    QDir dir(directoryPath);
    if (!dir.exists()) {
        return false;
    }
    
    m_imagePath = directoryPath;
    m_imageFiles.clear();
    
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif";
    
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Name);
    
    for (const QString& file : dir.entryList(QDir::Files)) {
        m_imageFiles.append(dir.absoluteFilePath(file));
    }
    
    Logger::instance().info("SimulationVideoSource",
                           QString("%1 loaded %2 images from %3")
                               .arg(m_sourceId)
                               .arg(m_imageFiles.size())
                               .arg(directoryPath));
    
    return !m_imageFiles.isEmpty();
}

bool SimulationVideoSource::loadFromResources(const QString& resourcePrefix) {
    m_imageFiles.clear();
    
    // Iterate through Qt resources
    QDirIterator it(resourcePrefix, QStringList() << "*.png" << "*.jpg" << "*.jpeg",
                    QDir::Files, QDirIterator::Subdirectories);
    
    while (it.hasNext()) {
        m_imageFiles.append(it.next());
    }
    
    m_imageFiles.sort();
    
    Logger::instance().info("SimulationVideoSource",
                           QString("%1 loaded %2 images from resources %3")
                               .arg(m_sourceId)
                               .arg(m_imageFiles.size())
                               .arg(resourcePrefix));
    
    return !m_imageFiles.isEmpty();
}

void SimulationVideoSource::processFrame() {
    if (!m_isOpen || !m_streaming) return;
    
    QImage frame;
    
    switch (m_mode) {
        case SimulationMode::Generated:
            frame = generateFrame();
            break;
            
        case SimulationMode::ImageSequence:
        case SimulationMode::Resources: {
            if (m_imageFiles.isEmpty()) {
                frame = generateFrame();  // Fallback
            } else {
                frame.load(m_imageFiles[m_currentImageIndex]);
                
                // Advance to next image
                m_currentImageIndex++;
                if (m_currentImageIndex >= m_imageFiles.size()) {
                    if (m_looping) {
                        m_currentImageIndex = 0;
                    } else {
                        m_currentImageIndex = m_imageFiles.size() - 1;
                    }
                }
                
                // Add overlay if enabled
                if (m_showOverlay && !frame.isNull()) {
                    frame = frame.convertToFormat(QImage::Format_RGB888);
                    drawOverlay(frame);
                }
            }
            break;
        }
    }
    
    if (!frame.isNull()) {
        m_frameCount++;
        emitFrame(frame);
    }
}

QImage SimulationVideoSource::generateFrame() {
    switch (m_scenarioType) {
        case 0:
            return generateUASTrackingFrame();
        case 1:
            return generateThermalFrame();
        case 2:
            return generateRadarFrame();
        default:
            return generateUASTrackingFrame();
    }
}

QImage SimulationVideoSource::generateUASTrackingFrame() {
    const int width = 1280;
    const int height = 720;
    
    QImage frame(width, height, QImage::Format_RGB888);
    
    // Sky gradient background
    QPainter painter(&frame);
    QLinearGradient skyGradient(0, 0, 0, height);
    skyGradient.setColorAt(0.0, QColor(100, 140, 180));
    skyGradient.setColorAt(0.4, QColor(150, 180, 210));
    skyGradient.setColorAt(0.5, QColor(170, 200, 220));
    skyGradient.setColorAt(0.6, QColor(100, 120, 80));
    skyGradient.setColorAt(1.0, QColor(60, 80, 50));
    painter.fillRect(0, 0, width, height, skyGradient);
    
    // Draw horizon line
    painter.setPen(QPen(QColor(80, 100, 60), 2));
    int horizonY = height / 2 + 20;
    painter.drawLine(0, horizonY, width, horizonY);
    
    // Draw some terrain features
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(70, 90, 60, 100));
    for (int i = 0; i < 5; i++) {
        int x = (i * width / 4) + static_cast<int>(m_frameCount % 50);
        int h = 30 + (i % 3) * 20;
        painter.drawEllipse(x - 50, horizonY - h/2, 100, h);
    }
    
    // Draw crosshairs
    drawCrosshairs(painter, width / 2, height / 2);
    
    // Draw target if visible
    if (m_targetVisible) {
        // Animate target position
        m_targetPhase += 0.05;
        int targetX = width / 2 + static_cast<int>(150 * std::sin(m_targetPhase));
        int targetY = height / 2 - 50 + static_cast<int>(30 * std::cos(m_targetPhase * 0.7));
        
        drawTarget(painter, targetX, targetY);
    }
    
    // Draw telemetry overlay
    if (m_showOverlay) {
        drawTelemetry(painter, frame.size());
    }
    
    painter.end();
    
    return frame;
}

QImage SimulationVideoSource::generateThermalFrame() {
    const int width = 1280;
    const int height = 720;
    
    QImage frame(width, height, QImage::Format_RGB888);
    
    QPainter painter(&frame);
    
    // Dark thermal background with noise
    frame.fill(QColor(20, 20, 30));
    
    // Add thermal noise
    auto* gen = QRandomGenerator::global();
    for (int y = 0; y < height; y += 4) {
        for (int x = 0; x < width; x += 4) {
            int noise = gen->bounded(20);
            painter.fillRect(x, y, 4, 4, QColor(20 + noise, 20 + noise, 25 + noise));
        }
    }
    
    // Draw hot spots (buildings/vehicles)
    painter.setBrush(QColor(150, 150, 100));
    painter.setPen(Qt::NoPen);
    painter.drawRect(100, height - 100, 80, 40);
    painter.drawRect(width - 200, height - 120, 60, 50);
    
    // Draw horizon
    painter.setPen(QPen(QColor(60, 60, 50), 1));
    painter.drawLine(0, height/2 + 30, width, height/2 + 30);
    
    // Draw crosshairs (white/hot)
    painter.setPen(QPen(QColor(255, 255, 200), 2));
    int cx = width / 2, cy = height / 2;
    painter.drawLine(cx - 40, cy, cx - 15, cy);
    painter.drawLine(cx + 15, cy, cx + 40, cy);
    painter.drawLine(cx, cy - 40, cx, cy - 15);
    painter.drawLine(cx, cy + 15, cx, cy + 40);
    
    // Draw target (hot signature)
    if (m_targetVisible) {
        m_targetPhase += 0.05;
        int targetX = width / 2 + static_cast<int>(120 * std::sin(m_targetPhase));
        int targetY = height / 2 - 40 + static_cast<int>(25 * std::cos(m_targetPhase * 0.8));
        
        // Hot core
        QRadialGradient hotSpot(targetX, targetY, 40);
        hotSpot.setColorAt(0, QColor(255, 255, 220));
        hotSpot.setColorAt(0.3, QColor(255, 200, 100));
        hotSpot.setColorAt(0.7, QColor(150, 100, 50));
        hotSpot.setColorAt(1, QColor(40, 40, 30));
        painter.setBrush(hotSpot);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(targetX - 30, targetY - 20, 60, 40);
        
        // Target box
        painter.setPen(QPen(QColor(255, 255, 200), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(targetX - 35, targetY - 25, 70, 50);
    }
    
    // Draw telemetry
    if (m_showOverlay) {
        painter.setPen(QColor(200, 200, 150));
        painter.setFont(QFont("Courier", 10));
        painter.drawText(10, 20, QString("THERMAL IR  %1").arg(m_cameraName));
        painter.drawText(10, 40, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
        painter.drawText(10, height - 20, QString("WFOV  POLARITY: WHITE HOT"));
        painter.drawText(width - 150, 20, QString("FRAME: %1").arg(m_frameCount));
    }
    
    painter.end();
    
    return frame;
}

QImage SimulationVideoSource::generateRadarFrame() {
    const int width = 720;
    const int height = 720;
    
    QImage frame(width, height, QImage::Format_RGB888);
    frame.fill(QColor(0, 20, 0));
    
    QPainter painter(&frame);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int cx = width / 2;
    int cy = height / 2;
    int maxRadius = qMin(cx, cy) - 20;
    
    // Draw range rings
    painter.setPen(QPen(QColor(0, 80, 0), 1));
    for (int r = maxRadius / 4; r <= maxRadius; r += maxRadius / 4) {
        painter.drawEllipse(cx - r, cy - r, r * 2, r * 2);
    }
    
    // Draw bearing lines
    for (int angle = 0; angle < 360; angle += 30) {
        double rad = qDegreesToRadians(static_cast<double>(angle));
        int x = cx + static_cast<int>(maxRadius * std::sin(rad));
        int y = cy - static_cast<int>(maxRadius * std::cos(rad));
        painter.drawLine(cx, cy, x, y);
    }
    
    // Draw sweep line
    double sweepAngle = (m_frameCount * 3) % 360;
    double sweepRad = qDegreesToRadians(sweepAngle);
    int sweepX = cx + static_cast<int>(maxRadius * std::sin(sweepRad));
    int sweepY = cy - static_cast<int>(maxRadius * std::cos(sweepRad));
    
    // Sweep fade effect
    painter.setPen(Qt::NoPen);
    for (int fade = 30; fade >= 0; fade -= 5) {
        double fadeAngle = sweepAngle - fade;
        double fadeRad = qDegreesToRadians(fadeAngle);
        int alpha = 255 - fade * 8;
        
        QPolygon sweep;
        sweep << QPoint(cx, cy);
        for (int r = 0; r <= maxRadius; r += 10) {
            int sx = cx + static_cast<int>(r * std::sin(fadeRad));
            int sy = cy - static_cast<int>(r * std::cos(fadeRad));
            sweep << QPoint(sx, sy);
        }
        painter.setBrush(QColor(0, 255, 0, alpha / 10));
        painter.drawPolygon(sweep);
    }
    
    painter.setPen(QPen(QColor(0, 255, 0), 2));
    painter.drawLine(cx, cy, sweepX, sweepY);
    
    // Draw targets
    if (m_targetVisible) {
        m_targetPhase += 0.02;
        
        // Primary target
        double targetRange = 0.6 + 0.1 * std::sin(m_targetPhase);
        double targetBearing = 45 + 20 * std::sin(m_targetPhase * 0.5);
        double targetRad = qDegreesToRadians(targetBearing);
        int targetX = cx + static_cast<int>(maxRadius * targetRange * std::sin(targetRad));
        int targetY = cy - static_cast<int>(maxRadius * targetRange * std::cos(targetRad));
        
        // Target blip
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 255, 0, 200));
        painter.drawEllipse(targetX - 6, targetY - 6, 12, 12);
        
        // Target trail
        painter.setPen(QPen(QColor(0, 200, 0, 100), 2));
        for (int t = 1; t <= 5; t++) {
            double trailPhase = m_targetPhase - t * 0.1;
            double trailRange = 0.6 + 0.1 * std::sin(trailPhase);
            double trailBearing = 45 + 20 * std::sin(trailPhase * 0.5);
            double trailRad = qDegreesToRadians(trailBearing);
            int trailX = cx + static_cast<int>(maxRadius * trailRange * std::sin(trailRad));
            int trailY = cy - static_cast<int>(maxRadius * trailRange * std::cos(trailRad));
            painter.drawPoint(trailX, trailY);
        }
        
        // Target label
        painter.setPen(QColor(0, 255, 0));
        painter.setFont(QFont("Courier", 9));
        painter.drawText(targetX + 10, targetY - 5, "TGT-001");
        painter.drawText(targetX + 10, targetY + 10, QString("%1m").arg(static_cast<int>(targetRange * 2000)));
    }
    
    // Draw telemetry
    painter.setPen(QColor(0, 200, 0));
    painter.setFont(QFont("Courier", 10));
    painter.drawText(10, 20, "RADAR DISPLAY");
    painter.drawText(10, 40, QDateTime::currentDateTime().toString("hh:mm:ss"));
    painter.drawText(width - 100, 20, QString("AZ: %1").arg(static_cast<int>(sweepAngle)));
    painter.drawText(10, height - 10, "RANGE: 2km");
    
    painter.end();
    
    return frame;
}

void SimulationVideoSource::drawOverlay(QImage& frame) {
    QPainter painter(&frame);
    drawTelemetry(painter, frame.size());
    painter.end();
}

void SimulationVideoSource::drawTarget(QPainter& painter, int x, int y) {
    // Target box
    painter.setPen(QPen(Qt::red, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(x - 30, y - 20, 60, 40);
    
    // Corner brackets
    painter.drawLine(x - 35, y - 25, x - 35, y - 15);
    painter.drawLine(x - 35, y - 25, x - 25, y - 25);
    
    painter.drawLine(x + 35, y - 25, x + 35, y - 15);
    painter.drawLine(x + 35, y - 25, x + 25, y - 25);
    
    painter.drawLine(x - 35, y + 25, x - 35, y + 15);
    painter.drawLine(x - 35, y + 25, x - 25, y + 25);
    
    painter.drawLine(x + 35, y + 25, x + 35, y + 15);
    painter.drawLine(x + 35, y + 25, x + 25, y + 25);
    
    // Cross on target
    painter.drawLine(x - 10, y, x + 10, y);
    painter.drawLine(x, y - 10, x, y + 10);
    
    // Target label
    painter.setFont(QFont("Courier", 9, QFont::Bold));
    painter.drawText(x + 40, y - 10, "TGT-001");
    painter.drawText(x + 40, y + 5, "UAS");
    
    // Simulated drone shape
    painter.setBrush(QColor(60, 60, 60));
    painter.setPen(QPen(QColor(40, 40, 40), 1));
    
    // Body
    painter.drawEllipse(x - 8, y - 4, 16, 8);
    
    // Arms and rotors
    painter.drawLine(x - 20, y - 12, x + 20, y + 12);
    painter.drawLine(x - 20, y + 12, x + 20, y - 12);
    
    // Draw rotor discs with animation effect
    painter.setBrush(QColor(80, 80, 80, 150));
    for (int i = 0; i < 4; i++) {
        int rx = x + ((i < 2) ? -18 : 18);
        int ry = y + ((i % 2 == 0) ? -10 : 10);
        painter.drawEllipse(rx - 8, ry - 3, 16, 6);
    }
}

void SimulationVideoSource::drawCrosshairs(QPainter& painter, int cx, int cy) {
    painter.setPen(QPen(Qt::green, 2));
    
    // Outer crosshairs
    painter.drawLine(cx - 50, cy, cx - 20, cy);
    painter.drawLine(cx + 20, cy, cx + 50, cy);
    painter.drawLine(cx, cy - 50, cx, cy - 20);
    painter.drawLine(cx, cy + 20, cx, cy + 50);
    
    // Center dot
    painter.setBrush(Qt::green);
    painter.drawEllipse(cx - 3, cy - 3, 6, 6);
    
    // Range circles
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::green, 1, Qt::DashLine));
    painter.drawEllipse(cx - 100, cy - 100, 200, 200);
    painter.drawEllipse(cx - 200, cy - 200, 400, 400);
}

void SimulationVideoSource::drawTelemetry(QPainter& painter, const QSize& size) {
    int w = size.width();
    int h = size.height();
    
    painter.setPen(Qt::white);
    painter.setFont(QFont("Courier", 10));
    
    // Top left - timestamp and camera info
    painter.drawText(10, 20, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    painter.drawText(10, 40, QString("CAM: %1").arg(m_cameraName));
    painter.drawText(10, 60, QString("FRAME: %1").arg(m_frameCount));
    
    // Top right - mode indicator
    QString modeStr;
    switch (m_scenarioType) {
        case 0: modeStr = "EO/IR"; break;
        case 1: modeStr = "THERMAL"; break;
        case 2: modeStr = "RADAR"; break;
        default: modeStr = "UNKNOWN"; break;
    }
    painter.drawText(w - 100, 20, modeStr);
    
    // Bottom - simulated PTZ info
    double azimuth = 180 + 30 * std::sin(m_targetPhase * 0.3);
    double elevation = 5 + 10 * std::cos(m_targetPhase * 0.2);
    double zoom = 1.0 + 0.5 * std::abs(std::sin(m_targetPhase * 0.1));
    
    painter.drawText(10, h - 30, QString("AZ: %1%2  EL: %3%4  ZOOM: %5x")
                                     .arg(azimuth, 0, 'f', 1)
                                     .arg(QChar(0x00B0))
                                     .arg(elevation, 0, 'f', 1)
                                     .arg(QChar(0x00B0))
                                     .arg(zoom, 0, 'f', 1));
    
    // Simulated GPS position
    painter.drawText(10, h - 10, QString("LAT: 34.0522N  LON: 118.2437W  ALT: 100m"));
    
    // Recording indicator
    if ((m_frameCount / 30) % 2 == 0) {
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        painter.drawEllipse(w - 30, h - 25, 12, 12);
        painter.drawText(w - 70, h - 15, "REC");
    }
    
    // Status bar
    painter.setPen(QColor(0, 255, 0));
    painter.drawText(w - 150, 40, "STATUS: TRACKING");
}

} // namespace CounterUAS
