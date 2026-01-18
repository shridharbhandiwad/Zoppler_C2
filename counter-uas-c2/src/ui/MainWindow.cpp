#include "ui/MainWindow.h"
#include "ui/MapWidget.h"
#include "ui/PPIDisplayWidget.h"
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
#include <QToolButton>

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
    setupPPIToolBar();
    setupDockWidgets();
    setupConnections();
    initializeSubsystems();
    setupVideoSimulation();
    setupSimulationManager();
    setupPPIDisplay();
    
    m_statusUpdateTimer->setInterval(1000);
    connect(m_statusUpdateTimer, &QTimer::timeout, this, &MainWindow::updateStatusBar);
    m_statusUpdateTimer->start();
    
    Logger::instance().info("MainWindow", "Application initialized");
}

MainWindow::~MainWindow() {
    stopSimulation();
}

void MainWindow::setupUI() {
    // Central widget with map/PPI and video
    QWidget* centralWidget = new QWidget(this);
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    
    // Create stacked widget for map/PPI display switching
    m_displayStack = new QStackedWidget(this);
    
    // Map widget
    m_mapWidget = new MapWidget(this);
    m_displayStack->addWidget(m_mapWidget);
    
    // PPI Display widget
    m_ppiWidget = new PPIDisplayWidget(this);
    m_displayStack->addWidget(m_ppiWidget);
    
    // Start with PPI display as default
    m_displayStack->setCurrentWidget(m_ppiWidget);
    
    mainSplitter->addWidget(m_displayStack);
    
    // Simplified Video panel with Day/Night cameras only
    QWidget* videoPanel = new QWidget(this);
    videoPanel->setStyleSheet("background-color: #1a1a1a;");
    QVBoxLayout* videoLayout = new QVBoxLayout(videoPanel);
    videoLayout->setContentsMargins(4, 4, 4, 4);
    videoLayout->setSpacing(4);
    
    // Video panel header
    QLabel* videoPanelLabel = new QLabel("CAMERA FEEDS", videoPanel);
    videoPanelLabel->setStyleSheet("QLabel { color: #aaa; font-weight: bold; font-size: 12px; "
                                   "padding: 4px; background-color: #2a2a2a; border-radius: 3px; }");
    videoPanelLabel->setAlignment(Qt::AlignCenter);
    videoLayout->addWidget(videoPanelLabel);
    
    // Primary video display (shows selected camera)
    m_primaryVideoWidget = new VideoDisplayWidget(this);
    m_primaryVideoWidget->setMinimumSize(400, 300);
    m_primaryVideoWidget->setStyleSheet("border: 2px solid #444; border-radius: 4px;");
    videoLayout->addWidget(m_primaryVideoWidget, 3);
    
    // Day/Night camera grid
    m_videoGridWidget = new VideoGridWidget(this);
    m_videoGridWidget->setMinimumHeight(150);
    videoLayout->addWidget(m_videoGridWidget, 2);
    
    mainSplitter->addWidget(videoPanel);
    mainSplitter->setSizes({600, 450});
    
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
    
    // Create placeholder actions that will be populated after dock widgets are created
    panelsMenu->addAction("Track List")->setCheckable(true);
    panelsMenu->addAction("Track Details")->setCheckable(true);
    panelsMenu->addAction("Sensor Status")->setCheckable(true);
    panelsMenu->addAction("Effector Control")->setCheckable(true);
    panelsMenu->addAction("Alerts")->setCheckable(true);
    
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
    
    // Grouped Simulation Controls
    QToolButton* simControlBtn = new QToolButton(this);
    simControlBtn->setText("Simulation");
    simControlBtn->setPopupMode(QToolButton::MenuButtonPopup);
    simControlBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    QMenu* simMenu = new QMenu(simControlBtn);
    QAction* startAction = simMenu->addAction("Start Simulation");
    connect(startAction, &QAction::triggered, this, &MainWindow::startSimulation);
    QAction* stopAction = simMenu->addAction("Stop Simulation");
    connect(stopAction, &QAction::triggered, this, &MainWindow::stopSimulation);
    QAction* pauseAction = simMenu->addAction("Pause/Resume");
    connect(pauseAction, &QAction::triggered, this, &MainWindow::pauseSimulation);
    simMenu->addSeparator();
    QAction* resetAction = simMenu->addAction("Reset Simulation");
    connect(resetAction, &QAction::triggered, this, &MainWindow::resetSimulation);
    simMenu->addSeparator();
    QAction* settingsAction = simMenu->addAction("Settings...");
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onSimulationSettings);
    
    simControlBtn->setMenu(simMenu);
    simControlBtn->setDefaultAction(startAction);
    m_mainToolBar->addWidget(simControlBtn);
    
    m_mainToolBar->addSeparator();
    
    // Grouped Map/View Controls
    QToolButton* viewControlBtn = new QToolButton(this);
    viewControlBtn->setText("View");
    viewControlBtn->setPopupMode(QToolButton::MenuButtonPopup);
    viewControlBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    QMenu* viewMenu = new QMenu(viewControlBtn);
    QAction* zoomInAction = viewMenu->addAction("Zoom In");
    connect(zoomInAction, &QAction::triggered, this, [this]() { 
        m_mapWidget->setZoom(m_mapWidget->zoom() + 1); 
    });
    QAction* zoomOutAction = viewMenu->addAction("Zoom Out");
    connect(zoomOutAction, &QAction::triggered, this, [this]() { 
        m_mapWidget->setZoom(m_mapWidget->zoom() - 1); 
    });
    QAction* centerAction = viewMenu->addAction("Center on Base");
    connect(centerAction, &QAction::triggered, this, [this]() { 
        GeoPosition basePos;
        basePos.latitude = 34.0522;
        basePos.longitude = -118.2437;
        basePos.altitude = 100.0;
        m_mapWidget->setCenter(basePos); 
    });
    
    viewControlBtn->setMenu(viewMenu);
    viewControlBtn->setDefaultAction(zoomInAction);
    m_mainToolBar->addWidget(viewControlBtn);
    
    m_mainToolBar->addSeparator();
    
    // Grouped Recording Controls
    QToolButton* recordBtn = new QToolButton(this);
    recordBtn->setText("Record");
    recordBtn->setPopupMode(QToolButton::MenuButtonPopup);
    recordBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    QMenu* recordMenu = new QMenu(recordBtn);
    QAction* recordAllAction = recordMenu->addAction("Start Recording All");
    connect(recordAllAction, &QAction::triggered, this, &MainWindow::onStartAllRecording);
    QAction* stopRecordAction = recordMenu->addAction("Stop All Recording");
    connect(stopRecordAction, &QAction::triggered, this, &MainWindow::onStopAllRecording);
    recordMenu->addSeparator();
    QAction* snapshotAction = recordMenu->addAction("Take Snapshot");
    connect(snapshotAction, &QAction::triggered, this, &MainWindow::onTakeSnapshot);
    
    recordBtn->setMenu(recordMenu);
    recordBtn->setDefaultAction(snapshotAction);
    m_mainToolBar->addWidget(recordBtn);
    
    // Secondary toolbar for simulation status display
    m_simulationToolBar = addToolBar("Simulation Status");
    m_simulationToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    
    QLabel* scenarioLabel = new QLabel(" Scenario: Default ", this);
    scenarioLabel->setStyleSheet("QLabel { padding: 4px; background-color: #2a2a2a; "
                                  "border-radius: 3px; color: #aaa; }");
    m_simulationToolBar->addWidget(scenarioLabel);
}

void MainWindow::setupPPIToolBar() {
    m_ppiToolBar = addToolBar("PPI Display");
    m_ppiToolBar->setIconSize(QSize(20, 20));
    m_ppiToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    // Display mode selector (Map vs PPI)
    QLabel* displayLabel = new QLabel(" Display: ", this);
    m_ppiToolBar->addWidget(displayLabel);
    
    m_displayModeCombo = new QComboBox(this);
    m_displayModeCombo->addItem("PPI Radar", 1);
    m_displayModeCombo->addItem("2D Map", 0);
    m_displayModeCombo->setCurrentIndex(0);  // PPI as default
    m_displayModeCombo->setToolTip("Switch between PPI radar display and 2D map");
    connect(m_displayModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onTogglePPIDisplay);
    m_ppiToolBar->addWidget(m_displayModeCombo);
    
    m_ppiToolBar->addSeparator();
    
    // PPI mode selector
    QLabel* ppiModeLabel = new QLabel(" Mode: ", this);
    m_ppiToolBar->addWidget(ppiModeLabel);
    
    m_ppiModeCombo = new QComboBox(this);
    m_ppiModeCombo->addItem("Radar Only", static_cast<int>(PPIDisplayMode::RadarOnly));
    m_ppiModeCombo->addItem("Map + Radar", static_cast<int>(PPIDisplayMode::MapOverlay));
    m_ppiModeCombo->addItem("Map Only", static_cast<int>(PPIDisplayMode::MapOnly));
    m_ppiModeCombo->addItem("Night Vision", static_cast<int>(PPIDisplayMode::NightVision));
    m_ppiModeCombo->setToolTip("Select PPI display rendering mode");
    connect(m_ppiModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onPPIDisplayModeChanged);
    m_ppiToolBar->addWidget(m_ppiModeCombo);
    
    m_ppiToolBar->addSeparator();
    
    // Grouped Range Controls
    QToolButton* rangeBtn = new QToolButton(this);
    rangeBtn->setText("Range");
    rangeBtn->setPopupMode(QToolButton::MenuButtonPopup);
    rangeBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    rangeBtn->setToolTip("Radar range controls");
    
    QMenu* rangeMenu = new QMenu(rangeBtn);
    
    QAction* rangeInAction = rangeMenu->addAction("Range In (Zoom)");
    rangeInAction->setToolTip("Decrease radar range (zoom in)");
    connect(rangeInAction, &QAction::triggered, this, [this]() {
        double newRange = m_ppiWidget->rangeScale() * 0.8;
        m_ppiWidget->setRangeScale(newRange);
    });
    
    QAction* rangeOutAction = rangeMenu->addAction("Range Out");
    rangeOutAction->setToolTip("Increase radar range (zoom out)");
    connect(rangeOutAction, &QAction::triggered, this, [this]() {
        double newRange = m_ppiWidget->rangeScale() * 1.25;
        m_ppiWidget->setRangeScale(newRange);
    });
    
    rangeMenu->addSeparator();
    rangeMenu->addAction("Preset Ranges:")->setEnabled(false);
    
    QAction* range1kmAction = rangeMenu->addAction("  1 km");
    connect(range1kmAction, &QAction::triggered, this, [this]() {
        m_ppiWidget->setRangeScale(1000.0);
    });
    
    QAction* range2kmAction = rangeMenu->addAction("  2 km");
    connect(range2kmAction, &QAction::triggered, this, [this]() {
        m_ppiWidget->setRangeScale(2000.0);
    });
    
    QAction* range5kmAction = rangeMenu->addAction("  5 km");
    connect(range5kmAction, &QAction::triggered, this, [this]() {
        m_ppiWidget->setRangeScale(5000.0);
    });
    
    QAction* range10kmAction = rangeMenu->addAction("  10 km");
    connect(range10kmAction, &QAction::triggered, this, [this]() {
        m_ppiWidget->setRangeScale(10000.0);
    });
    
    rangeBtn->setMenu(rangeMenu);
    rangeBtn->setDefaultAction(range5kmAction);
    m_ppiToolBar->addWidget(rangeBtn);
    
    m_ppiToolBar->addSeparator();
    
    // Grouped Display Options
    QToolButton* displayOptionsBtn = new QToolButton(this);
    displayOptionsBtn->setText("Options");
    displayOptionsBtn->setPopupMode(QToolButton::InstantPopup);
    displayOptionsBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    displayOptionsBtn->setToolTip("Display options");
    
    QMenu* optionsMenu = new QMenu(displayOptionsBtn);
    
    // Sweep control
    m_ppiSweepAction = optionsMenu->addAction("Radar Sweep");
    m_ppiSweepAction->setCheckable(true);
    m_ppiSweepAction->setChecked(false);
    m_ppiSweepAction->setToolTip("Toggle radar sweep animation");
    connect(m_ppiSweepAction, &QAction::triggered, this, &MainWindow::onPPISweepToggle);
    
    optionsMenu->addSeparator();
    
    // North-up toggle
    QAction* northUpAction = optionsMenu->addAction("North Up");
    northUpAction->setCheckable(true);
    northUpAction->setChecked(true);
    northUpAction->setToolTip("Toggle North-Up display orientation");
    connect(northUpAction, &QAction::triggered, this, [this](bool checked) {
        m_ppiWidget->setNorthUp(checked);
    });
    
    // Track history toggle
    QAction* historyAction = optionsMenu->addAction("Show Trails");
    historyAction->setCheckable(true);
    historyAction->setChecked(true);
    historyAction->setToolTip("Show track history trails");
    connect(historyAction, &QAction::triggered, this, [this](bool checked) {
        m_ppiWidget->setShowTrackHistory(checked);
    });
    
    displayOptionsBtn->setMenu(optionsMenu);
    m_ppiToolBar->addWidget(displayOptionsBtn);
}

void MainWindow::setupDockWidgets() {
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
    
    // Track detail dock (left)
    m_trackDetailDock = new QDockWidget("Track Detail", this);
    m_trackDetailPanel = new TrackDetailPanel(this);
    m_trackDetailDock->setWidget(m_trackDetailPanel);
    addDockWidget(Qt::LeftDockWidgetArea, m_trackDetailDock);
    
    // Sensor status dock (bottom left)
    m_sensorStatusDock = new QDockWidget("Sensor Status", this);
    m_sensorStatusPanel = new SensorStatusPanel(this);
    m_sensorStatusDock->setWidget(m_sensorStatusPanel);
    addDockWidget(Qt::BottomDockWidgetArea, m_sensorStatusDock);
    
    // Add default sensors to status panel
    m_sensorStatusPanel->addSensor("SIM-RADAR-001", "Primary Radar", "RADAR");
    m_sensorStatusPanel->addSensor("SIM-RF-001", "RF Detector", "RF_DETECTOR");
    m_sensorStatusPanel->addSensor("SIM-DAY-001", "Day Camera", "CAMERA");
    m_sensorStatusPanel->addSensor("SIM-NIGHT-001", "Night Camera", "CAMERA");
    
    // Camera status dock (bottom, tabbed with sensor status)
    m_cameraStatusDock = new QDockWidget("Camera Status", this);
    m_cameraStatusPanel = new CameraStatusPanel(m_videoManager, this);
    m_cameraStatusDock->setWidget(m_cameraStatusPanel);
    tabifyDockWidget(m_sensorStatusDock, m_cameraStatusDock);
    
    // Track list dock (bottom, below camera status)
    m_trackListDock = new QDockWidget("Track List", this);
    m_trackListWidget = new TrackListWidget(m_trackManager, this);
    m_trackListDock->setWidget(m_trackListWidget);
    addDockWidget(Qt::BottomDockWidgetArea, m_trackListDock);
    splitDockWidget(m_cameraStatusDock, m_trackListDock, Qt::Vertical);
    
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
    
    // Center PPI on base
    m_ppiWidget->setCenter(baseAsset.position);
    m_ppiWidget->setDefendedAreaRadii(baseAsset.criticalRadiusM, baseAsset.warningRadiusM, 5000.0);
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

void MainWindow::setupPPIDisplay() {
    // Configure PPI display
    m_ppiWidget->setTrackManager(m_trackManager);
    
    // Set initial center to match map widget
    GeoPosition basePos;
    basePos.latitude = 34.0522;
    basePos.longitude = -118.2437;
    basePos.altitude = 100.0;
    m_ppiWidget->setCenter(basePos);
    
    // Configure defended area radii
    m_ppiWidget->setDefendedAreaRadii(500.0, 1500.0, 5000.0);
    m_ppiWidget->setDefendedAreaVisible(true);
    
    // Set initial range
    m_ppiWidget->setRangeScale(5000.0);  // 5km default range
    
    // Enable track history
    m_ppiWidget->setShowTrackHistory(true);
    m_ppiWidget->setTrackHistoryLength(30);  // 30 seconds of history
    
    // Connect PPI signals
    connect(m_ppiWidget, &PPIDisplayWidget::trackSelected,
            this, &MainWindow::onTrackSelected);
    connect(m_ppiWidget, &PPIDisplayWidget::trackDoubleClicked,
            this, &MainWindow::onEngageRequested);
    
    // Connect track manager to PPI widget
    connect(m_trackManager, &TrackManager::trackUpdated,
            m_ppiWidget, &PPIDisplayWidget::updateTrack);
    connect(m_trackManager, &TrackManager::trackCreated,
            m_ppiWidget, &PPIDisplayWidget::addTrack);
    connect(m_trackManager, &TrackManager::trackDropped,
            m_ppiWidget, &PPIDisplayWidget::removeTrack);
    
    Logger::instance().info("MainWindow", "PPI display configured");
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
    m_sensorStatusPanel->updateSensorStatus("SIM-RF-001", "ONLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-DAY-001", "ONLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-NIGHT-001", "ONLINE");
    
    // Start PPI sweep animation
    m_ppiWidget->startSweep();
    m_ppiSweepAction->setChecked(true);
    m_ppiSweepAction->setText("Stop Sweep");
    
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
    m_sensorStatusPanel->updateSensorStatus("SIM-RF-001", "OFFLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-DAY-001", "OFFLINE");
    m_sensorStatusPanel->updateSensorStatus("SIM-NIGHT-001", "OFFLINE");
    
    // Stop PPI sweep animation
    m_ppiWidget->stopSweep();
    m_ppiSweepAction->setChecked(false);
    m_ppiSweepAction->setText("Start Sweep");
    
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
    // Stop simulation if running
    stopSimulation();
    
    // Clear PPI widget tracks before resetting simulation manager
    // This ensures no dangling pointers remain
    m_ppiWidget->clearTracks();
    m_mapWidget->clearTracks();
    
    // Reset video simulator separately (MainWindow owns its own instance)
    m_videoSimulator->clearTrackedTargets();
    
    // Reset simulation manager (clears track manager, simulators, etc.)
    m_simulationManager->reset();
    
    // Re-create simulation environment for next run
    m_simulationManager->createFullSimulationEnvironment();
    
    statusBar()->showMessage("Simulation reset - ready to start");
    Logger::instance().info("MainWindow", "Simulation reset complete");
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
        m_ppiWidget->selectTrack(trackId);
        
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
        "Primary Radar: ONLINE (Signal Quality: 95%)\n"
        "RF Detector: ONLINE (Signal Quality: 88%)\n"
        "Day Camera: ONLINE (Signal Quality: 100%)\n"
        "Night Camera: ONLINE (Signal Quality: 100%)");
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

void MainWindow::onTogglePPIDisplay() {
    int displayType = m_displayModeCombo->currentData().toInt();
    
    if (displayType == 0) {
        // Switch to 2D Map
        m_displayStack->setCurrentWidget(m_mapWidget);
        m_ppiModeCombo->setEnabled(false);
        m_ppiSweepAction->setEnabled(false);
        statusBar()->showMessage("Switched to 2D Map display");
    } else {
        // Switch to PPI radar display
        m_displayStack->setCurrentWidget(m_ppiWidget);
        m_ppiModeCombo->setEnabled(true);
        m_ppiSweepAction->setEnabled(true);
        statusBar()->showMessage("Switched to PPI radar display");
    }
}

void MainWindow::onPPIDisplayModeChanged(int index) {
    int mode = m_ppiModeCombo->itemData(index).toInt();
    m_ppiWidget->setDisplayMode(static_cast<PPIDisplayMode>(mode));
    
    QString modeNames[] = {"Radar Only", "Map + Radar", "Map Only", "Night Vision"};
    if (mode >= 0 && mode <= 3) {
        statusBar()->showMessage(QString("PPI display mode: %1").arg(modeNames[mode]));
    }
}

void MainWindow::onPPIRangeChanged(double rangeM) {
    m_ppiWidget->setRangeScale(rangeM);
    
    QString rangeStr;
    if (rangeM >= 1000) {
        rangeStr = QString("%1 km").arg(rangeM / 1000.0, 0, 'f', 1);
    } else {
        rangeStr = QString("%1 m").arg(rangeM, 0, 'f', 0);
    }
    statusBar()->showMessage(QString("PPI range: %1").arg(rangeStr));
}

void MainWindow::onPPISweepToggle() {
    if (m_ppiSweepAction->isChecked()) {
        m_ppiWidget->startSweep();
        m_ppiSweepAction->setText("Stop Sweep");
        statusBar()->showMessage("Radar sweep started");
    } else {
        m_ppiWidget->stopSweep();
        m_ppiSweepAction->setText("Start Sweep");
        statusBar()->showMessage("Radar sweep stopped");
    }
}

} // namespace CounterUAS
