#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QTimer>

namespace CounterUAS {

class MapWidget;
class VideoDisplayWidget;
class VideoGridWidget;
class TrackListWidget;
class TrackDetailPanel;
class SensorStatusPanel;
class CameraStatusPanel;
class EffectorControlPanel;
class AlertQueue;
class TrackManager;
class ThreatAssessor;
class EngagementManager;
class VideoStreamManager;
class VideoSimulator;

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    
    // Core system access
    TrackManager* trackManager() const { return m_trackManager; }
    ThreatAssessor* threatAssessor() const { return m_threatAssessor; }
    EngagementManager* engagementManager() const { return m_engagementManager; }
    VideoStreamManager* videoManager() const { return m_videoManager; }
    VideoSimulator* videoSimulator() const { return m_videoSimulator; }
    
public slots:
    void startSimulation();
    void stopSimulation();
    void loadConfiguration();
    void saveConfiguration();
    void showAbout();
    
private slots:
    void updateStatusBar();
    void onTrackSelected(const QString& trackId);
    void onEngageRequested(const QString& trackId);
    void onCameraSlewRequested(const QString& trackId);
    void onAlertClicked(const QString& alertId);
    void toggleFullScreenVideo();
    void onSimulationVideoFrame(const QImage& frame, qint64 timestamp);
    void onSimulationCameraFrame(const QString& cameraId, const QImage& frame, qint64 timestamp);
    
private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupDockWidgets();
    void setupConnections();
    void initializeSubsystems();
    void setupVideoSimulation();
    
    // Core subsystems
    TrackManager* m_trackManager;
    ThreatAssessor* m_threatAssessor;
    EngagementManager* m_engagementManager;
    VideoStreamManager* m_videoManager;
    VideoSimulator* m_videoSimulator;
    
    // UI Widgets
    MapWidget* m_mapWidget;
    VideoDisplayWidget* m_primaryVideoWidget;
    VideoGridWidget* m_videoGridWidget;
    TrackListWidget* m_trackListWidget;
    TrackDetailPanel* m_trackDetailPanel;
    SensorStatusPanel* m_sensorStatusPanel;
    CameraStatusPanel* m_cameraStatusPanel;
    EffectorControlPanel* m_effectorControlPanel;
    AlertQueue* m_alertQueue;
    
    // Dock widgets
    QDockWidget* m_trackListDock;
    QDockWidget* m_trackDetailDock;
    QDockWidget* m_sensorStatusDock;
    QDockWidget* m_cameraStatusDock;
    QDockWidget* m_effectorDock;
    QDockWidget* m_alertDock;
    QDockWidget* m_videoDock;
    QDockWidget* m_videoGridDock;
    
    // Toolbar and status
    QToolBar* m_mainToolBar;
    QTimer* m_statusUpdateTimer;
    
    bool m_simulationRunning = false;
};

} // namespace CounterUAS

#endif // MAINWINDOW_H
