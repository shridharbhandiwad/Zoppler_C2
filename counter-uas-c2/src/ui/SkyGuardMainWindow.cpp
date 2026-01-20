#include "ui/SkyGuardMainWindow.h"
#include "ui/SkyGuardTheme.h"
#include "ui/SidebarNavigation.h"
#include "ui/HeaderBar.h"
#include "ui/pages/TacticalMapPage.h"
#include "ui/pages/ThreatDatabasePage.h"
#include "ui/pages/SensorNetworkPage.h"
#include "ui/pages/AlertLogPage.h"
#include "ui/pages/SystemHealthPage.h"
#include "ui/MapWidget.h"
#include "core/TrackManager.h"
#include "core/ThreatAssessor.h"
#include "core/EngagementManager.h"
#include "video/VideoStreamManager.h"
#include "simulators/VideoSimulator.h"
#include "simulators/SystemSimulationManager.h"
#include "simulators/TrackSimulator.h"
#include "config/ConfigManager.h"
#include "utils/Logger.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

namespace CounterUAS {

SkyGuardMainWindow::SkyGuardMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_trackManager(new TrackManager(this))
    , m_threatAssessor(new ThreatAssessor(m_trackManager, this))
    , m_engagementManager(new EngagementManager(m_trackManager, this))
    , m_videoManager(new VideoStreamManager(this))
    , m_videoSimulator(new VideoSimulator(this))
    , m_simulationManager(new SystemSimulationManager(this))
    , m_statusUpdateTimer(new QTimer(this))
    , m_simulationRunning(false)
    , m_simulationPaused(false)
{
    setWindowTitle("SKYGUARD - C-UAS Command System");
    setMinimumSize(1280, 720);
    resize(1920, 1080);
    
    // Apply the SkyGuard theme
    setStyleSheet(SkyGuardTheme::getStyleSheet());
    
    setupUI();
    setupSubsystems();
    setupConnections();
    setupSimulation();
    
    // Start status update timer
    connect(m_statusUpdateTimer, &QTimer::timeout, this, &SkyGuardMainWindow::updateThreatLevel);
    m_statusUpdateTimer->start(1000);
    
    Logger::instance().info("SkyGuardMainWindow", "Application initialized");
}

SkyGuardMainWindow::~SkyGuardMainWindow() {
    stopSimulation();
}

void SkyGuardMainWindow::setupUI() {
    // Central widget
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setStyleSheet("background-color: #0a1628;");
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // === Sidebar Navigation ===
    m_sidebar = new SidebarNavigation(this);
    mainLayout->addWidget(m_sidebar);
    
    // === Content Area ===
    QWidget* contentArea = new QWidget(this);
    contentArea->setStyleSheet("background-color: #0a1628;");
    QVBoxLayout* contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    
    // Header bar
    m_headerBar = new HeaderBar(this);
    contentLayout->addWidget(m_headerBar);
    
    // Page stack
    m_pageStack = new QStackedWidget(this);
    m_pageStack->setStyleSheet("background-color: #0a1628;");
    
    // Create pages
    m_tacticalMapPage = new TacticalMapPage(this);
    m_pageStack->addWidget(m_tacticalMapPage);
    
    m_threatDatabasePage = new ThreatDatabasePage(this);
    m_pageStack->addWidget(m_threatDatabasePage);
    
    m_sensorNetworkPage = new SensorNetworkPage(this);
    m_pageStack->addWidget(m_sensorNetworkPage);
    
    m_alertLogPage = new AlertLogPage(this);
    m_pageStack->addWidget(m_alertLogPage);
    
    m_systemHealthPage = new SystemHealthPage(this);
    m_pageStack->addWidget(m_systemHealthPage);
    
    // Gallery placeholder
    m_galleryPage = new QWidget(this);
    m_galleryPage->setStyleSheet("background-color: #0a1628;");
    QVBoxLayout* galleryLayout = new QVBoxLayout(m_galleryPage);
    galleryLayout->setAlignment(Qt::AlignCenter);
    QLabel* galleryLabel = new QLabel("GALLERY / REFERENCE", m_galleryPage);
    galleryLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #667788; letter-spacing: 2px;"
    );
    galleryLayout->addWidget(galleryLabel);
    QLabel* gallerySubLabel = new QLabel("Coming soon...", m_galleryPage);
    gallerySubLabel->setStyleSheet("font-size: 14px; color: #445566;");
    galleryLayout->addWidget(gallerySubLabel);
    m_pageStack->addWidget(m_galleryPage);
    
    contentLayout->addWidget(m_pageStack, 1);
    
    // Status bar at bottom
    QWidget* statusBar = new QWidget(this);
    statusBar->setObjectName("statusBar");
    statusBar->setFixedHeight(36);
    statusBar->setStyleSheet(SkyGuardTheme::getStatusBarStyleSheet());
    
    QHBoxLayout* statusLayout = new QHBoxLayout(statusBar);
    statusLayout->setContentsMargins(16, 0, 16, 0);
    
    QLabel* statusText = new QLabel("STATUS:", statusBar);
    statusText->setObjectName("statusText");
    statusText->setStyleSheet("font-size: 12px; color: #667788; font-weight: bold;");
    statusLayout->addWidget(statusText);
    
    QLabel* onlineStatus = new QLabel("ONLINE", statusBar);
    onlineStatus->setObjectName("onlineStatus");
    onlineStatus->setStyleSheet("font-size: 12px; color: #00ff88; font-weight: bold;");
    statusLayout->addWidget(onlineStatus);
    
    statusLayout->addStretch();
    
    QLabel* versionText = new QLabel("V.2.4.1-ALPHA // SECURE", statusBar);
    versionText->setObjectName("versionText");
    versionText->setStyleSheet("font-size: 11px; color: #556677;");
    statusLayout->addWidget(versionText);
    
    contentLayout->addWidget(statusBar);
    
    mainLayout->addWidget(contentArea, 1);
}

void SkyGuardMainWindow::setupSubsystems() {
    // Set up managers for each page
    m_tacticalMapPage->setTrackManager(m_trackManager);
    m_tacticalMapPage->setThreatAssessor(m_threatAssessor);
    
    m_threatDatabasePage->setTrackManager(m_trackManager);
    
    m_alertLogPage->setThreatAssessor(m_threatAssessor);
    
    // Configure defended assets
    DefendedAsset baseAsset;
    baseAsset.id = "BASE-01";
    baseAsset.name = "Main Installation";
    baseAsset.position.latitude = 51.5050;  // London coordinates
    baseAsset.position.longitude = -0.0900;
    baseAsset.position.altitude = 100.0;
    baseAsset.criticalRadiusM = 500.0;
    baseAsset.warningRadiusM = 1500.0;
    baseAsset.priorityLevel = 5;
    m_threatAssessor->addDefendedAsset(baseAsset);
    
    // Center map on base
    if (m_tacticalMapPage->mapWidget()) {
        m_tacticalMapPage->mapWidget()->setCenter(baseAsset.position);
        m_tacticalMapPage->mapWidget()->setZoom(13);
    }
    
    // Configure simulation manager
    m_simulationManager->setTrackManager(m_trackManager);
    m_simulationManager->setThreatAssessor(m_threatAssessor);
    m_simulationManager->setEngagementManager(m_engagementManager);
    m_simulationManager->setVideoManager(m_videoManager);
    m_simulationManager->createFullSimulationEnvironment();
    
    Logger::instance().info("SkyGuardMainWindow", "Subsystems initialized");
}

void SkyGuardMainWindow::setupConnections() {
    // Sidebar navigation
    connect(m_sidebar, &SidebarNavigation::pageSelected,
            this, &SkyGuardMainWindow::onPageSelected);
    
    // Header bar
    connect(m_headerBar, &HeaderBar::simulateThreatClicked,
            this, &SkyGuardMainWindow::onSimulateThreat);
    
    // Page signals
    connect(m_tacticalMapPage, &TacticalMapPage::trackSelected,
            this, &SkyGuardMainWindow::onTrackSelected);
    connect(m_tacticalMapPage, &TacticalMapPage::viewAllThreats,
            this, [this]() {
                m_sidebar->setCurrentPage("threat_list");
                onPageSelected("threat_list");
            });
    
    connect(m_threatDatabasePage, &ThreatDatabasePage::trackSelected,
            this, &SkyGuardMainWindow::onTrackSelected);
    connect(m_threatDatabasePage, &ThreatDatabasePage::simulateThreatClicked,
            this, &SkyGuardMainWindow::onSimulateThreat);
    
    // Simulation manager signals
    connect(m_simulationManager, &SystemSimulationManager::runningChanged,
            this, [this](bool running) {
                m_sidebar->setOnlineStatus(running);
                if (running) {
                    m_headerBar->setSystemStatus("OPERATIONAL");
                } else {
                    m_headerBar->setSystemStatus("OFFLINE");
                }
            });
}

void SkyGuardMainWindow::setupSimulation() {
    // Configure video simulator
    m_videoSimulator->setVideoManager(m_videoManager);
    m_videoSimulator->setupDefaultCameras();
    
    Logger::instance().info("SkyGuardMainWindow", "Simulation setup complete");
}

void SkyGuardMainWindow::onPageSelected(const QString& pageId) {
    if (pageId == "tactical_map") {
        m_pageStack->setCurrentWidget(m_tacticalMapPage);
    } else if (pageId == "threat_list") {
        m_pageStack->setCurrentWidget(m_threatDatabasePage);
    } else if (pageId == "sensor_net") {
        m_pageStack->setCurrentWidget(m_sensorNetworkPage);
    } else if (pageId == "alert_log") {
        m_pageStack->setCurrentWidget(m_alertLogPage);
    } else if (pageId == "system_health") {
        m_pageStack->setCurrentWidget(m_systemHealthPage);
    } else if (pageId == "gallery") {
        m_pageStack->setCurrentWidget(m_galleryPage);
    }
}

void SkyGuardMainWindow::onSimulateThreat() {
    if (!m_simulationRunning) {
        startSimulation();
    }
    
    // Spawn a new threat via track simulator in simulation manager
    if (m_simulationManager && m_simulationManager->trackSimulator()) {
        m_simulationManager->trackSimulator()->injectTarget(ManualTargetParams());
    }
}

void SkyGuardMainWindow::onTrackSelected(const QString& trackId) {
    // Navigate to tactical map and select track
    m_sidebar->setCurrentPage("tactical_map");
    m_pageStack->setCurrentWidget(m_tacticalMapPage);
    
    if (m_tacticalMapPage->mapWidget()) {
        m_tacticalMapPage->mapWidget()->selectTrack(trackId);
    }
}

void SkyGuardMainWindow::updateThreatLevel() {
    if (!m_trackManager) return;
    
    int threatCount = 0;
    int hostileCount = 0;
    
    auto tracks = m_trackManager->allTracks();
    for (const auto& track : tracks) {
        if (track->classification() == TrackClassification::Hostile) {
            hostileCount++;
            threatCount++;
        } else if (track->classification() == TrackClassification::Pending) {
            threatCount++;
        }
    }
    
    // Update header threat level
    if (hostileCount > 2) {
        m_headerBar->setThreatLevel("CRITICAL");
    } else if (hostileCount > 0 || threatCount > 0) {
        m_headerBar->setThreatLevel("ELEVATED");
    } else {
        m_headerBar->setThreatLevel("LOW");
    }
    
    // Update tactical map page status
    m_tacticalMapPage->updateStatusCards();
}

void SkyGuardMainWindow::startSimulation() {
    if (m_simulationRunning) return;
    
    m_simulationManager->start();
    m_trackManager->start();
    m_threatAssessor->start();
    m_videoSimulator->start();
    
    // Update sensor status
    m_sensorNetworkPage->updateSensorStatus("SENTINEL-1", "ONLINE");
    m_sensorNetworkPage->updateSensorStatus("RF-SCANNER-ALPHA", "ONLINE");
    
    m_simulationRunning = true;
    m_simulationPaused = false;
    
    Logger::instance().info("SkyGuardMainWindow", "Simulation started");
}

void SkyGuardMainWindow::stopSimulation() {
    if (!m_simulationRunning) return;
    
    m_simulationManager->stop();
    m_trackManager->stop();
    m_threatAssessor->stop();
    m_videoSimulator->stop();
    
    // Update sensor status
    m_sensorNetworkPage->updateSensorStatus("SENTINEL-1", "OFFLINE");
    m_sensorNetworkPage->updateSensorStatus("RF-SCANNER-ALPHA", "OFFLINE");
    
    m_simulationRunning = false;
    m_simulationPaused = false;
    
    Logger::instance().info("SkyGuardMainWindow", "Simulation stopped");
}

void SkyGuardMainWindow::pauseSimulation() {
    if (!m_simulationRunning) return;
    
    if (m_simulationPaused) {
        m_simulationManager->resume();
        m_simulationPaused = false;
    } else {
        m_simulationManager->pause();
        m_simulationPaused = true;
    }
}

void SkyGuardMainWindow::resetSimulation() {
    stopSimulation();
    
    // Clear tracks from UI
    if (m_tacticalMapPage->mapWidget()) {
        m_tacticalMapPage->mapWidget()->clearTracks();
    }
    
    m_simulationManager->reset();
    m_simulationManager->createFullSimulationEnvironment();
    
    Logger::instance().info("SkyGuardMainWindow", "Simulation reset");
}

} // namespace CounterUAS
