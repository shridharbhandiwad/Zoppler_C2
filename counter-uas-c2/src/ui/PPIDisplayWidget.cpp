#include "ui/PPIDisplayWidget.h"
#include "core/TrackManager.h"
#include "utils/CoordinateUtils.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QDateTime>
#include <QUrlQuery>

namespace CounterUAS {

PPIDisplayWidget::PPIDisplayWidget(QWidget* parent)
    : QWidget(parent)
    , m_sweepTimer(new QTimer(this))
    , m_historyTimer(new QTimer(this))
    , m_networkManager(new QNetworkAccessManager(this))
    , m_tileCache(100)  // Cache up to 100 tiles
{
    setMinimumSize(400, 400);
    setMouseTracking(true);
    
    // Default center (Los Angeles area)
    m_center.latitude = 34.0522;
    m_center.longitude = -118.2437;
    m_center.altitude = 0;
    
    // Initialize sweep trail
    m_sweepTrail.fill(0.0, SWEEP_TRAIL_LENGTH);
    
    // Sweep timer - update at 60 FPS
    connect(m_sweepTimer, &QTimer::timeout, this, &PPIDisplayWidget::updateSweep);
    m_sweepTimer->setInterval(16);  // ~60 FPS
    
    // History update timer
    connect(m_historyTimer, &QTimer::timeout, this, &PPIDisplayWidget::updateTrackHistory);
    m_historyTimer->setInterval(100);  // 10 Hz
    m_historyTimer->start();
    
    // Network manager for map tiles
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &PPIDisplayWidget::onMapTileReceived);
    
    // Default map tile URL (OpenStreetMap)
    m_mapTileUrlTemplate = "https://tile.openstreetmap.org/{z}/{x}/{y}.png";
}

PPIDisplayWidget::~PPIDisplayWidget() {
    stopSweep();
}

void PPIDisplayWidget::setCenter(const GeoPosition& pos) {
    m_center = pos;
    m_backgroundDirty = true;
    updateVisibleTiles();
    update();
}

void PPIDisplayWidget::setRangeScale(double rangeM) {
    m_rangeScaleM = qBound(100.0, rangeM, 50000.0);
    m_backgroundDirty = true;
    emit rangeScaleChanged(m_rangeScaleM);
    updateVisibleTiles();
    update();
}

void PPIDisplayWidget::setDisplayMode(PPIDisplayMode mode) {
    if (m_displayMode != mode) {
        m_displayMode = mode;
        m_backgroundDirty = true;
        
        // Adjust colors based on mode
        switch (mode) {
            case PPIDisplayMode::RadarOnly:
                m_backgroundColor = QColor(10, 20, 10);
                m_sweepColor = QColor(0, 255, 0, 200);
                m_rangeRingColor = QColor(0, 180, 0, 150);
                break;
            case PPIDisplayMode::MapOverlay:
                m_backgroundColor = QColor(30, 30, 30);
                m_sweepColor = QColor(0, 255, 0, 150);
                m_rangeRingColor = QColor(255, 255, 255, 150);
                break;
            case PPIDisplayMode::MapOnly:
                m_backgroundColor = QColor(240, 240, 240);
                m_sweepColor = QColor(0, 0, 0, 0);  // Hidden
                m_rangeRingColor = QColor(100, 100, 100, 100);
                break;
            case PPIDisplayMode::NightVision:
                m_backgroundColor = QColor(0, 10, 0);
                m_sweepColor = QColor(0, 200, 0, 220);
                m_rangeRingColor = QColor(0, 150, 0, 180);
                break;
        }
        
        emit displayModeChanged(mode);
        update();
    }
}

void PPIDisplayWidget::setSweepMode(PPISweepMode mode) {
    m_sweepMode = mode;
    if (mode == PPISweepMode::None) {
        stopSweep();
    }
    update();
}

void PPIDisplayWidget::setSweepSpeed(double degreesPerSecond) {
    m_sweepSpeed = qBound(1.0, degreesPerSecond, 180.0);
}

void PPIDisplayWidget::setSectorLimits(double startDeg, double endDeg) {
    m_sectorStart = fmod(startDeg + 360.0, 360.0);
    m_sectorEnd = fmod(endDeg + 360.0, 360.0);
}

void PPIDisplayWidget::setRangeRingCount(int count) {
    m_rangeRingCount = qBound(1, count, 20);
    m_backgroundDirty = true;
    update();
}

void PPIDisplayWidget::setShowRangeLabels(bool show) {
    m_showRangeLabels = show;
    update();
}

void PPIDisplayWidget::setAzimuthDivisions(int divisions) {
    m_azimuthDivisions = qBound(4, divisions, 36);
    m_backgroundDirty = true;
    update();
}

void PPIDisplayWidget::setShowAzimuthLabels(bool show) {
    m_showAzimuthLabels = show;
    update();
}

void PPIDisplayWidget::setTrackManager(TrackManager* manager) {
    if (m_trackManager) {
        disconnect(m_trackManager, nullptr, this, nullptr);
    }
    
    m_trackManager = manager;
    
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackCreated,
                this, &PPIDisplayWidget::addTrack);
        connect(m_trackManager, &TrackManager::trackUpdated,
                this, &PPIDisplayWidget::updateTrack);
        connect(m_trackManager, &TrackManager::trackDropped,
                this, &PPIDisplayWidget::removeTrack);
    }
}

void PPIDisplayWidget::selectTrack(const QString& trackId) {
    m_selectedTrackId = trackId;
    update();
}

void PPIDisplayWidget::setShowTrackHistory(bool show) {
    m_showTrackHistory = show;
    update();
}

void PPIDisplayWidget::setTrackHistoryLength(int seconds) {
    m_trackHistorySeconds = qBound(5, seconds, 300);
}

void PPIDisplayWidget::setDefendedAreaVisible(bool visible) {
    m_showDefendedArea = visible;
    update();
}

void PPIDisplayWidget::setDefendedAreaRadii(double criticalM, double warningM, double detectionM) {
    m_criticalRadiusM = criticalM;
    m_warningRadiusM = warningM;
    m_detectionRadiusM = detectionM;
    update();
}

void PPIDisplayWidget::setMapTileUrl(const QString& urlTemplate) {
    m_mapTileUrlTemplate = urlTemplate;
    m_tileCache.clear();
    updateVisibleTiles();
}

void PPIDisplayWidget::setMapZoomLevel(int zoom) {
    m_mapZoomLevel = qBound(1, zoom, 19);
    m_tileCache.clear();
    updateVisibleTiles();
}

void PPIDisplayWidget::setMapOpacity(double opacity) {
    m_mapOpacity = qBound(0.0, opacity, 1.0);
    update();
}

void PPIDisplayWidget::setBackgroundColor(const QColor& color) {
    m_backgroundColor = color;
    m_backgroundDirty = true;
    update();
}

void PPIDisplayWidget::setSweepColor(const QColor& color) {
    m_sweepColor = color;
    update();
}

void PPIDisplayWidget::setRangeRingColor(const QColor& color) {
    m_rangeRingColor = color;
    m_backgroundDirty = true;
    update();
}

void PPIDisplayWidget::setTrackColors(const QColor& hostile, const QColor& friendly,
                                       const QColor& unknown, const QColor& neutral) {
    m_hostileColor = hostile;
    m_friendlyColor = friendly;
    m_unknownColor = unknown;
    m_neutralColor = neutral;
    update();
}

void PPIDisplayWidget::setNorthUp(bool northUp) {
    m_northUp = northUp;
    update();
}

void PPIDisplayWidget::setHeading(double headingDeg) {
    m_heading = fmod(headingDeg + 360.0, 360.0);
    if (!m_northUp) {
        update();
    }
}

void PPIDisplayWidget::addTrack(const QString& trackId) {
    if (m_trackManager) {
        Track* track = m_trackManager->track(trackId);
        if (track) {
            m_tracks[trackId] = track;
            m_trackHistory[trackId] = QList<TrackHistoryPoint>();
        }
    }
    update();
}

void PPIDisplayWidget::updateTrack(const QString& trackId) {
    if (m_trackManager && m_tracks.contains(trackId)) {
        Track* track = m_tracks[trackId];
        if (track) {
            // Add to history
            QPointF ppiPos = geoToPPI(track->position());
            TrackHistoryPoint histPt;
            histPt.position = ppiPos;
            histPt.timestamp = QDateTime::currentMSecsSinceEpoch();
            histPt.intensity = 1.0;
            m_trackHistory[trackId].append(histPt);
            
            // Trim old history
            qint64 cutoffTime = histPt.timestamp - (m_trackHistorySeconds * 1000);
            while (!m_trackHistory[trackId].isEmpty() && 
                   m_trackHistory[trackId].first().timestamp < cutoffTime) {
                m_trackHistory[trackId].removeFirst();
            }
        }
    }
    update();
}

void PPIDisplayWidget::removeTrack(const QString& trackId) {
    m_tracks.remove(trackId);
    m_trackHistory.remove(trackId);
    if (m_selectedTrackId == trackId) {
        m_selectedTrackId.clear();
    }
    update();
}

void PPIDisplayWidget::clearTracks() {
    m_tracks.clear();
    m_trackHistory.clear();
    m_selectedTrackId.clear();
    update();
}

void PPIDisplayWidget::startSweep() {
    if (!m_sweepRunning && m_sweepMode != PPISweepMode::None) {
        m_sweepRunning = true;
        m_sweepTimer->start();
    }
}

void PPIDisplayWidget::stopSweep() {
    m_sweepRunning = false;
    m_sweepTimer->stop();
}

void PPIDisplayWidget::resetSweep() {
    m_sweepAngle = 0.0;
    m_sweepTrail.fill(0.0);
    update();
}

void PPIDisplayWidget::refresh() {
    m_backgroundDirty = true;
    update();
}

void PPIDisplayWidget::updateSweep() {
    // Calculate angle increment based on timer interval and sweep speed
    double angleIncrement = m_sweepSpeed * (m_sweepTimer->interval() / 1000.0);
    
    if (m_sweepMode == PPISweepMode::Rotating) {
        m_sweepAngle = fmod(m_sweepAngle + angleIncrement, 360.0);
    } else if (m_sweepMode == PPISweepMode::Sector) {
        m_sweepAngle += angleIncrement;
        if (m_sweepAngle >= m_sectorEnd) {
            m_sweepAngle = m_sectorStart;
        }
    }
    
    // Update sweep trail (persistence effect)
    for (int i = SWEEP_TRAIL_LENGTH - 1; i > 0; --i) {
        m_sweepTrail[i] = m_sweepTrail[i - 1] * 0.95;  // Decay
    }
    m_sweepTrail[0] = m_sweepAngle;
    
    emit sweepAngleChanged(m_sweepAngle);
    update();
}

void PPIDisplayWidget::updateTrackHistory() {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 cutoffTime = currentTime - (m_trackHistorySeconds * 1000);
    
    // Update intensity for fading effect
    for (auto it = m_trackHistory.begin(); it != m_trackHistory.end(); ++it) {
        QList<TrackHistoryPoint>& history = it.value();
        for (TrackHistoryPoint& pt : history) {
            double age = (currentTime - pt.timestamp) / 1000.0;
            pt.intensity = qMax(0.0, 1.0 - (age / m_trackHistorySeconds));
        }
        
        // Remove old points
        while (!history.isEmpty() && history.first().timestamp < cutoffTime) {
            history.removeFirst();
        }
    }
}

void PPIDisplayWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw layers in order
    drawBackground(painter);
    
    if (m_displayMode == PPIDisplayMode::MapOverlay || 
        m_displayMode == PPIDisplayMode::MapOnly) {
        drawMapTiles(painter);
    }
    
    drawRangeRings(painter);
    drawAzimuthLines(painter);
    drawDefendedArea(painter);
    
    if (m_sweepMode != PPISweepMode::None && m_displayMode != PPIDisplayMode::MapOnly) {
        drawSweepTrail(painter);
        drawSweep(painter);
    }
    
    drawTracks(painter);
    drawNorthIndicator(painter);
    drawCompassRose(painter);
    drawScaleInfo(painter);
}

void PPIDisplayWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QString trackId = findTrackAtPoint(event->pos());
        if (!trackId.isEmpty()) {
            m_selectedTrackId = trackId;
            emit trackSelected(trackId);
            update();
        } else {
            // Convert to geo position
            QPointF ppiPos = event->pos() - screenCenter();
            GeoPosition clickPos = ppiToGeo(ppiPos);
            emit mapClicked(clickPos);
        }
    }
}

void PPIDisplayWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QString trackId = findTrackAtPoint(event->pos());
        if (!trackId.isEmpty()) {
            emit trackDoubleClicked(trackId);
        }
    }
}

void PPIDisplayWidget::wheelEvent(QWheelEvent* event) {
    double delta = event->angleDelta().y() / 120.0;
    double scaleFactor = (delta > 0) ? 0.8 : 1.25;
    setRangeScale(m_rangeScaleM * scaleFactor);
}

void PPIDisplayWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    m_backgroundDirty = true;
    updateVisibleTiles();
}

void PPIDisplayWidget::drawBackground(QPainter& painter) {
    // Fill with background color
    painter.fillRect(rect(), m_backgroundColor);
    
    // Draw circular display area mask
    QPointF center = screenCenter();
    double radius = ppiRadius();
    
    // Create gradient for radar glow effect
    if (m_displayMode == PPIDisplayMode::RadarOnly || 
        m_displayMode == PPIDisplayMode::NightVision) {
        QRadialGradient gradient(center, radius);
        gradient.setColorAt(0.0, m_backgroundColor.lighter(120));
        gradient.setColorAt(0.7, m_backgroundColor);
        gradient.setColorAt(1.0, m_backgroundColor.darker(120));
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(center, radius, radius);
    }
    
    // Draw circular border
    painter.setPen(QPen(m_rangeRingColor.lighter(150), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, radius, radius);
}

void PPIDisplayWidget::drawMapTiles(QPainter& painter) {
    if (m_mapTileUrlTemplate.isEmpty()) return;
    
    painter.setOpacity(m_mapOpacity);
    
    // Calculate visible tile range
    QPointF center = screenCenter();
    double radius = ppiRadius();
    
    // Get corner positions
    double mPerPixel = m_rangeScaleM / radius;
    
    for (int dx = -2; dx <= 2; ++dx) {
        for (int dy = -2; dy <= 2; ++dy) {
            GeoPosition tileCenter;
            tileCenter.latitude = m_center.latitude + (dy * 256 * mPerPixel / CoordinateUtils::DEG_TO_M_LAT);
            tileCenter.longitude = m_center.longitude + (dx * 256 * mPerPixel / CoordinateUtils::degToMeterLon(m_center.latitude));
            
            MapTileKey key = geoToTile(tileCenter, m_mapZoomLevel);
            
            QPixmap* tile = m_tileCache.object(key);
            if (tile) {
                // Calculate tile screen position
                double tileN = pow(2.0, m_mapZoomLevel);
                double tileLatRad = atan(sinh(M_PI * (1 - 2 * key.y / tileN)));
                double tileLat = qRadiansToDegrees(tileLatRad);
                double tileLon = key.x / tileN * 360.0 - 180.0;
                
                GeoPosition tilePos;
                tilePos.latitude = tileLat;
                tilePos.longitude = tileLon;
                tilePos.altitude = 0;
                
                QPointF screenPos = center + geoToPPI(tilePos);
                
                // Scale tile to match current view
                double tileSize = 256.0 / mPerPixel * (CoordinateUtils::DEG_TO_M_LAT / pow(2.0, m_mapZoomLevel) * 360.0 / 256.0);
                
                painter.drawPixmap(QRectF(screenPos.x(), screenPos.y(), tileSize, tileSize), *tile, tile->rect());
            } else if (!m_pendingTiles.contains(key)) {
                requestMapTile(key.x, key.y, m_mapZoomLevel);
            }
        }
    }
    
    painter.setOpacity(1.0);
}

void PPIDisplayWidget::drawRangeRings(QPainter& painter) {
    QPointF center = screenCenter();
    double radius = ppiRadius();
    double ringSpacing = radius / m_rangeRingCount;
    double rangePerRing = m_rangeScaleM / m_rangeRingCount;
    
    painter.setPen(QPen(m_rangeRingColor, 1, Qt::DashLine));
    
    QFont labelFont = painter.font();
    labelFont.setPointSize(9);
    painter.setFont(labelFont);
    
    for (int i = 1; i <= m_rangeRingCount; ++i) {
        double ringRadius = ringSpacing * i;
        painter.drawEllipse(center, ringRadius, ringRadius);
        
        if (m_showRangeLabels) {
            double rangeKm = (rangePerRing * i) / 1000.0;
            QString label;
            if (rangeKm >= 1.0) {
                label = QString("%1 km").arg(rangeKm, 0, 'f', 1);
            } else {
                label = QString("%1 m").arg(rangePerRing * i, 0, 'f', 0);
            }
            
            painter.setPen(m_rangeRingColor.lighter(150));
            painter.drawText(QPointF(center.x() + 5, center.y() - ringRadius + 12), label);
            painter.setPen(QPen(m_rangeRingColor, 1, Qt::DashLine));
        }
    }
}

void PPIDisplayWidget::drawAzimuthLines(QPainter& painter) {
    QPointF center = screenCenter();
    double radius = ppiRadius();
    double angleStep = 360.0 / m_azimuthDivisions;
    
    painter.setPen(QPen(m_azimuthLineColor, 1));
    
    QFont labelFont = painter.font();
    labelFont.setPointSize(9);
    painter.setFont(labelFont);
    
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    
    for (int i = 0; i < m_azimuthDivisions; ++i) {
        double angle = i * angleStep + rotationOffset;
        double angleRad = qDegreesToRadians(angle - 90.0);  // -90 to make 0 deg at top
        
        double x1 = center.x() + cos(angleRad) * 20;
        double y1 = center.y() + sin(angleRad) * 20;
        double x2 = center.x() + cos(angleRad) * radius;
        double y2 = center.y() + sin(angleRad) * radius;
        
        painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
        
        if (m_showAzimuthLabels) {
            double labelAngle = i * angleStep;
            QString label = QString("%1°").arg(static_cast<int>(labelAngle));
            
            double labelX = center.x() + cos(angleRad) * (radius + 15);
            double labelY = center.y() + sin(angleRad) * (radius + 15);
            
            painter.setPen(m_rangeRingColor.lighter(150));
            QFontMetrics fm(labelFont);
            QRect textRect = fm.boundingRect(label);
            painter.drawText(QPointF(labelX - textRect.width() / 2, labelY + textRect.height() / 4), label);
            painter.setPen(QPen(m_azimuthLineColor, 1));
        }
    }
}

void PPIDisplayWidget::drawSweep(QPainter& painter) {
    QPointF center = screenCenter();
    double radius = ppiRadius();
    
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    double angleRad = qDegreesToRadians(m_sweepAngle + rotationOffset - 90.0);
    
    double x = center.x() + cos(angleRad) * radius;
    double y = center.y() + sin(angleRad) * radius;
    
    // Draw sweep line
    painter.setPen(QPen(m_sweepColor, 2));
    painter.drawLine(center, QPointF(x, y));
    
    // Draw bright tip
    painter.setPen(QPen(m_sweepColor.lighter(150), 4));
    painter.drawPoint(QPointF(x, y));
}

void PPIDisplayWidget::drawSweepTrail(QPainter& painter) {
    QPointF center = screenCenter();
    double radius = ppiRadius();
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    
    // Draw fade trail behind sweep
    for (int i = 1; i < SWEEP_TRAIL_LENGTH; ++i) {
        double trailAngle = m_sweepAngle - (i * 0.5);  // Trail behind sweep
        double angleRad = qDegreesToRadians(trailAngle + rotationOffset - 90.0);
        
        double intensity = 1.0 - (static_cast<double>(i) / SWEEP_TRAIL_LENGTH);
        QColor trailColor = m_sweepColor;
        trailColor.setAlphaF(intensity * 0.3);
        
        double x = center.x() + cos(angleRad) * radius;
        double y = center.y() + sin(angleRad) * radius;
        
        painter.setPen(QPen(trailColor, 1));
        painter.drawLine(center, QPointF(x, y));
    }
}

void PPIDisplayWidget::drawDefendedArea(QPainter& painter) {
    if (!m_showDefendedArea) return;
    
    QPointF center = screenCenter();
    double scale = ppiRadius() / m_rangeScaleM;
    
    // Critical zone (red)
    double criticalRadius = m_criticalRadiusM * scale;
    painter.setPen(QPen(QColor(255, 0, 0, 150), 2));
    painter.setBrush(QColor(255, 0, 0, 40));
    painter.drawEllipse(center, criticalRadius, criticalRadius);
    
    // Warning zone (yellow)
    double warningRadius = m_warningRadiusM * scale;
    painter.setPen(QPen(QColor(255, 200, 0, 120), 2, Qt::DashLine));
    painter.setBrush(QColor(255, 200, 0, 20));
    painter.drawEllipse(center, warningRadius, warningRadius);
    
    // Detection zone (outer boundary)
    double detectionRadius = qMin(m_detectionRadiusM * scale, ppiRadius() * 0.95);
    painter.setPen(QPen(QColor(100, 200, 255, 100), 1, Qt::DotLine));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, detectionRadius, detectionRadius);
}

void PPIDisplayWidget::drawTracks(QPainter& painter) {
    QPointF center = screenCenter();
    
    for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
        Track* track = it.value();
        if (!track || track->state() == TrackState::Dropped) continue;
        
        QPointF ppiPos = geoToPPI(track->position());
        QPointF screenPos = center + ppiPos;
        
        // Check if track is within display range
        double distance = QLineF(center, screenPos).length();
        if (distance > ppiRadius()) continue;
        
        bool selected = (it.key() == m_selectedTrackId);
        
        // Draw track history trail
        if (m_showTrackHistory) {
            drawTrackHistory(painter, track);
        }
        
        // Draw velocity vector
        drawVelocityVector(painter, track, screenPos);
        
        // Draw track symbol
        drawTrackSymbol(painter, track, screenPos, selected);
        
        // Draw label
        drawTrackLabel(painter, track, screenPos);
    }
}

void PPIDisplayWidget::drawTrackSymbol(QPainter& painter, Track* track, const QPointF& pos, bool selected) {
    QColor color = colorForClassification(track->classification());
    
    // Threat level affects intensity
    if (track->threatLevel() >= 4) {
        color = color.lighter(130);
    }
    
    int size = selected ? 14 : 10;
    int penWidth = selected ? 3 : 2;
    
    painter.setPen(QPen(color, penWidth));
    painter.setBrush(Qt::NoBrush);
    
    // Draw symbol based on classification
    switch (track->classification()) {
        case TrackClassification::Hostile:
            // Filled diamond for hostile
            {
                QPolygonF diamond;
                diamond << QPointF(pos.x(), pos.y() - size)
                       << QPointF(pos.x() + size, pos.y())
                       << QPointF(pos.x(), pos.y() + size)
                       << QPointF(pos.x() - size, pos.y());
                painter.setBrush(QColor(color.red(), color.green(), color.blue(), 100));
                painter.drawPolygon(diamond);
            }
            break;
            
        case TrackClassification::Friendly:
            // Circle for friendly
            painter.drawEllipse(pos, size, size);
            break;
            
        case TrackClassification::Pending:
            // Question mark shape (circle with ?)
            painter.drawEllipse(pos, size, size);
            painter.setPen(color);
            painter.drawText(QRectF(pos.x() - size, pos.y() - size, size * 2, size * 2),
                            Qt::AlignCenter, "?");
            break;
            
        default:
            // Square for unknown/neutral
            painter.drawRect(QRectF(pos.x() - size, pos.y() - size, size * 2, size * 2));
            break;
    }
    
    // Selection highlight
    if (selected) {
        painter.setPen(QPen(Qt::white, 1, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(pos, size + 5, size + 5);
    }
    
    // Engagement indicator
    if (track->isEngaged()) {
        painter.setPen(QPen(Qt::red, 2));
        painter.drawEllipse(pos, size + 8, size + 8);
    }
}

void PPIDisplayWidget::drawTrackHistory(QPainter& painter, Track* track) {
    QString trackId = track->trackId();
    if (!m_trackHistory.contains(trackId)) return;
    
    const QList<TrackHistoryPoint>& history = m_trackHistory[trackId];
    if (history.size() < 2) return;
    
    QColor color = colorForClassification(track->classification());
    QPointF center = screenCenter();
    
    for (int i = 1; i < history.size(); ++i) {
        const TrackHistoryPoint& pt1 = history[i - 1];
        const TrackHistoryPoint& pt2 = history[i];
        
        QColor lineColor = color;
        lineColor.setAlphaF(pt1.intensity * 0.5);
        
        painter.setPen(QPen(lineColor, 1));
        painter.drawLine(center + pt1.position, center + pt2.position);
    }
    
    // Draw dots at history points
    for (const TrackHistoryPoint& pt : history) {
        QColor dotColor = color;
        dotColor.setAlphaF(pt.intensity * 0.7);
        painter.setPen(Qt::NoPen);
        painter.setBrush(dotColor);
        painter.drawEllipse(center + pt.position, 2, 2);
    }
}

void PPIDisplayWidget::drawTrackLabel(QPainter& painter, Track* track, const QPointF& pos) {
    QColor color = colorForClassification(track->classification());
    
    QString label = track->trackId();
    
    // Add speed if available
    VelocityVector vel = track->velocity();
    double speed = vel.speed();
    if (speed > 1.0) {
        label += QString(" %1m/s").arg(speed, 0, 'f', 0);
    }
    
    // Add altitude
    double alt = track->position().altitude;
    if (alt > 0) {
        label += QString(" %1m").arg(alt, 0, 'f', 0);
    }
    
    painter.setPen(color.lighter(120));
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    
    painter.drawText(QPointF(pos.x() + 15, pos.y() + 5), label);
}

void PPIDisplayWidget::drawVelocityVector(QPainter& painter, Track* track, const QPointF& pos) {
    VelocityVector vel = track->velocity();
    double speed = vel.speed();
    
    if (speed < 1.0) return;
    
    double heading = vel.heading();
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    double angleRad = qDegreesToRadians(heading + rotationOffset - 90.0);
    
    // Scale velocity vector (max 30 pixels)
    double vectorLength = qMin(speed * 0.5, 30.0);
    
    double vx = cos(angleRad) * vectorLength;
    double vy = sin(angleRad) * vectorLength;
    
    QColor color = colorForClassification(track->classification());
    painter.setPen(QPen(color, 2));
    painter.drawLine(pos, QPointF(pos.x() + vx, pos.y() + vy));
    
    // Draw arrowhead
    double arrowSize = 6;
    double arrowAngle = M_PI / 6;
    
    double ax1 = pos.x() + vx - arrowSize * cos(angleRad - arrowAngle);
    double ay1 = pos.y() + vy - arrowSize * sin(angleRad - arrowAngle);
    double ax2 = pos.x() + vx - arrowSize * cos(angleRad + arrowAngle);
    double ay2 = pos.y() + vy - arrowSize * sin(angleRad + arrowAngle);
    
    QPolygonF arrow;
    arrow << QPointF(pos.x() + vx, pos.y() + vy)
          << QPointF(ax1, ay1)
          << QPointF(ax2, ay2);
    painter.setBrush(color);
    painter.drawPolygon(arrow);
}

void PPIDisplayWidget::drawNorthIndicator(QPainter& painter) {
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    double angleRad = qDegreesToRadians(rotationOffset - 90.0);  // North at top
    
    QPointF center = screenCenter();
    double indicatorRadius = ppiRadius() + 25;
    
    double nx = center.x() + cos(angleRad) * indicatorRadius;
    double ny = center.y() + sin(angleRad) * indicatorRadius;
    
    // Draw N indicator
    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(Qt::NoBrush);
    
    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    painter.setFont(font);
    
    painter.drawText(QPointF(nx - 6, ny + 5), "N");
    
    // Draw small triangle pointing north
    QPolygonF arrow;
    arrow << QPointF(nx, ny + 10)
          << QPointF(nx - 5, ny + 18)
          << QPointF(nx + 5, ny + 18);
    painter.setBrush(Qt::white);
    painter.drawPolygon(arrow);
}

void PPIDisplayWidget::drawCompassRose(QPainter& painter) {
    // Draw small compass rose in corner
    QPointF roseCenter(width() - 50, height() - 50);
    double roseRadius = 30;
    
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    
    painter.setPen(QPen(Qt::white, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(roseCenter, roseRadius, roseRadius);
    
    // Cardinal directions
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    
    QString dirs[] = {"N", "E", "S", "W"};
    for (int i = 0; i < 4; ++i) {
        double angle = i * 90.0 + rotationOffset;
        double angleRad = qDegreesToRadians(angle - 90.0);
        
        double x = roseCenter.x() + cos(angleRad) * (roseRadius - 10);
        double y = roseCenter.y() + sin(angleRad) * (roseRadius - 10);
        
        painter.drawText(QPointF(x - 4, y + 4), dirs[i]);
        
        // Draw tick marks
        double tx1 = roseCenter.x() + cos(angleRad) * (roseRadius - 5);
        double ty1 = roseCenter.y() + sin(angleRad) * (roseRadius - 5);
        double tx2 = roseCenter.x() + cos(angleRad) * roseRadius;
        double ty2 = roseCenter.y() + sin(angleRad) * roseRadius;
        painter.drawLine(QPointF(tx1, ty1), QPointF(tx2, ty2));
    }
    
    // Draw heading if not north-up
    if (!m_northUp) {
        painter.setPen(Qt::yellow);
        painter.drawText(QPointF(roseCenter.x() - 15, roseCenter.y() + roseRadius + 15),
                        QString("HDG: %1°").arg(static_cast<int>(m_heading)));
    }
}

void PPIDisplayWidget::drawScaleInfo(QPainter& painter) {
    // Draw info panel in bottom-left corner
    int padding = 10;
    int lineHeight = 16;
    
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);
    
    QString rangeStr;
    if (m_rangeScaleM >= 1000) {
        rangeStr = QString("Range: %1 km").arg(m_rangeScaleM / 1000.0, 0, 'f', 1);
    } else {
        rangeStr = QString("Range: %1 m").arg(m_rangeScaleM, 0, 'f', 0);
    }
    
    QString posStr = QString("Lat: %1  Lon: %2")
                         .arg(m_center.latitude, 0, 'f', 4)
                         .arg(m_center.longitude, 0, 'f', 4);
    
    QString modeStr;
    switch (m_displayMode) {
        case PPIDisplayMode::RadarOnly: modeStr = "Mode: Radar"; break;
        case PPIDisplayMode::MapOverlay: modeStr = "Mode: Map+Radar"; break;
        case PPIDisplayMode::MapOnly: modeStr = "Mode: Map"; break;
        case PPIDisplayMode::NightVision: modeStr = "Mode: Night Vision"; break;
    }
    
    QString sweepStr = QString("Sweep: %1°").arg(static_cast<int>(m_sweepAngle));
    
    int trackCount = 0;
    for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
        if (it.value() && it.value()->state() != TrackState::Dropped) {
            ++trackCount;
        }
    }
    QString trackStr = QString("Tracks: %1").arg(trackCount);
    
    // Background for readability
    QRect infoBg(padding, height() - padding - lineHeight * 5, 180, lineHeight * 5);
    painter.fillRect(infoBg, QColor(0, 0, 0, 150));
    
    int y = height() - padding - lineHeight * 4;
    painter.drawText(padding + 5, y, rangeStr);
    painter.drawText(padding + 5, y + lineHeight, posStr);
    painter.drawText(padding + 5, y + lineHeight * 2, modeStr);
    painter.drawText(padding + 5, y + lineHeight * 3, sweepStr);
    painter.drawText(padding + 5, y + lineHeight * 4, trackStr);
}

void PPIDisplayWidget::requestMapTile(int x, int y, int zoom) {
    MapTileKey key{x, y, zoom};
    if (m_pendingTiles.contains(key)) return;
    
    m_pendingTiles.insert(key);
    
    QString url = getTileUrl(x, y, zoom);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::User, QVariant::fromValue(key));
    request.setHeader(QNetworkRequest::UserAgentHeader, "CounterUAS-C2/1.0");
    
    m_networkManager->get(request);
}

QString PPIDisplayWidget::getTileUrl(int x, int y, int zoom) const {
    QString url = m_mapTileUrlTemplate;
    url.replace("{x}", QString::number(x));
    url.replace("{y}", QString::number(y));
    url.replace("{z}", QString::number(zoom));
    return url;
}

MapTileKey PPIDisplayWidget::geoToTile(const GeoPosition& pos, int zoom) const {
    double n = pow(2.0, zoom);
    double latRad = qDegreesToRadians(pos.latitude);
    
    int x = static_cast<int>((pos.longitude + 180.0) / 360.0 * n);
    int y = static_cast<int>((1.0 - asinh(tan(latRad)) / M_PI) / 2.0 * n);
    
    return {x, y, zoom};
}

void PPIDisplayWidget::onMapTileReceived(QNetworkReply* reply) {
    reply->deleteLater();
    
    QVariant keyVar = reply->request().attribute(QNetworkRequest::User);
    MapTileKey key = keyVar.value<MapTileKey>();
    m_pendingTiles.remove(key);
    
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }
    
    QByteArray data = reply->readAll();
    QPixmap* tile = new QPixmap();
    if (tile->loadFromData(data)) {
        m_tileCache.insert(key, tile);
        update();
    } else {
        delete tile;
    }
}

void PPIDisplayWidget::updateVisibleTiles() {
    if (m_displayMode != PPIDisplayMode::MapOverlay && 
        m_displayMode != PPIDisplayMode::MapOnly) {
        return;
    }
    
    // Pre-fetch visible tiles
    for (int dx = -2; dx <= 2; ++dx) {
        for (int dy = -2; dy <= 2; ++dy) {
            MapTileKey key = geoToTile(m_center, m_mapZoomLevel);
            key.x += dx;
            key.y += dy;
            
            if (!m_tileCache.contains(key) && !m_pendingTiles.contains(key)) {
                requestMapTile(key.x, key.y, key.zoom);
            }
        }
    }
}

QPointF PPIDisplayWidget::screenCenter() const {
    return QPointF(width() / 2.0, height() / 2.0);
}

double PPIDisplayWidget::ppiRadius() const {
    return qMin(width(), height()) / 2.0 - 40;  // Leave margin for labels
}

QPointF PPIDisplayWidget::geoToPPI(const GeoPosition& pos) const {
    // Calculate distance and bearing from center
    double distance = CoordinateUtils::haversineDistance(m_center, pos);
    double bearing = CoordinateUtils::bearing(m_center, pos);
    
    // Convert to PPI coordinates (polar to cartesian)
    double scale = ppiRadius() / m_rangeScaleM;
    double ppiRange = distance * scale;
    
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    double angleRad = qDegreesToRadians(bearing + rotationOffset - 90.0);  // -90 for top=north
    
    double x = ppiRange * cos(angleRad);
    double y = ppiRange * sin(angleRad);
    
    return QPointF(x, y);
}

GeoPosition PPIDisplayWidget::ppiToGeo(const QPointF& ppiPos) const {
    double ppiRange = sqrt(ppiPos.x() * ppiPos.x() + ppiPos.y() * ppiPos.y());
    double angleRad = atan2(ppiPos.y(), ppiPos.x());
    
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    double bearing = qRadiansToDegrees(angleRad) + 90.0 - rotationOffset;  // +90 to convert back
    
    double scale = ppiRadius() / m_rangeScaleM;
    double distance = ppiRange / scale;
    
    return CoordinateUtils::positionFromBearingDistance(m_center, bearing, distance);
}

QPointF PPIDisplayWidget::polarToScreen(double rangeM, double azimuthDeg) const {
    double scale = ppiRadius() / m_rangeScaleM;
    double ppiRange = rangeM * scale;
    
    double rotationOffset = m_northUp ? 0.0 : -m_heading;
    double angleRad = qDegreesToRadians(azimuthDeg + rotationOffset - 90.0);
    
    QPointF center = screenCenter();
    double x = center.x() + ppiRange * cos(angleRad);
    double y = center.y() + ppiRange * sin(angleRad);
    
    return QPointF(x, y);
}

QColor PPIDisplayWidget::colorForClassification(TrackClassification cls) const {
    switch (cls) {
        case TrackClassification::Hostile: return m_hostileColor;
        case TrackClassification::Friendly: return m_friendlyColor;
        case TrackClassification::Pending: return m_unknownColor;
        case TrackClassification::Neutral: return m_neutralColor;
        default: return m_unknownColor;
    }
}

QColor PPIDisplayWidget::colorForThreatLevel(int level) const {
    switch (level) {
        case 5: return Qt::red;
        case 4: return QColor(255, 128, 0);  // Orange
        case 3: return Qt::yellow;
        case 2: return QColor(144, 238, 144);  // Light green
        default: return Qt::green;
    }
}

QString PPIDisplayWidget::findTrackAtPoint(const QPointF& point) const {
    QPointF center = screenCenter();
    
    for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
        Track* track = it.value();
        if (!track || track->state() == TrackState::Dropped) continue;
        
        QPointF ppiPos = geoToPPI(track->position());
        QPointF screenPos = center + ppiPos;
        
        double distance = QLineF(point, screenPos).length();
        if (distance < 15) {  // 15 pixel hit radius
            return it.key();
        }
    }
    
    return QString();
}

} // namespace CounterUAS
