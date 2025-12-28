#ifndef VIDEOOVERLAYRENDERER_H
#define VIDEOOVERLAYRENDERER_H

#include <QObject>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QColor>
#include "core/Track.h"

namespace CounterUAS {

/**
 * @brief Overlay style configuration
 */
struct OverlayStyle {
    QColor trackBoxColor = Qt::green;
    QColor hostileBoxColor = Qt::red;
    QColor friendlyBoxColor = Qt::cyan;
    QColor pendingBoxColor = Qt::yellow;
    QColor crosshairColor = Qt::red;
    QColor textColor = Qt::white;
    QColor textBackgroundColor = QColor(0, 0, 0, 128);
    
    int boxLineWidth = 2;
    int crosshairSize = 20;
    QFont labelFont = QFont("Arial", 10);
    QFont telemetryFont = QFont("Courier", 9);
    
    bool showTrackIds = true;
    bool showClassification = true;
    bool showThreatLevel = true;
    bool showVelocity = false;
    bool showDistance = true;
    bool showTimestamp = true;
    bool showCameraInfo = true;
    bool showRecordingIndicator = true;
};

/**
 * @brief Telemetry data for overlay
 */
struct OverlayTelemetry {
    QString cameraId;
    QString cameraName;
    double azimuth = 0.0;
    double elevation = 0.0;
    double zoom = 1.0;
    bool recording = false;
    qint64 timestamp = 0;
    QString operatorNote;
};

/**
 * @brief Track overlay data
 */
struct TrackOverlay {
    QString trackId;
    BoundingBox boundingBox;
    TrackClassification classification;
    int threatLevel = 0;
    double distance = 0.0;
    double bearing = 0.0;
    VelocityVector velocity;
    bool isSelected = false;
    bool isEngaged = false;
};

/**
 * @brief Video overlay renderer for drawing on video frames
 */
class VideoOverlayRenderer : public QObject {
    Q_OBJECT
    
public:
    explicit VideoOverlayRenderer(QObject* parent = nullptr);
    ~VideoOverlayRenderer() override = default;
    
    // Style configuration
    void setStyle(const OverlayStyle& style);
    OverlayStyle style() const { return m_style; }
    
    // Telemetry
    void setTelemetry(const OverlayTelemetry& telemetry);
    
    // Track overlays
    void setTrackOverlays(const QList<TrackOverlay>& tracks);
    void addTrackOverlay(const TrackOverlay& track);
    void removeTrackOverlay(const QString& trackId);
    void clearTrackOverlays();
    
    // Selected track
    void setSelectedTrack(const QString& trackId);
    QString selectedTrack() const { return m_selectedTrackId; }
    
    // Designation crosshairs
    void setDesignationPoint(const QPointF& point);
    void clearDesignationPoint();
    bool hasDesignation() const { return m_hasDesignation; }
    
    // Render overlay onto frame
    QImage renderOverlay(const QImage& frame);
    void renderOverlay(QPainter* painter, const QSize& frameSize);
    
signals:
    void trackClicked(const QString& trackId);
    
private:
    void drawTrackBox(QPainter* painter, const TrackOverlay& track, const QSize& frameSize);
    void drawTrackLabel(QPainter* painter, const TrackOverlay& track, const QSize& frameSize);
    void drawCrosshairs(QPainter* painter, const QPointF& point, const QSize& frameSize);
    void drawTelemetryBar(QPainter* painter, const QSize& frameSize);
    void drawRecordingIndicator(QPainter* painter, const QSize& frameSize);
    void drawTimestamp(QPainter* painter, const QSize& frameSize);
    
    QColor colorForClassification(TrackClassification cls) const;
    QString classificationLabel(TrackClassification cls) const;
    QString threatLevelLabel(int level) const;
    
    OverlayStyle m_style;
    OverlayTelemetry m_telemetry;
    QList<TrackOverlay> m_tracks;
    QString m_selectedTrackId;
    
    bool m_hasDesignation = false;
    QPointF m_designationPoint;
};

} // namespace CounterUAS

#endif // VIDEOOVERLAYRENDERER_H
