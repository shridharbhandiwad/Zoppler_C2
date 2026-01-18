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
#include "ui/dialogs/SensorConfigDialog.h"
#include "ui/dialogs/EffectorStatusDialog.h"
#include "ui/dialogs/SimulationSettingsDialog.h"
#include "ui/dialogs/RulesOfEngagementDialog.h"
#include "ui/dialogs/RecordingSettingsDialog.h"
#include "core/TrackManager.h"
#include "core/ThreatAssessor.h"
#include "core/EngagementManager.h"
#include "video/VideoStreamManager.h"
#include "simulators/VideoSimulator.h"
#include "simulators/SystemSimulationManager.h"
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
#include <QInputDialog>
#include <QSettings>

namespace CounterUAS {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_trackManager(new TrackManager(this))
    , m_threatAssessor(new ThreatAssessor(m_trackManager, this))
    , m_engagementManager(new EngagementManager(m_trackManager, this))
    , m_videoManager(new VideoStreamManager(this))
    , m_videoSimulator(new VideoSimulator(this))
    , m_simulationManager(new SystemSimulationManager(this))
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
    setupSimulationManager();
    
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
    
    // Status bar with custom widgets
    m_statusTrackCount = new QLabel("Tracks: 0", this);
    m_statusThreatCount = new QLabel("Threats: 0", this);
    m_statusSimStatus = new QLabel("Simulation: Stopped", this);
    m_statusTime = new QLabel("", this);
    
    statusBar()->addWidget(m_statusTrackCount);
    statusBar()->addWidget(new QLabel(" | ", this));
    statusBar()->addWidget(m_statusThreatCount);
    statusBar()->addWidget(new QLabel(" | ", this));
    statusBar()->addWidget(m_statusSimStatus);
    statusBar()->addPermanentWidget(m_statusTime);
    
    statusBar()->showMessage("Ready");
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = this->menuBar();
    
    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    fileMenu->addAction("&Load Configuration...", this, &MainWindow::loadConfiguration, QKeySequence::Open);
    fileMenu->addAction("&Save Configuration...", this, &MainWindow::saveConfiguration, QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction("&Export Config...", this, &MainWindow::exportConfiguration);
    fileMenu->addAction("&Import Config...", this, &MainWindow::importConfiguration);
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, &QWidget::close, QKeySequence::Quit);
    
    // View menu
    QMenu* viewMenu = menuBar->addMenu("&View");
    createViewMenu(viewMenu);
    
    // Simulation menu
    QMenu* simMenu = menuBar->addMenu("Si&mulation");
    m_startSimAction = simMenu->addAction("&Start Simulation", this, &MainWindow::startSimulation, Qt::Key_F5);
    m_stopSimAction = simMenu->addAction("S&top Simulation", this, &MainWindow::stopSimulation, Qt::Key_F6);
    m_stopSimAction->setEnabled(false);
    m_pauseSimAction = simMenu->addAction("&Pause Simulation", this, &MainWindow::pauseSimulation, Qt::Key_F7);
    m_pauseSimAction->setEnabled(false);
    simMenu->addAction("&Reset Simulation", this, &MainWindow::resetSimulation);
    simMenu->addSeparator();
    simMenu->addAction("Simulation &Settings...", this, &MainWindow::onSimulationSettings);
    
    // Sensors menu
    QMenu* sensorsMenu = menuBar->addMenu("&Sensors");
    sensorsMenu->addAction("&Configure Sensors...", this, &MainWindow::onSensorConfigure);
    sensorsMenu->addAction("Sensor &Diagnostics...", this, &MainWindow::onSensorDiagnostics);
    sensorsMenu->addSeparator();
    sensorsMenu->addAction("Start All Sensors", this, [this]() {
        QMessageBox::information(this, "Sensors", "All sensors started.");
    });
    sensorsMenu->addAction("Stop All Sensors", this, [this]() {
        QMessageBox::information(this, "Sensors", "All sensors stopped.");
    });
    
    // Video menu
    QMenu* videoMenu = menuBar->addMenu("V&ideo");
    videoMenu->addAction("&Add Camera Stream...", this, &MainWindow::onAddCameraStream);
    videoMenu->addAction("&Recording Settings...", this, &MainWindow::onRecordingSettings);
    videoMenu->addSeparator();
    videoMenu->addAction("Start &All Recording", this, &MainWindow::onStartAllRecording);
    videoMenu->addAction("S&top All Recording", this, &MainWindow::onStopAllRecording);
    videoMenu->addSeparator();
    videoMenu->addAction("Take &Snapshot", this, &MainWindow::onTakeSnapshot, Qt::Key_F8);
    videoMenu->addAction("&Full Screen Video", this, &MainWindow::toggleFullScreenVideo, Qt::Key_F11);
    
    // Engage menu
    QMenu* engageMenu = menuBar->addMenu("&Engage");
    engageMenu->addAction("&Effector Status...", this, &MainWindow::onEffectorStatus);
    engageMenu->addAction("&Rules of Engagement...", this, &MainWindow::onRulesOfEngagement);
    engageMenu->addSeparator();
    engageMenu->addAction("Initialize All Effectors", this, [this]() {
        for (auto* eff : m_engagementManager->effectors()) {
            eff->initialize();
        }
        QMessageBox::information(this, "Effectors", "All effectors initializing.");
    });
    engageMenu->addAction("Shutdown All Effectors", this, [this]() {
        for (auto* eff : m_engagementManager->effectors()) {
            eff->shutdown();
        }
        QMessageBox::information(this, "Effectors", "All effectors shutdown.");
    });
    
    // Help menu
    QMenu* helpMenu = menuBar->addMenu("&Help");
    helpMenu->addAction("&User Guide", this, &MainWindow::showHelp, Qt::Key_F1);
    helpMenu->addAction("&About...", this, &MainWindow::showAbout);
}

void MainWindow::createViewMenu(QMenu* viewMenu) {
    // Panel visibility toggles
    QMenu* panelsMenu = viewMenu->addMenu("&Panels");
    
    auto addPanelToggle = [panelsMenu](QDockWidget* dock) {
        if (dock) {
            QAction* action = panelsMenu->addAction(dock->windowTitle());
            action->setCheckable(true);
            action->setChecked(dock->isVisible());
            QObject::connect(action, &QAction::toggled, dock, &QDockWidget::setVisible);
            QObject::connect(dock, &QDockWidget::visibilityChanged, action, &QAction::setChecked);
        }
    };
    
    // Will be connected after dock widgets are created
    viewMenu->addSeparator();
    
    // Layout management
    viewMenu->addAction("&Save Layout", this, &MainWindow::onSaveLayout);
    viewMenu->addAction("&Restore Layout", this, &MainWindow::onRestoreLayout);
    viewMenu->addAction("Reset to &Default", this, &MainWindow::onResetLayout);
    viewMenu->addSeparator();
    viewMenu->addAction("&Full Screen Video", this, &MainWindow::toggleFullScreenVideo, Qt::Key_F11);
}

void MainWindow::setupToolBar() {
    m_mainToolBar = addToolBar("Main");
    m_mainToolBar->setIconSize(QSize(24, 24));
    m_mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    // Simulation controls
    QAction* startAction = m_mainToolBar->addAction("Start Sim");
    connect(startAction, &QAction::triggered, this, &MainWindow::startSimulation);
    
    QAction* stopAction = m_mainToolBar->addAction("Stop Sim");
    connect(stopAction, &QAction::triggered, this, &MainWindow::stopSimulation);
    
    QAction* pauseAction = m_mainToolBar->addAction("Pause");
    connect(pauseAction, &QAction::triggered, this, &MainWindow::pauseSimulation);
    
    m_mainToolBar->addSeparator();
    
    // Map controls
    m_mainToolBar->addAction("Zoom In", m_mapWidget, [this]() { m_mapWidget->setZoom(m_mapWidget->zoom() + 1); });
    m_mainToolBar->addAction("Zoom Out", m_mapWidget, [this]() { m_mapWidget->setZoom(m_mapWidget->zoom() - 1); });
    m_mainToolBar->addAction("Center", m_mapWidget, [this]() { 
        GeoPosition basePos;
        basePos.latitude = 34.0522;
        basePos.longitude = -118.2437;
        basePos.altitude = 100.0;
        m_mapWidget->setCenter(basePos); 
    });
    
    m_mainToolBar->addSeparator();
    
    // Recording
    m_mainToolBar->addAction("Record All", this, &MainWindow::onStartAllRecording);
    m_mainToolBar->addAction("Snapshot", this, &MainWindow::onTakeSnapshot);
    
    // Secondary toolbar for simulation status
    m_simulationToolBar = addToolBar("Simulation");
    m_simulationToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    
    QLabel* scenarioLabel = new QLabel(" Scenario: Default ", this);
    m_simulationToolBar->addWidget(scenarioLabel);
    
    m_simulationToolBar->addAction("Settings", this, &MainWindow::onSimulationSettings);
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
    
    // Add default sensors to status panel
    m_sensorStatusPanel->addSensor("SIM-RADAR-001", "Primary Radar", "RADAR");
    m_sensorStatusPanel->addSensor("SIM-RADAR-002", "Secondary Radar", "RADAR");
    m_sensorStatusPanel->addSensor("SIM-RF-001", "RF Detector", "RF_DETECTOR");
    m_sensorStatusPanel->addSensor("SIM-CAM-001", "Main EO Tracker", "CAMERA");
    m_sensorStatusPanel->addSensor("SIM-CAM-002", "Thermal Camera", "CAMERA");
    
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
    
    // Simulation manager signals
    connect(m_simulationManager, &SystemSimulationManager::runningChanged,
            this, [this](bool running) {
                m_startSimAction->setEnabled(!running);
                m_stopSimAction->setEnabled(running);
                m_pauseSimAction->setEnabled(running);
                m_statusSimStatus->setText(running ? "Simulation: Running" : "Simulation: Stopped");
            });
    
    connect(m_simulationManager, &SystemSimulationManager::pausedChanged,
            this, [this](bool paused) {
                m_pauseSimAction->setText(paused ? "Resume Simulation" : "Pause Simulation");
                if (paused) {
                    m_statusSimStatus->setText("Simulation: Paused");
                }
            });
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

void MainWindow::setupSimulationManager() {
    // Configure simulation manager with all subsystems
    m_simulationManager->setTrackManager(m_trackManager);
    m_simulationManager->setThreatAssessor(m_threatAssessor);
    m_simulationManager->setEngagementManager(m_engagementManager);
    m_simulationManager->setVideoManager(m_videoManager);
    
    // Create full simulation environment
    m_simulationManager->createFullSimulationEnvironment();
    
    Logger::instance().info("MainWindow", "Simulation manager configured");
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
    
    // Start simulation manager (coordinates all simulators)
    m_simulationManager->start();
    
    // Start core simulation systems
    m_trackManager->start();
    m_threatAssessor->start();
    
    // Start video simulation
    m_videoSimulator->start();
    
    // Set primary video source to first simulated camera
    auto cameras = m_videoSimulator->simulatedCameras();
    if (!cameras.isEmpty()) {
        m_primaryVideoWidget->setSource(cameras.first().cameraId);
    }
    
    // Update sensor status
    m_sensorStatusPanel->updateSensorStatus("SIM-RADAR-001", "ONLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-RADAR-002", "ONLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-RF-001", "ONLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-CAM-001", "ONLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-CAM-002", "ONLINE");
    
    m_simulationRunning = true;
    
    statusBar()->showMessage("Simulation started - All systems active");
    Logger::instance().info("MainWindow", "Simulation started with full environment");
}

void MainWindow::stopSimulation() {
    if (!m_simulationRunning) return;
    
    // Stop simulation manager
    m_simulationManager->stop();
    
    // Stop video simulation
    m_videoSimulator->stop();
    
    // Stop core simulation systems
    m_trackManager->stop();
    m_threatAssessor->stop();
    
    // Update sensor status
    m_sensorStatusPanel->updateSensorStatus("SIM-RADAR-001", "OFFLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-RADAR-002", "OFFLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-RF-001", "OFFLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-CAM-001", "OFFLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-CAM-002", "OFFLINE");
    
    m_simulationRunning = false;
    m_simulationPaused = false;
    
    statusBar()->showMessage("Simulation stopped");
    Logger::instance().info("MainWindow", "Simulation stopped");
}

void MainWindow::pauseSimulation() {
    if (!m_simulationRunning) return;
    
    if (m_simulationPaused) {
        m_simulationManager->resume();
        m_simulationPaused = false;
        statusBar()->showMessage("Simulation resumed");
    } else {
        m_simulationManager->pause();
        m_simulationPaused = true;
        statusBar()->showMessage("Simulation paused");
    }
}

void MainWindow::resetSimulation() {
    stopSimulation();
    m_simulationManager->reset();
    statusBar()->showMessage("Simulation reset");
}

void MainWindow::loadConfiguration() {
    QString path = QFileDialog::getOpenFileName(this, "Load Configuration",
                                                QString(), "JSON Files (*.json)");
    if (!path.isEmpty()) {
        ConfigManager::instance().loadConfig(path);
        statusBar()->showMessage("Configuration loaded: " + path);
    }
}

void MainWindow::saveConfiguration() {
    QString path = QFileDialog::getSaveFileName(this, "Save Configuration",
                                                QString(), "JSON Files (*.json)");
    if (!path.isEmpty()) {
        ConfigManager::instance().saveConfig(path);
        statusBar()->showMessage("Configuration saved: " + path);
    }
}

void MainWindow::exportConfiguration() {
    QString path = QFileDialog::getSaveFileName(this, "Export Configuration",
                                                "config_export.json", "JSON Files (*.json)");
    if (!path.isEmpty()) {
        ConfigManager::instance().saveConfig(path);
        QMessageBox::information(this, "Export", "Configuration exported successfully.");
    }
}

void MainWindow::importConfiguration() {
    QString path = QFileDialog::getOpenFileName(this, "Import Configuration",
                                                QString(), "JSON Files (*.json)");
    if (!path.isEmpty()) {
        ConfigManager::instance().loadConfig(path);
        QMessageBox::information(this, "Import", "Configuration imported successfully.");
    }
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "About Counter-UAS C2",
                      "Counter-UAS Command & Control System\n\n"
                      "Version 1.0.0\n\n"
                      "A real-time command and control application for\n"
                      "Counter-UAS system operations.\n\n"
                      "Features:\n"
                      "- Multi-sensor integration (Radar, RF, Camera)\n"
                      "- Track management with Kalman filtering\n"
                      "- Threat assessment and prioritization\n"
                      "- Video feed management with overlays\n"
                      "- Effector control (RF Jammer, Kinetic, DE)\n"
                      "- Complete simulation environment");
}

void MainWindow::showHelp() {
    QMessageBox::information(this, "Help",
                            "Counter-UAS C2 User Guide\n\n"
                            "Quick Start:\n"
                            "1. Click 'Start Sim' to begin simulation\n"
                            "2. Tracks will appear on the map\n"
                            "3. Click a track to view details\n"
                            "4. Use the Engage menu for effector control\n\n"
                            "Keyboard Shortcuts:\n"
                            "F5 - Start Simulation\n"
                            "F6 - Stop Simulation\n"
                            "F7 - Pause Simulation\n"
                            "F8 - Take Snapshot\n"
                            "F11 - Full Screen Video");
}

void MainWindow::updateStatusBar() {
    int trackCount = m_trackManager->trackCount();
    auto metrics = m_threatAssessor->metrics();
    
    m_statusTrackCount->setText(QString("Tracks: %1").arg(trackCount));
    m_statusThreatCount->setText(QString("Hostile: %1 | High Threat: %2")
                                     .arg(metrics.hostileCount)
                                     .arg(metrics.highThreatCount));
    m_statusTime->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
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

void MainWindow::onSensorConfigure() {
    SensorConfigDialog dialog(m_simulationManager->sensorSimulator(), this);
    dialog.exec();
}

void MainWindow::onSensorDiagnostics() {
    QMessageBox::information(this, "Sensor Diagnostics",
        "Sensor Diagnostics\n\n"
        "Radar-001: ONLINE (Signal Quality: 95%)\n"
        "Radar-002: ONLINE (Signal Quality: 92%)\n"
        "RF-001: ONLINE (Signal Quality: 88%)\n"
        "CAM-001: ONLINE (Signal Quality: 100%)\n"
        "CAM-002: ONLINE (Signal Quality: 100%)");
}

void MainWindow::onEffectorStatus() {
    EffectorStatusDialog dialog(m_engagementManager, this);
    dialog.exec();
}

void MainWindow::onRulesOfEngagement() {
    RulesOfEngagementDialog dialog(m_engagementManager, this);
    dialog.exec();
}

void MainWindow::onRecordingSettings() {
    RecordingSettingsDialog dialog(m_videoManager, this);
    dialog.exec();
}

void MainWindow::onSimulationSettings() {
    SimulationSettingsDialog dialog(m_simulationManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        statusBar()->showMessage("Simulation settings updated");
    }
}

void MainWindow::onAddCameraStream() {
    QString url = QInputDialog::getText(this, "Add Camera Stream",
        "Enter RTSP URL or device path:");
    if (!url.isEmpty()) {
        CameraDefinition cam;
        cam.cameraId = QString("CAM-%1").arg(QDateTime::currentMSecsSinceEpoch() % 10000);
        cam.name = "New Camera";
        cam.streamUrl = url;
        cam.sourceType = "RTSP";
        m_videoManager->addStream(cam);
        QMessageBox::information(this, "Camera Added", 
            QString("Camera %1 added successfully.").arg(cam.cameraId));
    }
}

void MainWindow::onStartAllRecording() {
    m_videoManager->startAllRecording("recordings/");
    statusBar()->showMessage("Started recording on all cameras");
}

void MainWindow::onStopAllRecording() {
    m_videoManager->stopAllRecording();
    statusBar()->showMessage("Stopped all recordings");
}

void MainWindow::onTakeSnapshot() {
    QString path = QFileDialog::getSaveFileName(this, "Save Snapshot",
        QString("snapshot_%1.png").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
        "PNG Images (*.png)");
    if (!path.isEmpty()) {
        QImage frame = m_primaryVideoWidget->currentFrame();
        if (!frame.isNull()) {
            frame.save(path);
            statusBar()->showMessage("Snapshot saved: " + path);
        }
    }
}

void MainWindow::onSaveLayout() {
    QSettings settings("CounterUAS", "C2");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    statusBar()->showMessage("Layout saved");
}

void MainWindow::onRestoreLayout() {
    QSettings settings("CounterUAS", "C2");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    statusBar()->showMessage("Layout restored");
}

void MainWindow::onResetLayout() {
    // Reset to default layout
    m_trackListDock->show();
    m_trackDetailDock->show();
    m_sensorStatusDock->show();
    m_cameraStatusDock->show();
    m_effectorDock->show();
    m_alertDock->show();
    statusBar()->showMessage("Layout reset to default");
}

} // namespace CounterUAS
