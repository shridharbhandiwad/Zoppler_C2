#include "video/VideoOverlayRenderer.h"
#include <QDateTime>

namespace CounterUAS {

VideoOverlayRenderer::VideoOverlayRenderer(QObject* parent)
    : QObject(parent)
{
}

void VideoOverlayRenderer::setStyle(const OverlayStyle& style) {
    m_style = style;
}

void VideoOverlayRenderer::setTelemetry(const OverlayTelemetry& telemetry) {
    m_telemetry = telemetry;
}

void VideoOverlayRenderer::setTrackOverlays(const QList<TrackOverlay>& tracks) {
    m_tracks = tracks;
}

void VideoOverlayRenderer::addTrackOverlay(const TrackOverlay& track) {
    // Remove existing if present
    removeTrackOverlay(track.trackId);
    m_tracks.append(track);
}

void VideoOverlayRenderer::removeTrackOverlay(const QString& trackId) {
    m_tracks.erase(std::remove_if(m_tracks.begin(), m_tracks.end(),
        [&trackId](const TrackOverlay& t) { return t.trackId == trackId; }), m_tracks.end());
}

void VideoOverlayRenderer::clearTrackOverlays() {
    m_tracks.clear();
}

void VideoOverlayRenderer::setSelectedTrack(const QString& trackId) {
    m_selectedTrackId = trackId;
}

void VideoOverlayRenderer::setDesignationPoint(const QPointF& point) {
    m_designationPoint = point;
    m_hasDesignation = true;
}

void VideoOverlayRenderer::clearDesignationPoint() {
    m_hasDesignation = false;
}

QImage VideoOverlayRenderer::renderOverlay(const QImage& frame) {
    QImage result = frame.copy();
    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    
    renderOverlay(&painter, frame.size());
    
    painter.end();
    return result;
}

void VideoOverlayRenderer::renderOverlay(QPainter* painter, const QSize& frameSize) {
    // Draw track boxes
    for (const TrackOverlay& track : m_tracks) {
        drawTrackBox(painter, track, frameSize);
        drawTrackLabel(painter, track, frameSize);
    }
    
    // Draw designation crosshairs
    if (m_hasDesignation) {
        drawCrosshairs(painter, m_designationPoint, frameSize);
    }
    
    // Draw telemetry bar
    if (m_style.showCameraInfo) {
        drawTelemetryBar(painter, frameSize);
    }
    
    // Draw recording indicator
    if (m_style.showRecordingIndicator && m_telemetry.recording) {
        drawRecordingIndicator(painter, frameSize);
    }
    
    // Draw timestamp
    if (m_style.showTimestamp) {
        drawTimestamp(painter, frameSize);
    }
}

void VideoOverlayRenderer::drawTrackBox(QPainter* painter, const TrackOverlay& track, 
                                         const QSize& frameSize) {
    if (!track.boundingBox.isValid()) return;
    
    // Convert normalized coordinates to pixel coordinates
    int x = static_cast<int>(track.boundingBox.x);
    int y = static_cast<int>(track.boundingBox.y);
    int w = static_cast<int>(track.boundingBox.width);
    int h = static_cast<int>(track.boundingBox.height);
    
    QRect rect(x, y, w, h);
    
    // Choose color based on classification
    QColor boxColor = colorForClassification(track.classification);
    
    // Highlight selected track
    int lineWidth = m_style.boxLineWidth;
    if (track.trackId == m_selectedTrackId) {
        lineWidth = m_style.boxLineWidth + 2;
    }
    
    // Draw box
    QPen pen(boxColor, lineWidth);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(rect);
    
    // Draw engaged indicator
    if (track.isEngaged) {
        painter->setPen(QPen(Qt::red, 1, Qt::DashLine));
        painter->drawRect(rect.adjusted(-5, -5, 5, 5));
    }
    
    // Draw corner markers for selected track
    if (track.trackId == m_selectedTrackId) {
        int cornerLen = 10;
        painter->setPen(QPen(Qt::white, 2));
        
        // Top-left corner
        painter->drawLine(rect.topLeft(), rect.topLeft() + QPoint(cornerLen, 0));
        painter->drawLine(rect.topLeft(), rect.topLeft() + QPoint(0, cornerLen));
        
        // Top-right corner
        painter->drawLine(rect.topRight(), rect.topRight() + QPoint(-cornerLen, 0));
        painter->drawLine(rect.topRight(), rect.topRight() + QPoint(0, cornerLen));
        
        // Bottom-left corner
        painter->drawLine(rect.bottomLeft(), rect.bottomLeft() + QPoint(cornerLen, 0));
        painter->drawLine(rect.bottomLeft(), rect.bottomLeft() + QPoint(0, -cornerLen));
        
        // Bottom-right corner
        painter->drawLine(rect.bottomRight(), rect.bottomRight() + QPoint(-cornerLen, 0));
        painter->drawLine(rect.bottomRight(), rect.bottomRight() + QPoint(0, -cornerLen));
    }
}

void VideoOverlayRenderer::drawTrackLabel(QPainter* painter, const TrackOverlay& track,
                                           const QSize& frameSize) {
    Q_UNUSED(frameSize)
    
    if (!track.boundingBox.isValid()) return;
    
    QStringList labelParts;
    
    if (m_style.showTrackIds) {
        labelParts << track.trackId;
    }
    
    if (m_style.showClassification) {
        labelParts << classificationLabel(track.classification);
    }
    
    if (m_style.showThreatLevel && track.threatLevel > 0) {
        labelParts << threatLevelLabel(track.threatLevel);
    }
    
    if (m_style.showDistance && track.distance > 0) {
        labelParts << QString("%1m").arg(static_cast<int>(track.distance));
    }
    
    if (labelParts.isEmpty()) return;
    
    QString label = labelParts.join(" | ");
    
    painter->setFont(m_style.labelFont);
    QFontMetrics fm(m_style.labelFont);
    QRect textRect = fm.boundingRect(label);
    
    int x = track.boundingBox.x;
    int y = track.boundingBox.y - textRect.height() - 4;
    
    if (y < 5) {
        y = track.boundingBox.y + track.boundingBox.height + textRect.height() + 4;
    }
    
    // Draw background
    QRect bgRect(x - 2, y - textRect.height(), textRect.width() + 4, textRect.height() + 4);
    painter->fillRect(bgRect, m_style.textBackgroundColor);
    
    // Draw text
    painter->setPen(m_style.textColor);
    painter->drawText(x, y, label);
}

void VideoOverlayRenderer::drawCrosshairs(QPainter* painter, const QPointF& point,
                                           const QSize& frameSize) {
    int x = static_cast<int>(point.x() * frameSize.width());
    int y = static_cast<int>(point.y() * frameSize.height());
    int size = m_style.crosshairSize;
    
    painter->setPen(QPen(m_style.crosshairColor, 2));
    
    // Draw crosshair lines
    painter->drawLine(x - size, y, x - 5, y);
    painter->drawLine(x + 5, y, x + size, y);
    painter->drawLine(x, y - size, x, y - 5);
    painter->drawLine(x, y + 5, x, y + size);
    
    // Draw center circle
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(QPoint(x, y), 5, 5);
}

void VideoOverlayRenderer::drawTelemetryBar(QPainter* painter, const QSize& frameSize) {
    QString telemetryText = QString("%1  Az: %2°  El: %3°  Zoom: %4x")
                               .arg(m_telemetry.cameraName)
                               .arg(m_telemetry.azimuth, 0, 'f', 1)
                               .arg(m_telemetry.elevation, 0, 'f', 1)
                               .arg(m_telemetry.zoom, 0, 'f', 1);
    
    painter->setFont(m_style.telemetryFont);
    QFontMetrics fm(m_style.telemetryFont);
    QRect textRect = fm.boundingRect(telemetryText);
    
    int x = 10;
    int y = frameSize.height() - 10;
    
    // Draw background
    QRect bgRect(x - 2, y - textRect.height(), textRect.width() + 4, textRect.height() + 4);
    painter->fillRect(bgRect, m_style.textBackgroundColor);
    
    // Draw text
    painter->setPen(m_style.textColor);
    painter->drawText(x, y, telemetryText);
}

void VideoOverlayRenderer::drawRecordingIndicator(QPainter* painter, const QSize& frameSize) {
    int x = frameSize.width() - 80;
    int y = 25;
    
    // Draw red circle
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    painter->drawEllipse(QPoint(x, y), 8, 8);
    
    // Draw "REC" text
    painter->setFont(m_style.labelFont);
    painter->setPen(Qt::white);
    painter->drawText(x + 15, y + 5, "REC");
}

void VideoOverlayRenderer::drawTimestamp(QPainter* painter, const QSize& frameSize) {
    QString timestamp = QDateTime::fromMSecsSinceEpoch(m_telemetry.timestamp)
                            .toString("yyyy-MM-dd hh:mm:ss.zzz");
    
    if (m_telemetry.timestamp == 0) {
        timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    }
    
    painter->setFont(m_style.telemetryFont);
    painter->setPen(m_style.textColor);
    
    QFontMetrics fm(m_style.telemetryFont);
    int textWidth = fm.horizontalAdvance(timestamp);
    
    int x = frameSize.width() - textWidth - 10;
    int y = frameSize.height() - 10;
    
    // Draw background
    QRect bgRect(x - 2, y - fm.height(), textWidth + 4, fm.height() + 4);
    painter->fillRect(bgRect, m_style.textBackgroundColor);
    
    // Draw text
    painter->drawText(x, y, timestamp);
}

QColor VideoOverlayRenderer::colorForClassification(TrackClassification cls) const {
    switch (cls) {
        case TrackClassification::Hostile:
            return m_style.hostileBoxColor;
        case TrackClassification::Friendly:
            return m_style.friendlyBoxColor;
        case TrackClassification::Pending:
            return m_style.pendingBoxColor;
        default:
            return m_style.trackBoxColor;
    }
}

QString VideoOverlayRenderer::classificationLabel(TrackClassification cls) const {
    switch (cls) {
        case TrackClassification::Unknown: return "UNK";
        case TrackClassification::Friendly: return "FND";
        case TrackClassification::Hostile: return "HOS";
        case TrackClassification::Pending: return "PND";
        case TrackClassification::Neutral: return "NEU";
        default: return "???";
    }
}

QString VideoOverlayRenderer::threatLevelLabel(int level) const {
    switch (level) {
        case 1: return "LOW";
        case 2: return "MOD";
        case 3: return "MED";
        case 4: return "HIGH";
        case 5: return "CRIT";
        default: return "";
    }
}

} // namespace CounterUAS
