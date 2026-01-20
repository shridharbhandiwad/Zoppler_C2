#include "ui/MapWidget.h"
#include "utils/CoordinateUtils.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QRandomGenerator>
#include <QLinearGradient>

namespace CounterUAS {

MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(400, 300);
    setMouseTracking(true);
    
    m_center.latitude = 34.0522;
    m_center.longitude = -118.2437;
    m_center.altitude = 0;
}

void MapWidget::setCenter(const GeoPosition& pos) {
    m_center = pos;
    update();
}

void MapWidget::setZoom(double zoom) {
    m_zoom = qBound(1.0, zoom, 20.0);
    emit zoomChanged(m_zoom);
    update();
}

void MapWidget::selectTrack(const QString& trackId) {
    m_selectedTrackId = trackId;
    update();
}

void MapWidget::addTrack(const QString& trackId) {
    Q_UNUSED(trackId)
    update();
}

void MapWidget::updateTrack(const QString& trackId) {
    Q_UNUSED(trackId)
    update();
}

void MapWidget::removeTrack(const QString& trackId) {
    m_tracks.remove(trackId);
    if (m_selectedTrackId == trackId) {
        m_selectedTrackId.clear();
    }
    update();
}

void MapWidget::clearTracks() {
    m_tracks.clear();
    m_selectedTrackId.clear();
    update();
}

void MapWidget::setTrackData(const QString& trackId, Track* track) {
    if (track) {
        m_tracks[trackId] = track;
        update();
    }
}

void MapWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw satellite-style map background
    drawSatelliteBackground(painter);
    
    // Draw grid overlay
    drawGrid(painter);
    
    // Draw defended area
    drawDefendedArea(painter);
    
    // Draw tracks (drones)
    drawTracks(painter);
    
    // Draw center crosshair
    painter.setPen(QPen(QColor(0, 212, 255), 2));
    int cx = width() / 2;
    int cy = height() / 2;
    painter.drawLine(cx - 15, cy, cx - 5, cy);
    painter.drawLine(cx + 5, cy, cx + 15, cy);
    painter.drawLine(cx, cy - 15, cx, cy - 5);
    painter.drawLine(cx, cy + 5, cx, cy + 15);
    
    // Draw scale bar
    drawScaleBar(painter);
    
    // Draw compass
    drawCompass(painter);
    
    // Draw coordinates overlay
    painter.setPen(Qt::white);
    painter.setFont(QFont("Consolas", 10));
    QString coordText = QString("LAT: %1  LON: %2  ZOOM: %3x")
                        .arg(m_center.latitude, 0, 'f', 4)
                        .arg(m_center.longitude, 0, 'f', 4)
                        .arg(m_zoom, 0, 'f', 1);
    
    // Semi-transparent background for text
    QFontMetrics fm(painter.font());
    QRect textRect = fm.boundingRect(coordText);
    textRect.moveTo(10, height() - 25);
    textRect.adjust(-5, -2, 10, 4);
    painter.fillRect(textRect, QColor(0, 0, 0, 150));
    painter.drawText(10, height() - 10, coordText);
}

void MapWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        GeoPosition clickPos = screenToGeo(event->pos());
        emit mapClicked(clickPos);
    }
}

void MapWidget::wheelEvent(QWheelEvent* event) {
    double delta = event->angleDelta().y() / 120.0;
    setZoom(m_zoom + delta * 0.5);
}

void MapWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
}

QPointF MapWidget::geoToScreen(const GeoPosition& pos) const {
    double scale = m_zoom * 5000.0;  // pixels per degree at this zoom
    
    double dx = (pos.longitude - m_center.longitude) * scale;
    double dy = -(pos.latitude - m_center.latitude) * scale;
    
    return QPointF(width() / 2.0 + dx, height() / 2.0 + dy);
}

GeoPosition MapWidget::screenToGeo(const QPointF& screen) const {
    double scale = m_zoom * 5000.0;
    
    double dx = (screen.x() - width() / 2.0) / scale;
    double dy = -(screen.y() - height() / 2.0) / scale;
    
    GeoPosition pos;
    pos.latitude = m_center.latitude + dy;
    pos.longitude = m_center.longitude + dx;
    pos.altitude = m_center.altitude;
    return pos;
}

void MapWidget::drawSatelliteBackground(QPainter& painter) {
    // Create satellite imagery style background with terrain features
    QLinearGradient skyGradient(0, 0, 0, height());
    skyGradient.setColorAt(0, QColor(15, 35, 25));    // Dark green-gray at top
    skyGradient.setColorAt(1, QColor(25, 45, 35));    // Slightly lighter at bottom
    painter.fillRect(rect(), skyGradient);
    
    // Draw terrain patches (simulated satellite view)
    auto* gen = QRandomGenerator::global();
    
    // Use a seed based on center position for consistent terrain
    int seedX = static_cast<int>(m_center.longitude * 1000) % 10000;
    int seedY = static_cast<int>(m_center.latitude * 1000) % 10000;
    
    // Draw varied terrain patches
    for (int i = 0; i < 50; ++i) {
        int px = (seedX + i * 137) % width();
        int py = (seedY + i * 211) % height();
        int size = 30 + (seedX + seedY + i) % 100;
        
        // Vary terrain colors (greens, browns, grays for satellite look)
        int colorType = (seedX + i) % 5;
        QColor terrainColor;
        switch (colorType) {
            case 0: terrainColor = QColor(35, 55, 35, 80); break;  // Forest
            case 1: terrainColor = QColor(45, 50, 40, 80); break;  // Field
            case 2: terrainColor = QColor(55, 55, 45, 80); break;  // Urban
            case 3: terrainColor = QColor(40, 45, 50, 80); break;  // Industrial
            case 4: terrainColor = QColor(30, 60, 40, 80); break;  // Park
        }
        
        painter.setPen(Qt::NoPen);
        painter.setBrush(terrainColor);
        painter.drawEllipse(px - size/2, py - size/2, size, size);
    }
    
    // Draw roads (linear features)
    painter.setPen(QPen(QColor(60, 60, 55, 100), 2));
    for (int i = 0; i < 8; ++i) {
        int x1 = (seedX * (i + 1)) % width();
        int y1 = (seedY * (i + 2)) % height();
        int x2 = (x1 + 200 + i * 50) % width();
        int y2 = (y1 + 100 + i * 30) % height();
        painter.drawLine(x1, y1, x2, y2);
    }
    
    // Draw water features (blue patches)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(25, 45, 65, 100));
    for (int i = 0; i < 3; ++i) {
        int px = (seedX + i * 500) % width();
        int py = (seedY + i * 400) % height();
        int w = 80 + (i * 40);
        int h = 40 + (i * 20);
        painter.drawEllipse(px, py, w, h);
    }
    
    // Draw buildings/structures (small rectangles in clusters)
    painter.setBrush(QColor(70, 70, 65, 120));
    for (int i = 0; i < 30; ++i) {
        int px = (seedX * 3 + i * 97) % width();
        int py = (seedY * 2 + i * 71) % height();
        int w = 8 + (i % 5) * 3;
        int h = 6 + (i % 4) * 2;
        painter.drawRect(px, py, w, h);
    }
}

void MapWidget::drawScaleBar(QPainter& painter) {
    // Draw scale bar in bottom-right corner
    int barWidth = 100;
    int barHeight = 4;
    int x = width() - barWidth - 20;
    int y = height() - 40;
    
    // Calculate distance represented by scale bar
    double metersPerPixel = 111000.0 / (m_zoom * 5000.0);  // Approximate
    double scaleMeters = barWidth * metersPerPixel;
    
    QString scaleText;
    if (scaleMeters >= 1000) {
        scaleText = QString("%1 km").arg(scaleMeters / 1000.0, 0, 'f', 1);
    } else {
        scaleText = QString("%1 m").arg(static_cast<int>(scaleMeters));
    }
    
    // Draw scale bar background
    painter.fillRect(x - 5, y - 5, barWidth + 10, barHeight + 25, QColor(0, 0, 0, 150));
    
    // Draw scale bar
    painter.setPen(QPen(Qt::white, 2));
    painter.drawLine(x, y, x + barWidth, y);
    painter.drawLine(x, y - 5, x, y + 5);
    painter.drawLine(x + barWidth, y - 5, x + barWidth, y + 5);
    
    // Draw scale text
    painter.setFont(QFont("Arial", 9));
    painter.drawText(x, y + 18, scaleText);
}

void MapWidget::drawCompass(QPainter& painter) {
    // Draw compass in top-right corner
    int cx = width() - 50;
    int cy = 50;
    int radius = 25;
    
    // Background circle
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawEllipse(QPoint(cx, cy), radius + 5, radius + 5);
    
    // Compass circle
    painter.setPen(QPen(QColor(0, 212, 255), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPoint(cx, cy), radius, radius);
    
    // North indicator (red triangle)
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 51, 68));
    QPolygonF northArrow;
    northArrow << QPointF(cx, cy - radius + 5)
               << QPointF(cx - 6, cy - 5)
               << QPointF(cx + 6, cy - 5);
    painter.drawPolygon(northArrow);
    
    // South indicator (white triangle)
    painter.setBrush(Qt::white);
    QPolygonF southArrow;
    southArrow << QPointF(cx, cy + radius - 5)
               << QPointF(cx - 6, cy + 5)
               << QPointF(cx + 6, cy + 5);
    painter.drawPolygon(southArrow);
    
    // N label
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(cx - 5, cy - radius - 8, "N");
}

void MapWidget::drawGrid(QPainter& painter) {
    painter.setPen(QPen(QColor(0, 212, 255, 40), 1));  // Cyan grid with transparency
    
    double gridSpacing = 0.01 / (m_zoom / 10.0);
    
    double startLat = std::floor(m_center.latitude / gridSpacing) * gridSpacing - gridSpacing * 5;
    double startLon = std::floor(m_center.longitude / gridSpacing) * gridSpacing - gridSpacing * 5;
    
    for (int i = 0; i < 10; ++i) {
        GeoPosition p1, p2;
        p1.latitude = startLat + i * gridSpacing;
        p1.longitude = startLon;
        p2.latitude = startLat + i * gridSpacing;
        p2.longitude = startLon + gridSpacing * 10;
        
        QPointF s1 = geoToScreen(p1);
        QPointF s2 = geoToScreen(p2);
        painter.drawLine(s1, s2);
    }
    
    for (int i = 0; i < 10; ++i) {
        GeoPosition p1, p2;
        p1.latitude = startLat;
        p1.longitude = startLon + i * gridSpacing;
        p2.latitude = startLat + gridSpacing * 10;
        p2.longitude = startLon + i * gridSpacing;
        
        QPointF s1 = geoToScreen(p1);
        QPointF s2 = geoToScreen(p2);
        painter.drawLine(s1, s2);
    }
}

void MapWidget::drawDefendedArea(QPainter& painter) {
    // Draw defended area circles
    QPointF centerPt = geoToScreen(m_center);
    
    // Critical zone (red)
    painter.setPen(QPen(QColor(255, 0, 0, 100), 2));
    painter.setBrush(QColor(255, 0, 0, 30));
    double criticalRadius = 500.0 * m_zoom * 0.01;  // Simplified
    painter.drawEllipse(centerPt, criticalRadius, criticalRadius);
    
    // Warning zone (yellow)
    painter.setPen(QPen(QColor(255, 255, 0, 100), 2));
    painter.setBrush(QColor(255, 255, 0, 20));
    double warningRadius = 1500.0 * m_zoom * 0.01;
    painter.drawEllipse(centerPt, warningRadius, warningRadius);
}

void MapWidget::drawTracks(QPainter& painter) {
    for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
        Track* track = it.value();
        if (!track || track->state() == TrackState::Dropped) continue;
        
        QPointF pos = geoToScreen(track->position());
        QColor color = colorForClassification(track->classification());
        
        // Draw track symbol
        bool selected = (it.key() == m_selectedTrackId);
        int size = selected ? 16 : 12;
        
        // Draw glow effect for hostile drones
        if (track->classification() == TrackClassification::Hostile) {
            painter.setPen(Qt::NoPen);
            QRadialGradient glow(pos.x(), pos.y(), size * 2);
            glow.setColorAt(0, QColor(255, 51, 68, 100));
            glow.setColorAt(1, QColor(255, 51, 68, 0));
            painter.setBrush(glow);
            painter.drawEllipse(pos, size * 2, size * 2);
        }
        
        // Draw drone symbol (quadcopter shape)
        painter.setPen(QPen(color, selected ? 3 : 2));
        painter.setBrush(QColor(color.red(), color.green(), color.blue(), 80));
        
        // Main body circle
        painter.drawEllipse(pos, size/2, size/2);
        
        // Draw 4 rotors (quadcopter)
        double rotorOffset = size * 0.7;
        double rotorSize = size * 0.35;
        
        // Front-left rotor
        painter.drawEllipse(QPointF(pos.x() - rotorOffset, pos.y() - rotorOffset), rotorSize, rotorSize);
        // Front-right rotor
        painter.drawEllipse(QPointF(pos.x() + rotorOffset, pos.y() - rotorOffset), rotorSize, rotorSize);
        // Back-left rotor
        painter.drawEllipse(QPointF(pos.x() - rotorOffset, pos.y() + rotorOffset), rotorSize, rotorSize);
        // Back-right rotor
        painter.drawEllipse(QPointF(pos.x() + rotorOffset, pos.y() + rotorOffset), rotorSize, rotorSize);
        
        // Draw arms connecting rotors to body
        painter.setPen(QPen(color, 2));
        painter.drawLine(pos, QPointF(pos.x() - rotorOffset, pos.y() - rotorOffset));
        painter.drawLine(pos, QPointF(pos.x() + rotorOffset, pos.y() - rotorOffset));
        painter.drawLine(pos, QPointF(pos.x() - rotorOffset, pos.y() + rotorOffset));
        painter.drawLine(pos, QPointF(pos.x() + rotorOffset, pos.y() + rotorOffset));
        
        // Draw velocity vector / heading indicator
        VelocityVector vel = track->velocity();
        double speed = vel.speed();
        if (speed > 1.0) {
            double heading = vel.heading();
            double vx = std::sin(qDegreesToRadians(heading)) * (size + 10);
            double vy = -std::cos(qDegreesToRadians(heading)) * (size + 10);
            
            painter.setPen(QPen(color, 2));
            painter.drawLine(pos, QPointF(pos.x() + vx, pos.y() + vy));
            
            // Draw arrowhead
            double arrowSize = 6;
            double arrowAngle = qDegreesToRadians(heading);
            QPointF arrowEnd(pos.x() + vx, pos.y() + vy);
            QPolygonF arrow;
            arrow << arrowEnd
                  << QPointF(arrowEnd.x() - arrowSize * std::sin(arrowAngle - 0.5),
                            arrowEnd.y() + arrowSize * std::cos(arrowAngle - 0.5))
                  << QPointF(arrowEnd.x() - arrowSize * std::sin(arrowAngle + 0.5),
                            arrowEnd.y() + arrowSize * std::cos(arrowAngle + 0.5));
            painter.setBrush(color);
            painter.drawPolygon(arrow);
        }
        
        // Draw track info label
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 9, QFont::Bold));
        
        // Info box background
        QString infoText = QString("%1 | ALT:%2m | SPD:%3m/s")
            .arg(track->trackId())
            .arg(static_cast<int>(track->position().altitude))
            .arg(speed, 0, 'f', 1);
        
        QFontMetrics fm(painter.font());
        QRect textRect = fm.boundingRect(infoText);
        textRect.moveTo(pos.x() + size + 8, pos.y() - 6);
        textRect.adjust(-3, -2, 6, 2);
        
        painter.fillRect(textRect, QColor(0, 0, 0, 180));
        painter.setPen(color);
        painter.drawRect(textRect);
        painter.setPen(Qt::white);
        painter.drawText(pos.x() + size + 8, pos.y() + 4, infoText);
        
        // Draw selection indicator
        if (selected) {
            painter.setPen(QPen(QColor(0, 212, 255), 2, Qt::DashLine));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(pos, size * 1.5, size * 1.5);
        }
    }
}

QColor MapWidget::colorForClassification(TrackClassification cls) const {
    switch (cls) {
        case TrackClassification::Hostile: return Qt::red;
        case TrackClassification::Friendly: return Qt::cyan;
        case TrackClassification::Pending: return Qt::yellow;
        case TrackClassification::Neutral: return Qt::gray;
        default: return Qt::green;
    }
}

} // namespace CounterUAS
