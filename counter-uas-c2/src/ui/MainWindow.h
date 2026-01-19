#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>
#include <QToolButton>

namespace CounterUAS {

class MapWidget;
class PPIDisplayWidget;
class VideoDisplayWidget;
class VideoGridWidget;
class TrackListWidget;
class TrackDetailPanel;
class SensorStatusPanel;
class CameraStatusPanel;
class EffectorControlPanel;
class AlertQueue;
class QuickActionsPanel;
class SystemStatusWidget;
class TrackManager;
class ThreatAssessor;
class EngagementManager;
class VideoStreamManager;
class VideoSimulator;
class SystemSimulationManager;

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
    SystemSimulationManager* simulationManager() const { return m_simulationManager; }
    
public slots:
    // Simulation control
    void startSimulation();
    void stopSimulation();
    void pauseSimulation();
    void resetSimulation();
    
    // Configuration
    void loadConfiguration();
    void saveConfiguration();
    void exportConfiguration();
    void importConfiguration();
    
    // Info
    void showAbout();
    void showHelp();
    
private slots:
    void updateStatusBar();
    void onTrackSelected(const QString& trackId);
    void onEngageRequested(const QString& trackId);
    void onCameraSlewRequested(const QString& trackId);
    void onAlertClicked(const QString& alertId);
    void toggleFullScreenVideo();
    void onSimulationVideoFrame(const QImage& frame, qint64 timestamp);
    void onSimulationCameraFrame(const QString& cameraId, const QImage& frame, qint64 timestamp);
    
    // Menu actions
    void onSensorConfigure();
    void onSensorDiagnostics();
    void onEffectorStatus();
    void onRulesOfEngagement();
    void onRecordingSettings();
    void onSimulationSettings();
    void onAddCameraStream();
    void onStartAllRecording();
    void onStopAllRecording();
    void onTakeSnapshot();
    
    // View menu
    void onSaveLayout();
    void onRestoreLayout();
    void onResetLayout();
    
    // PPI Display controls
    void onTogglePPIDisplay();
    void onPPIDisplayModeChanged(int mode);
    void onPPIRangeChanged(double rangeM);
    void onPPISweepToggle();
    
private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupPPIToolBar();
    void setupDockWidgets();
    void setupConnections();
    void initializeSubsystems();
    void setupVideoSimulation();
    void setupSimulationManager();
    void setupPPIDisplay();
    void createViewMenu(QMenu* viewMenu);
    
    // Core subsystems
    TrackManager* m_trackManager;
    ThreatAssessor* m_threatAssessor;
    EngagementManager* m_engagementManager;
    VideoStreamManager* m_videoManager;
    VideoSimulator* m_videoSimulator;
    SystemSimulationManager* m_simulationManager;
    
    // UI Widgets
    MapWidget* m_mapWidget;
    PPIDisplayWidget* m_ppiWidget;
    QStackedWidget* m_displayStack;  // For switching between map and PPI
    VideoDisplayWidget* m_primaryVideoWidget;
    VideoGridWidget* m_videoGridWidget;
    TrackListWidget* m_trackListWidget;
    TrackDetailPanel* m_trackDetailPanel;
    SensorStatusPanel* m_sensorStatusPanel;
    CameraStatusPanel* m_cameraStatusPanel;
    EffectorControlPanel* m_effectorControlPanel;
    AlertQueue* m_alertQueue;
    
    // Additional UI Widgets
    QuickActionsPanel* m_quickActionsPanel;
    SystemStatusWidget* m_systemStatusWidget;
    
    // Dock widgets
    QDockWidget* m_trackListDock;
    QDockWidget* m_trackDetailDock;
    QDockWidget* m_sensorStatusDock;
    QDockWidget* m_cameraStatusDock;
    QDockWidget* m_effectorDock;
    QDockWidget* m_alertDock;
    QDockWidget* m_videoDock;
    QDockWidget* m_videoGridDock;
    QDockWidget* m_quickActionsDock;
    QDockWidget* m_systemStatusDock;
    
    // Toolbar and status
    QToolBar* m_mainToolBar;
    QToolBar* m_simulationToolBar;
    QToolBar* m_ppiToolBar;
    QTimer* m_statusUpdateTimer;
    
    // PPI controls
    QComboBox* m_displayModeCombo;
    QComboBox* m_ppiModeCombo;
    QAction* m_ppiSweepAction;
    
    // Status bar widgets
    QLabel* m_statusTrackCount;
    QLabel* m_statusThreatCount;
    QLabel* m_statusSimStatus;
    QLabel* m_statusTime;
    
    // Menu actions
    QAction* m_startSimAction;
    QAction* m_stopSimAction;
    QAction* m_pauseSimAction;
    
    bool m_simulationRunning = false;
    bool m_simulationPaused = false;
};

} // namespace CounterUAS

#endif // MAINWINDOW_H
