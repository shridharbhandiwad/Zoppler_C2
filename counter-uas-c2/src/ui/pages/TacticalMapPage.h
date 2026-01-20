#ifndef TACTICALMAPPAGE_H
#define TACTICALMAPPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>
#include <QMap>
#include <QTimer>
#include <QPushButton>
#include "core/Track.h"

namespace CounterUAS {

class MapWidget;
class TrackManager;
class ThreatAssessor;
class Track;
class TiffMapSettingsDialog;

/**
 * CameraVideoWindow - Small video display widget for Day/Night cameras
 */
class CameraVideoWindow : public QFrame {
    Q_OBJECT
    
public:
    enum CameraType { Day, Night };
    
    explicit CameraVideoWindow(CameraType type, QWidget* parent = nullptr);
    void setTitle(const QString& title);
    void setStatus(const QString& status);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    
private:
    void setupUI();
    
    CameraType m_type;
    QLabel* m_titleLabel;
    QLabel* m_statusLabel;
    QLabel* m_videoFrame;
    int m_frameCounter;
    int m_timerId;
};

/**
 * ThreatCard - Widget displaying a single threat/track summary
 */
class ThreatCard : public QFrame {
    Q_OBJECT
    
public:
    explicit ThreatCard(QWidget* parent = nullptr);
    
    void setTrackId(const QString& id);
    void setClassification(const QString& type);
    void setStatus(const QString& status);  // HOSTILE, FRIENDLY, etc.
    void setAltitude(double altitudeM);
    void setSpeed(double speedMps);
    void setSource(const QString& source);
    void setProgressValue(double progress);  // 0.0 - 1.0
    
signals:
    void clicked(const QString& trackId);
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    
private:
    void setupUI();
    void updateStyle();
    
    QString m_trackId;
    QString m_status;
    
    QLabel* m_idLabel;
    QLabel* m_statusBadge;
    QLabel* m_classLabel;
    QLabel* m_altLabel;
    QLabel* m_altValue;
    QLabel* m_spdLabel;
    QLabel* m_spdValue;
    QLabel* m_srcLabel;
    QLabel* m_srcValue;
    QFrame* m_progressBar;
};

/**
 * AlertItem - Widget for recent alert display
 */
class AlertItem : public QFrame {
    Q_OBJECT
    
public:
    explicit AlertItem(QWidget* parent = nullptr);
    
    void setTime(const QString& time);
    void setSeverity(const QString& severity);  // INFO, CRITICAL
    void setMessage(const QString& message);
    
private:
    void setupUI();
    
    QLabel* m_timeLabel;
    QLabel* m_severityBadge;
    QLabel* m_messageLabel;
};

/**
 * TacticalMapPage - Main tactical display page
 * 
 * Shows:
 * - Interactive map with track positions
 * - Status cards (Active Threats, Sensor Net, Coordinates)
 * - Threat summary cards
 * - Recent alerts panel
 */
class TacticalMapPage : public QWidget {
    Q_OBJECT
    
public:
    explicit TacticalMapPage(QWidget* parent = nullptr);
    ~TacticalMapPage() override = default;
    
    void setTrackManager(TrackManager* manager);
    void setThreatAssessor(ThreatAssessor* assessor);
    MapWidget* mapWidget() const { return m_mapWidget; }
    
public slots:
    void refreshThreatCards();
    void refreshAlerts();
    void updateStatusCards();
    void updateCoordinates(double lat, double lng, int zoom);
    void openTiffMapSettings();
    
signals:
    void trackSelected(const QString& trackId);
    void viewAllThreats();
    
private slots:
    void onThreatCardClicked(const QString& trackId);
    void onTrackCreated(const QString& trackId);
    void onTrackDropped(const QString& trackId);
    void onMapTrackCreated(const QString& trackId);
    void onMapTrackUpdated(const QString& trackId);
    void onTiffMapLoaded(const QString& filePath);
    void onTiffMapCleared();
    void onMapCenterChanged(const GeoPosition& pos);
    void onMapZoomChanged(double zoom);
    
private:
    void setupUI();
    void setupMapSection();
    void setupRightPanel();
    void setupCameraWindows();
    void setupMapControls();
    ThreatCard* createThreatCard(Track* track);
    
    MapWidget* m_mapWidget;
    TrackManager* m_trackManager;
    ThreatAssessor* m_threatAssessor;
    TiffMapSettingsDialog* m_tiffMapDialog;
    
    // Status cards
    QLabel* m_activeThreatsValue;
    QLabel* m_sensorNetValue;
    QLabel* m_latValue;
    QLabel* m_lngValue;
    QLabel* m_zoomValue;
    
    // Threat cards container
    QVBoxLayout* m_threatCardsLayout;
    QScrollArea* m_threatScrollArea;
    QMap<QString, ThreatCard*> m_threatCards;
    
    // Alerts container
    QVBoxLayout* m_alertsLayout;
    
    // Camera video windows
    CameraVideoWindow* m_dayCameraWindow;
    CameraVideoWindow* m_nightCameraWindow;
    
    // Map controls
    QPushButton* m_loadMapBtn;
    QLabel* m_mapStatusLabel;
};

} // namespace CounterUAS

#endif // TACTICALMAPPAGE_H
