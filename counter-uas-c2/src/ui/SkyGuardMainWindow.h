#ifndef SKYGUARDMAINWINDOW_H
#define SKYGUARDMAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTimer>

namespace CounterUAS {

class SidebarNavigation;
class HeaderBar;
class TacticalMapPage;
class ThreatDatabasePage;
class SensorNetworkPage;
class AlertLogPage;
class SystemHealthPage;

class TrackManager;
class ThreatAssessor;
class EngagementManager;
class VideoStreamManager;
class VideoSimulator;
class SystemSimulationManager;

/**
 * SkyGuardMainWindow - Redesigned main window for SKYGUARD C-UAS Command System
 * 
 * Features a modern tactical interface with:
 * - Left sidebar navigation
 * - Top header bar with status indicators
 * - Stacked page views for each navigation section
 * - Dark theme with cyan/teal accents
 */
class SkyGuardMainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit SkyGuardMainWindow(QWidget* parent = nullptr);
    ~SkyGuardMainWindow() override;
    
    // Core system access
    TrackManager* trackManager() const { return m_trackManager; }
    ThreatAssessor* threatAssessor() const { return m_threatAssessor; }
    EngagementManager* engagementManager() const { return m_engagementManager; }
    VideoStreamManager* videoManager() const { return m_videoManager; }
    SystemSimulationManager* simulationManager() const { return m_simulationManager; }
    
public slots:
    void startSimulation();
    void stopSimulation();
    void pauseSimulation();
    void resetSimulation();
    
private slots:
    void onPageSelected(const QString& pageId);
    void onSimulateThreat();
    void onTrackSelected(const QString& trackId);
    void updateThreatLevel();
    
private:
    void setupUI();
    void setupSubsystems();
    void setupConnections();
    void setupSimulation();
    
    // UI Components
    SidebarNavigation* m_sidebar;
    HeaderBar* m_headerBar;
    QStackedWidget* m_pageStack;
    
    // Pages
    TacticalMapPage* m_tacticalMapPage;
    ThreatDatabasePage* m_threatDatabasePage;
    SensorNetworkPage* m_sensorNetworkPage;
    AlertLogPage* m_alertLogPage;
    SystemHealthPage* m_systemHealthPage;
    QWidget* m_galleryPage;  // Placeholder for gallery
    
    // Core subsystems
    TrackManager* m_trackManager;
    ThreatAssessor* m_threatAssessor;
    EngagementManager* m_engagementManager;
    VideoStreamManager* m_videoManager;
    VideoSimulator* m_videoSimulator;
    SystemSimulationManager* m_simulationManager;
    
    // Timers
    QTimer* m_statusUpdateTimer;
    
    bool m_simulationRunning;
    bool m_simulationPaused;
};

} // namespace CounterUAS

#endif // SKYGUARDMAINWINDOW_H
