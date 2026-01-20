#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QHash>
#include <QPainter>
#include "core/Track.h"

namespace CounterUAS {

class MapWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit MapWidget(QWidget* parent = nullptr);
    
    void setCenter(const GeoPosition& pos);
    GeoPosition center() const { return m_center; }
    
    void setZoom(double zoom);
    double zoom() const { return m_zoom; }
    
    void selectTrack(const QString& trackId);
    QString selectedTrack() const { return m_selectedTrackId; }
    
public slots:
    void addTrack(const QString& trackId);
    void updateTrack(const QString& trackId);
    void removeTrack(const QString& trackId);
    void clearTracks();
    void setTrackData(const QString& trackId, Track* track);
    
signals:
    void trackSelected(const QString& trackId);
    void mapClicked(const GeoPosition& pos);
    void zoomChanged(double zoom);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    
private:
    QPointF geoToScreen(const GeoPosition& pos) const;
    GeoPosition screenToGeo(const QPointF& screen) const;
    void drawSatelliteBackground(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawTracks(QPainter& painter);
    void drawDefendedArea(QPainter& painter);
    void drawScaleBar(QPainter& painter);
    void drawCompass(QPainter& painter);
    QColor colorForClassification(TrackClassification cls) const;
    
    GeoPosition m_center;
    double m_zoom = 15.0;
    QString m_selectedTrackId;
    QHash<QString, Track*> m_tracks;
};

} // namespace CounterUAS

#endif // MAPWIDGET_H
