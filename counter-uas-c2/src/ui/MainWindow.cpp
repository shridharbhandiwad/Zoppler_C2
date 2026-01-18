#include "ui/MainWindow.h"
#include "ui/MapWidget.h"
#include "ui/VideoDisplayWidget.h"
#include "ui/VideoGridWidget.h"
#include "ui/TrackListWidget.h"
#include "ui/TrackDetailPanel.h"
#include "ui/SensorStatusPanel.h"
#include "ui/CameraStatusPanel.h"
#include "ui/EffectorControlPanel.h"
#include "ui/AlertQueue.h"
#include "core/TrackManager.h"
#include "core/ThreatAssessor.h"
#include "core/EngagementManager.h"
#include "video/VideoStreamManager.h"
#include "simulators/VideoSimulator.h"
#include "config/ConfigManager.h"
#include "utils/Logger.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>

namespace CounterUAS {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_trackManager(new TrackManager(this))
    , m_threatAssessor(new ThreatAssessor(m_trackManager, this))
    , m_engagementManager(new EngagementManager(m_trackManager, this))
    , m_videoManager(new VideoStreamManager(this))
    , m_videoSimulator(new VideoSimulator(this))
    , m_statusUpdateTimer(new QTimer(this))
{
    setWindowTitle("Counter-UAS Command & Control System");
    setMinimumSize(1280, 720);
    resize(1920, 1080);
    
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupDockWidgets();
    setupConnections();
    initializeSubsystems();
    setupVideoSimulation();
    
    m_statusUpdateTimer->setInterval(1000);
    connect(m_statusUpdateTimer, &QTimer::timeout, this, &MainWindow::updateStatusBar);
    m_statusUpdateTimer->start();
    
    Logger::instance().info("MainWindow", "Application initialized");
}

MainWindow::~MainWindow() {
    stopSimulation();
}

void MainWindow::setupUI() {
    // Central widget with map and video
    QWidget* centralWidget = new QWidget(this);
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    
    // Map widget
    m_mapWidget = new MapWidget(this);
    mainSplitter->addWidget(m_mapWidget);
    
    // Video panel
    QWidget* videoPanel = new QWidget(this);
    QVBoxLayout* videoLayout = new QVBoxLayout(videoPanel);
    videoLayout->setContentsMargins(0, 0, 0, 0);
    
    m_primaryVideoWidget = new VideoDisplayWidget(this);
    m_primaryVideoWidget->setMinimumSize(640, 480);
    videoLayout->addWidget(m_primaryVideoWidget, 2);
    
    m_videoGridWidget = new VideoGridWidget(this);
    m_videoGridWidget->setMinimumHeight(200);
    videoLayout->addWidget(m_videoGridWidget, 1);
    
    mainSplitter->addWidget(videoPanel);
    mainSplitter->setSizes({700, 500});
    
    QVBoxLayout* centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->addWidget(mainSplitter);
    
    setCentralWidget(centralWidget);
    
    // Status bar
    statusBar()->showMessage("Ready");
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = this->menuBar();
    
    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("&Load Configuration...", this, &MainWindow::loadConfiguration, QKeySequence::Open);
    fileMenu->addAction("&Save Configuration...", this, &MainWindow::saveConfiguration, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, &QWidget::close, QKeySequence::Quit);
    
    // View menu
    QMenu* viewMenu = menuBar->addMenu("&View");
    viewMenu->addAction("&Full Screen Video", this, &MainWindow::toggleFullScreenVideo, Qt::Key_F11);
    viewMenu->addSeparator();
    
    // Sensors menu
    QMenu* sensorsMenu = menuBar->addMenu("&Sensors");
    sensorsMenu->addAction("Configure Sensors...");
    sensorsMenu->addAction("Sensor Diagnostics...");
    
    // Video menu
    QMenu* videoMenu = menuBar->addMenu("V&ideo");
    videoMenu->addAction("Add Camera Stream...");
    videoMenu->addAction("Recording Settings...");
    videoMenu->addSeparator();
    videoMenu->addAction("Start All Recording");
    videoMenu->addAction("Stop All Recording");
    
    // Engage menu
    QMenu* engageMenu = menuBar->addMenu("&Engage");
    engageMenu->addAction("Effector Status...");
    engageMenu->addAction("Rules of Engagement...");
    
    // Help menu
    QMenu* helpMenu = menuBar->addMenu("&Help");
    helpMenu->addAction("&About...", this, &MainWindow::showAbout);
}

void MainWindow::setupToolBar() {
    m_mainToolBar = addToolBar("Main");
    m_mainToolBar->setIconSize(QSize(24, 24));
    
    m_mainToolBar->addAction("Start Sim", this, &MainWindow::startSimulation);
    m_mainToolBar->addAction("Stop Sim", this, &MainWindow::stopSimulation);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction("Zoom In");
    m_mainToolBar->addAction("Zoom Out");
    m_mainToolBar->addAction("Pan");
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction("Record All");
    m_mainToolBar->addAction("Snapshot");
}

void MainWindow::setupDockWidgets() {
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
    
    // Track list dock (left)
    m_trackListDock = new QDockWidget("Track List", this);
    m_trackListWidget = new TrackListWidget(m_trackManager, this);
    m_trackListDock->setWidget(m_trackListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, m_trackListDock);
    
    // Track detail dock (left, tabbed with track list)
    m_trackDetailDock = new QDockWidget("Track Detail", this);
    m_trackDetailPanel = new TrackDetailPanel(this);
    m_trackDetailDock->setWidget(m_trackDetailPanel);
    tabifyDockWidget(m_trackListDock, m_trackDetailDock);
    
    // Sensor status dock (bottom left)
    m_sensorStatusDock = new QDockWidget("Sensor Status", this);
    m_sensorStatusPanel = new SensorStatusPanel(this);
    m_sensorStatusDock->setWidget(m_sensorStatusPanel);
    addDockWidget(Qt::BottomDockWidgetArea, m_sensorStatusDock);
    
    // Camera status dock (bottom)
    m_cameraStatusDock = new QDockWidget("Camera Status", this);
    m_cameraStatusPanel = new CameraStatusPanel(m_videoManager, this);
    m_cameraStatusDock->setWidget(m_cameraStatusPanel);
    tabifyDockWidget(m_sensorStatusDock, m_cameraStatusDock);
    
    // Effector control dock (right)
    m_effectorDock = new QDockWidget("Effector Control", this);
    m_effectorControlPanel = new EffectorControlPanel(m_engagementManager, this);
    m_effectorDock->setWidget(m_effectorControlPanel);
    addDockWidget(Qt::RightDockWidgetArea, m_effectorDock);
    
    // Alert queue dock (right, below effector)
    m_alertDock = new QDockWidget("Alerts", this);
    m_alertQueue = new AlertQueue(m_threatAssessor, this);
    m_alertDock->setWidget(m_alertQueue);
    addDockWidget(Qt::RightDockWidgetArea, m_alertDock);
}

void MainWindow::setupConnections() {
    // Track selection
    connect(m_trackListWidget, &TrackListWidget::trackSelected,
            this, &MainWindow::onTrackSelected);
    connect(m_mapWidget, &MapWidget::trackSelected,
            this, &MainWindow::onTrackSelected);
    
    // Track updates
    connect(m_trackManager, &TrackManager::trackUpdated,
            m_mapWidget, &MapWidget::updateTrack);
    connect(m_trackManager, &TrackManager::trackCreated,
            m_mapWidget, &MapWidget::addTrack);
    connect(m_trackManager, &TrackManager::trackDropped,
            m_mapWidget, &MapWidget::removeTrack);
    
    // Alerts
    connect(m_alertQueue, &AlertQueue::alertClicked,
            this, &MainWindow::onAlertClicked);
    
    // Engagement
    connect(m_effectorControlPanel, &EffectorControlPanel::engageRequested,
            this, &MainWindow::onEngageRequested);
    
    // Video
    connect(m_videoGridWidget, &VideoGridWidget::cameraSelected,
            m_primaryVideoWidget, &VideoDisplayWidget::setSource);
}

void MainWindow::initializeSubsystems() {
    // Load default defended assets
    DefendedAsset baseAsset;
    baseAsset.id = "BASE-01";
    baseAsset.name = "Main Installation";
    baseAsset.position.latitude = 34.0522;
    baseAsset.position.longitude = -118.2437;
    baseAsset.position.altitude = 100.0;
    baseAsset.criticalRadiusM = 500.0;
    baseAsset.warningRadiusM = 1500.0;
    baseAsset.priorityLevel = 5;
    m_threatAssessor->addDefendedAsset(baseAsset);
    
    // Center map on base
    m_mapWidget->setCenter(baseAsset.position);
    m_mapWidget->setZoom(15);
}

void MainWindow::setupVideoSimulation() {
    // Configure video simulator with default cameras
    m_videoSimulator->setVideoManager(m_videoManager);
    m_videoSimulator->setupDefaultCameras();
    
    // Connect video simulator signals
    connect(m_videoSimulator, &VideoSimulator::frameReady,
            this, &MainWindow::onSimulationVideoFrame);
    connect(m_videoSimulator, &VideoSimulator::cameraFrameReady,
            this, &MainWindow::onSimulationCameraFrame);
    
    // Connect video manager frame signals to grid widget
    connect(m_videoManager, &VideoStreamManager::frameReady,
            m_videoGridWidget, &VideoGridWidget::updateFrame);
    
    Logger::instance().info("MainWindow", "Video simulation configured");
}

void MainWindow::onSimulationVideoFrame(const QImage& frame, qint64 timestamp) {
    Q_UNUSED(timestamp)
    // Update primary video display with simulation frame
    m_primaryVideoWidget->updateFrame(frame);
}

void MainWindow::onSimulationCameraFrame(const QString& cameraId, const QImage& frame, qint64 timestamp) {
    Q_UNUSED(timestamp)
    // Update video grid with camera-specific frame
    m_videoGridWidget->updateFrame(cameraId, frame);
}

void MainWindow::startSimulation() {
    if (m_simulationRunning) return;
    
    // Start core simulation systems
    m_trackManager->start();
    m_threatAssessor->start();
    
    // Start video simulation - this will create simulated camera feeds
    m_videoSimulator->start();
    
    // Set primary video source to first simulated camera
    auto cameras = m_videoSimulator->simulatedCameras();
    if (!cameras.isEmpty()) {
        m_primaryVideoWidget->setSource(cameras.first().cameraId);
    }
    
    m_simulationRunning = true;
    
    statusBar()->showMessage("Simulation started - Video feeds active");
    Logger::instance().info("MainWindow", "Simulation started with video");
}

void MainWindow::stopSimulation() {
    if (!m_simulationRunning) return;
    
    // Stop video simulation
    m_videoSimulator->stop();
    
    // Stop core simulation systems
    m_trackManager->stop();
    m_threatAssessor->stop();
    
    m_simulationRunning = false;
    
    statusBar()->showMessage("Simulation stopped");
    Logger::instance().info("MainWindow", "Simulation stopped");
}

void MainWindow::loadConfiguration() {
    QString path = QFileDialog::getOpenFileName(this, "Load Configuration",
                                                QString(), "JSON Files (*.json)");
    if (!path.isEmpty()) {
        ConfigManager::instance().loadConfig(path);
    }
}

void MainWindow::saveConfiguration() {
    QString path = QFileDialog::getSaveFileName(this, "Save Configuration",
                                                QString(), "JSON Files (*.json)");
    if (!path.isEmpty()) {
        ConfigManager::instance().saveConfig(path);
    }
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "About Counter-UAS C2",
                      "Counter-UAS Command & Control System\n\n"
                      "Version 1.0.0\n\n"
                      "A real-time command and control application for\n"
                      "Counter-UAS system operations.");
}

void MainWindow::updateStatusBar() {
    int trackCount = m_trackManager->trackCount();
    auto metrics = m_threatAssessor->metrics();
    
    QString status = QString("Tracks: %1 | Hostile: %2 | High Threat: %3 | %4")
                         .arg(trackCount)
                         .arg(metrics.hostileCount)
                         .arg(metrics.highThreatCount)
                         .arg(QDateTime::currentDateTime().toString("hh:mm:ss"));
    
    statusBar()->showMessage(status);
}

void MainWindow::onTrackSelected(const QString& trackId) {
    Track* track = m_trackManager->track(trackId);
    if (track) {
        m_trackDetailPanel->setTrack(track);
        m_mapWidget->selectTrack(trackId);
        
        // Slew camera if track has associated camera
        if (!track->associatedCameraId().isEmpty()) {
            m_primaryVideoWidget->setSource(track->associatedCameraId());
        }
    }
}

void MainWindow::onEngageRequested(const QString& trackId) {
    m_engagementManager->selectTrack(trackId);
}

void MainWindow::onCameraSlewRequested(const QString& trackId) {
    Track* track = m_trackManager->track(trackId);
    if (track) {
        m_videoManager->slewNearestCamera(track->position());
    }
}

void MainWindow::onAlertClicked(const QString& alertId) {
    auto alerts = m_threatAssessor->alerts();
    for (const auto& alert : alerts) {
        if (alert.alertId == alertId) {
            onTrackSelected(alert.trackId);
            break;
        }
    }
}

void MainWindow::toggleFullScreenVideo() {
    if (m_primaryVideoWidget->isFullScreen()) {
        m_primaryVideoWidget->showNormal();
    } else {
        m_primaryVideoWidget->showFullScreen();
    }
}

} // namespace CounterUAS
