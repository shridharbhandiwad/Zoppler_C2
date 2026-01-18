#ifndef PPIDISPLAYWIDGET_H
#define PPIDISPLAYWIDGET_H

#include <QWidget>
#include <QHash>
#include <QPainter>
#include <QTimer>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCache>
#include "core/Track.h"

namespace CounterUAS {

class TrackManager;

/**
 * @brief PPI Display modes
 */
enum class PPIDisplayMode {
    RadarOnly,          // Pure radar display
    MapOverlay,         // Radar with map tiles underneath
    MapOnly,            // Map display without radar styling
    NightVision         // Night vision green style
};

/**
 * @brief PPI sweep mode
 */
enum class PPISweepMode {
    Rotating,           // Continuous rotating sweep
    Sector,             // Sector scan mode
    None                // No sweep animation
};

/**
 * @brief Track history point for trail display
 */
struct TrackHistoryPoint {
    QPointF position;
    qint64 timestamp;
    double intensity;   // For fading effect (0.0 - 1.0)
};

/**
 * @brief Map tile key for caching
 */
struct MapTileKey {
    int x;
    int y;
    int zoom;
    
    bool operator==(const MapTileKey& other) const {
        return x == other.x && y == other.y && zoom == other.zoom;
    }
};

inline size_t qHash(const MapTileKey& key, size_t seed = 0) {
    return ::qHash(key.x, seed) ^ ::qHash(key.y, seed) ^ ::qHash(key.zoom, seed);
}

/**
 * @brief PPI (Plan Position Indicator) Display Widget
 * 
 * Provides a classic radar-style circular display with:
 * - Range rings (concentric circles)
 * - Azimuth markers (radial lines)
 * - Rotating sweep animation
 * - Track plotting in polar coordinates
 * - Optional map tile overlay
 * - Track history trails
 * - Defended area visualization
 */
class PPIDisplayWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(double sweepAngle READ sweepAngle NOTIFY sweepAngleChanged)
    Q_PROPERTY(double rangeScale READ rangeScale WRITE setRangeScale NOTIFY rangeScaleChanged)
    Q_PROPERTY(PPIDisplayMode displayMode READ displayMode WRITE setDisplayMode NOTIFY displayModeChanged)
    
public:
    explicit PPIDisplayWidget(QWidget* parent = nullptr);
    ~PPIDisplayWidget() override;
    
    // Center position (radar location)
    void setCenter(const GeoPosition& pos);
    GeoPosition center() const { return m_center; }
    
    // Range scale in meters
    void setRangeScale(double rangeM);
    double rangeScale() const { return m_rangeScaleM; }
    
    // Display mode
    void setDisplayMode(PPIDisplayMode mode);
    PPIDisplayMode displayMode() const { return m_displayMode; }
    
    // Sweep mode
    void setSweepMode(PPISweepMode mode);
    PPISweepMode sweepMode() const { return m_sweepMode; }
    
    // Sweep configuration
    void setSweepSpeed(double degreesPerSecond);
    double sweepSpeed() const { return m_sweepSpeed; }
    double sweepAngle() const { return m_sweepAngle; }
    
    // Sector scan configuration
    void setSectorLimits(double startDeg, double endDeg);
    
    // Range ring configuration
    void setRangeRingCount(int count);
    int rangeRingCount() const { return m_rangeRingCount; }
    void setShowRangeLabels(bool show);
    bool showRangeLabels() const { return m_showRangeLabels; }
    
    // Azimuth configuration
    void setAzimuthDivisions(int divisions);
    int azimuthDivisions() const { return m_azimuthDivisions; }
    void setShowAzimuthLabels(bool show);
    bool showAzimuthLabels() const { return m_showAzimuthLabels; }
    
    // Track manager connection
    void setTrackManager(TrackManager* manager);
    
    // Track selection
    void selectTrack(const QString& trackId);
    QString selectedTrack() const { return m_selectedTrackId; }
    
    // Track history/trail
    void setShowTrackHistory(bool show);
    bool showTrackHistory() const { return m_showTrackHistory; }
    void setTrackHistoryLength(int seconds);
    
    // Defended area
    void setDefendedAreaVisible(bool visible);
    void setDefendedAreaRadii(double criticalM, double warningM, double detectionM);
    
    // Map tile configuration
    void setMapTileUrl(const QString& urlTemplate);
    void setMapZoomLevel(int zoom);
    int mapZoomLevel() const { return m_mapZoomLevel; }
    void setMapOpacity(double opacity);
    double mapOpacity() const { return m_mapOpacity; }
    
    // Color scheme
    void setBackgroundColor(const QColor& color);
    void setSweepColor(const QColor& color);
    void setRangeRingColor(const QColor& color);
    void setTrackColors(const QColor& hostile, const QColor& friendly,
                       const QColor& unknown, const QColor& neutral);
    
    // North indicator
    void setNorthUp(bool northUp);
    bool isNorthUp() const { return m_northUp; }
    void setHeading(double headingDeg);  // For head-up mode
    
    // Coordinate conversions
    QPointF geoToPPI(const GeoPosition& pos) const;
    GeoPosition ppiToGeo(const QPointF& ppiPos) const;
    QPointF polarToScreen(double rangeM, double azimuthDeg) const;
    
public slots:
    void addTrack(const QString& trackId);
    void updateTrack(const QString& trackId);
    void removeTrack(const QString& trackId);
    void clearTracks();
    
    void startSweep();
    void stopSweep();
    void resetSweep();
    
    // Force redraw
    void refresh();
    
signals:
    void trackSelected(const QString& trackId);
    void trackDoubleClicked(const QString& trackId);
    void mapClicked(const GeoPosition& pos);
    void rangeScaleChanged(double rangeM);
    void sweepAngleChanged(double angle);
    void displayModeChanged(PPIDisplayMode mode);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    
private slots:
    void updateSweep();
    void onMapTileReceived(QNetworkReply* reply);
    void updateTrackHistory();
    
private:
    // Rendering methods
    void drawBackground(QPainter& painter);
    void drawMapTiles(QPainter& painter);
    void drawRangeRings(QPainter& painter);
    void drawAzimuthLines(QPainter& painter);
    void drawSweep(QPainter& painter);
    void drawSweepTrail(QPainter& painter);
    void drawDefendedArea(QPainter& painter);
    void drawTracks(QPainter& painter);
    void drawTrackSymbol(QPainter& painter, Track* track, const QPointF& pos, bool selected);
    void drawTrackHistory(QPainter& painter, Track* track);
    void drawTrackLabel(QPainter& painter, Track* track, const QPointF& pos);
    void drawVelocityVector(QPainter& painter, Track* track, const QPointF& pos);
    void drawNorthIndicator(QPainter& painter);
    void drawScaleInfo(QPainter& painter);
    void drawCompassRose(QPainter& painter);
    
    // Map tile methods
    void requestMapTile(int x, int y, int zoom);
    QString getTileUrl(int x, int y, int zoom) const;
    MapTileKey geoToTile(const GeoPosition& pos, int zoom) const;
    QPointF tileToPixel(int tileX, int tileY, int zoom, const GeoPosition& topLeft) const;
    void updateVisibleTiles();
    
    // Helper methods
    QPointF screenCenter() const;
    double ppiRadius() const;
    QColor colorForClassification(TrackClassification cls) const;
    QColor colorForThreatLevel(int level) const;
    QString findTrackAtPoint(const QPointF& point) const;
    
    // Track manager
    TrackManager* m_trackManager = nullptr;
    QHash<QString, Track*> m_tracks;
    QHash<QString, QList<TrackHistoryPoint>> m_trackHistory;
    
    // Center position
    GeoPosition m_center;
    
    // Display settings
    PPIDisplayMode m_displayMode = PPIDisplayMode::RadarOnly;
    PPISweepMode m_sweepMode = PPISweepMode::Rotating;
    double m_rangeScaleM = 5000.0;  // 5km default
    bool m_northUp = true;
    double m_heading = 0.0;
    
    // Sweep state
    double m_sweepAngle = 0.0;
    double m_sweepSpeed = 30.0;  // degrees per second
    double m_sectorStart = 0.0;
    double m_sectorEnd = 360.0;
    QTimer* m_sweepTimer;
    bool m_sweepRunning = false;
    
    // Range rings
    int m_rangeRingCount = 5;
    bool m_showRangeLabels = true;
    
    // Azimuth lines
    int m_azimuthDivisions = 12;  // Every 30 degrees
    bool m_showAzimuthLabels = true;
    
    // Track selection
    QString m_selectedTrackId;
    
    // Track history
    bool m_showTrackHistory = true;
    int m_trackHistorySeconds = 30;
    QTimer* m_historyTimer;
    
    // Defended area
    bool m_showDefendedArea = true;
    double m_criticalRadiusM = 500.0;
    double m_warningRadiusM = 1500.0;
    double m_detectionRadiusM = 5000.0;
    
    // Map tiles
    QString m_mapTileUrlTemplate;
    int m_mapZoomLevel = 14;
    double m_mapOpacity = 0.5;
    QNetworkAccessManager* m_networkManager;
    QCache<MapTileKey, QPixmap> m_tileCache;
    QSet<MapTileKey> m_pendingTiles;
    
    // Colors
    QColor m_backgroundColor = QColor(10, 20, 10);
    QColor m_sweepColor = QColor(0, 255, 0, 200);
    QColor m_rangeRingColor = QColor(0, 180, 0, 150);
    QColor m_azimuthLineColor = QColor(0, 150, 0, 100);
    QColor m_hostileColor = Qt::red;
    QColor m_friendlyColor = Qt::cyan;
    QColor m_unknownColor = Qt::yellow;
    QColor m_neutralColor = Qt::gray;
    
    // Pre-rendered elements
    QPixmap m_backgroundCache;
    bool m_backgroundDirty = true;
    
    // Sweep trail (persistence effect)
    static constexpr int SWEEP_TRAIL_LENGTH = 60;
    QVector<double> m_sweepTrail;
};

} // namespace CounterUAS

Q_DECLARE_METATYPE(CounterUAS::MapTileKey)
Q_DECLARE_METATYPE(CounterUAS::PPIDisplayMode)
Q_DECLARE_METATYPE(CounterUAS::PPISweepMode)

#endif // PPIDISPLAYWIDGET_H
