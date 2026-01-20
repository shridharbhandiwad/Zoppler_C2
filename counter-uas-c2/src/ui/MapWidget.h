#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QHash>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include "core/Track.h"

namespace CounterUAS {

/**
 * TiffMapBounds - Geographical bounds for a TIFF map image
 * Defines the geo-referenced corners of the map image
 */
struct TiffMapBounds {
    double northLat = 0.0;   // Top edge latitude
    double southLat = 0.0;   // Bottom edge latitude
    double eastLon = 0.0;    // Right edge longitude
    double westLon = 0.0;    // Left edge longitude
    
    bool isValid() const {
        return (northLat > southLat) && (eastLon > westLon) &&
               (northLat >= -90.0 && northLat <= 90.0) &&
               (southLat >= -90.0 && southLat <= 90.0) &&
               (eastLon >= -180.0 && eastLon <= 180.0) &&
               (westLon >= -180.0 && westLon <= 180.0);
    }
    
    double width() const { return eastLon - westLon; }
    double height() const { return northLat - southLat; }
    
    GeoPosition center() const {
        GeoPosition pos;
        pos.latitude = (northLat + southLat) / 2.0;
        pos.longitude = (eastLon + westLon) / 2.0;
        pos.altitude = 0.0;
        return pos;
    }
    
    bool contains(double lat, double lon) const {
        return lat >= southLat && lat <= northLat &&
               lon >= westLon && lon <= eastLon;
    }
};

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
    
    // TIFF Map support
    bool loadTiffMap(const QString& filePath);
    bool loadTiffMap(const QString& filePath, const TiffMapBounds& bounds);
    void clearTiffMap();
    bool hasTiffMap() const { return m_hasTiffMap; }
    QString tiffMapPath() const { return m_tiffMapPath; }
    TiffMapBounds tiffMapBounds() const { return m_tiffBounds; }
    void setTiffMapBounds(const TiffMapBounds& bounds);
    void setTiffMapOpacity(double opacity); // 0.0 to 1.0
    double tiffMapOpacity() const { return m_tiffOpacity; }
    void setShowTiffMap(bool show);
    bool showTiffMap() const { return m_showTiffMap; }
    void centerOnTiffMap();  // Center view on loaded TIFF map
    
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
    void tiffMapLoaded(const QString& filePath);
    void tiffMapLoadError(const QString& error);
    void tiffMapCleared();
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    
private:
    QPointF geoToScreen(const GeoPosition& pos) const;
    GeoPosition screenToGeo(const QPointF& screen) const;
    void drawSatelliteBackground(QPainter& painter);
    void drawTiffMapBackground(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawTracks(QPainter& painter);
    void drawDefendedArea(QPainter& painter);
    void drawScaleBar(QPainter& painter);
    void drawCompass(QPainter& painter);
    void drawTiffMapInfo(QPainter& painter);
    QColor colorForClassification(TrackClassification cls) const;
    bool tryParseGeoTiffBounds(const QString& filePath, TiffMapBounds& bounds);
    void updateScaledTiffCache();
    
    GeoPosition m_center;
    double m_zoom = 15.0;
    QString m_selectedTrackId;
    QHash<QString, Track*> m_tracks;
    
    // TIFF map data
    bool m_hasTiffMap = false;
    bool m_showTiffMap = true;
    QString m_tiffMapPath;
    QImage m_tiffMapImage;
    QPixmap m_scaledTiffCache;
    TiffMapBounds m_tiffBounds;
    double m_tiffOpacity = 1.0;
    QSize m_lastWidgetSize;
    double m_lastZoom = 0.0;
};

} // namespace CounterUAS

#endif // MAPWIDGET_H
