#include "ui/QuickActionsPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

namespace CounterUAS {

QuickActionsPanel::QuickActionsPanel(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void QuickActionsPanel::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);
    
    // Panel title with consistent styling
    QLabel* titleLabel = new QLabel("QUICK ACTIONS", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 11px; color: #00d4ff; "
                             "padding: 6px; background-color: #1a2a35; border-radius: 4px; "
                             "border: 1px solid #2a4a5a; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Create grouped sections
    createSimulationGroup();
    createDisplayGroup();
    createRecordingGroup();
    createEngagementGroup();
    
    mainLayout->addWidget(m_simGroup);
    mainLayout->addWidget(m_displayGroup);
    mainLayout->addWidget(m_recordGroup);
    mainLayout->addWidget(m_engageGroup);
    mainLayout->addStretch();
}

void QuickActionsPanel::createSimulationGroup() {
    m_simGroup = new QGroupBox("Simulation", this);
    m_simGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #2a4a5a; border-radius: 6px; "
        "background-color: #0d1a20; margin-top: 10px; padding: 12px 8px 8px 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 2px 8px; "
        "color: #00d4ff; background-color: #0d1a20; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_simGroup);
    layout->setSpacing(6);
    layout->setContentsMargins(10, 14, 10, 10);
    
    // Status label
    m_simStatusLabel = new QLabel("Status: STOPPED", this);
    m_simStatusLabel->setStyleSheet("QLabel { color: #888; font-size: 10px; }");
    m_simStatusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_simStatusLabel);
    
    // Control buttons in 2x2 grid
    QGridLayout* btnGrid = new QGridLayout();
    btnGrid->setSpacing(4);
    
    m_startBtn = new QPushButton("START", this);
    m_startBtn->setStyleSheet(
        "QPushButton { background-color: #2a6a2a; color: white; font-weight: bold; "
        "padding: 8px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #3a8a3a; }"
        "QPushButton:disabled { background-color: #444; color: #888; }");
    connect(m_startBtn, &QPushButton::clicked, this, &QuickActionsPanel::startSimulation);
    btnGrid->addWidget(m_startBtn, 0, 0);
    
    m_stopBtn = new QPushButton("STOP", this);
    m_stopBtn->setStyleSheet(
        "QPushButton { background-color: #6a2a2a; color: white; font-weight: bold; "
        "padding: 8px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #8a3a3a; }"
        "QPushButton:disabled { background-color: #444; color: #888; }");
    m_stopBtn->setEnabled(false);
    connect(m_stopBtn, &QPushButton::clicked, this, &QuickActionsPanel::stopSimulation);
    btnGrid->addWidget(m_stopBtn, 0, 1);
    
    m_pauseBtn = new QPushButton("PAUSE", this);
    m_pauseBtn->setStyleSheet(
        "QPushButton { background-color: #5a5a2a; color: white; font-weight: bold; "
        "padding: 8px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #7a7a3a; }"
        "QPushButton:disabled { background-color: #444; color: #888; }");
    m_pauseBtn->setEnabled(false);
    connect(m_pauseBtn, &QPushButton::clicked, this, &QuickActionsPanel::pauseSimulation);
    btnGrid->addWidget(m_pauseBtn, 1, 0);
    
    m_resetBtn = new QPushButton("RESET", this);
    m_resetBtn->setStyleSheet(
        "QPushButton { background-color: #3a3a5a; color: white; font-weight: bold; "
        "padding: 8px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #4a4a7a; }");
    connect(m_resetBtn, &QPushButton::clicked, this, &QuickActionsPanel::resetSimulation);
    btnGrid->addWidget(m_resetBtn, 1, 1);
    
    layout->addLayout(btnGrid);
}

void QuickActionsPanel::createDisplayGroup() {
    m_displayGroup = new QGroupBox("Display", this);
    m_displayGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #2a4a5a; border-radius: 6px; "
        "background-color: #0d1a20; margin-top: 10px; padding: 12px 8px 8px 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 2px 8px; "
        "color: #00d4ff; background-color: #0d1a20; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_displayGroup);
    layout->setSpacing(6);
    layout->setContentsMargins(10, 14, 10, 10);
    
    // Display mode selector
    QHBoxLayout* modeLayout = new QHBoxLayout();
    QLabel* modeLabel = new QLabel("Mode:", this);
    modeLabel->setStyleSheet("QLabel { color: #ccc; }");
    modeLayout->addWidget(modeLabel);
    
    m_displayModeCombo = new QComboBox(this);
    m_displayModeCombo->addItem("PPI Radar", 1);
    m_displayModeCombo->addItem("2D Map", 0);
    m_displayModeCombo->setStyleSheet(
        "QComboBox { background-color: #333; color: #ccc; padding: 4px; border-radius: 3px; }");
    connect(m_displayModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                emit displayModeChanged(m_displayModeCombo->itemData(index).toInt());
            });
    modeLayout->addWidget(m_displayModeCombo, 1);
    layout->addLayout(modeLayout);
    
    // Zoom controls in a row
    QHBoxLayout* zoomLayout = new QHBoxLayout();
    zoomLayout->setSpacing(4);
    
    m_zoomInBtn = new QPushButton("+", this);
    m_zoomInBtn->setFixedSize(32, 28);
    m_zoomInBtn->setStyleSheet(
        "QPushButton { background-color: #3a4a5a; color: white; font-weight: bold; "
        "font-size: 14px; border-radius: 3px; }"
        "QPushButton:hover { background-color: #4a5a6a; }");
    m_zoomInBtn->setToolTip("Zoom In");
    connect(m_zoomInBtn, &QPushButton::clicked, this, &QuickActionsPanel::zoomIn);
    zoomLayout->addWidget(m_zoomInBtn);
    
    m_zoomOutBtn = new QPushButton("-", this);
    m_zoomOutBtn->setFixedSize(32, 28);
    m_zoomOutBtn->setStyleSheet(
        "QPushButton { background-color: #3a4a5a; color: white; font-weight: bold; "
        "font-size: 14px; border-radius: 3px; }"
        "QPushButton:hover { background-color: #4a5a6a; }");
    m_zoomOutBtn->setToolTip("Zoom Out");
    connect(m_zoomOutBtn, &QPushButton::clicked, this, &QuickActionsPanel::zoomOut);
    zoomLayout->addWidget(m_zoomOutBtn);
    
    m_centerBtn = new QPushButton("Center", this);
    m_centerBtn->setStyleSheet(
        "QPushButton { background-color: #3a4a5a; color: white; padding: 4px 8px; "
        "border-radius: 3px; }"
        "QPushButton:hover { background-color: #4a5a6a; }");
    m_centerBtn->setToolTip("Center on Base");
    connect(m_centerBtn, &QPushButton::clicked, this, &QuickActionsPanel::centerView);
    zoomLayout->addWidget(m_centerBtn, 1);
    
    layout->addLayout(zoomLayout);
}

void QuickActionsPanel::createRecordingGroup() {
    m_recordGroup = new QGroupBox("Recording", this);
    m_recordGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #2a4a5a; border-radius: 6px; "
        "background-color: #0d1a20; margin-top: 10px; padding: 12px 8px 8px 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 2px 8px; "
        "color: #00d4ff; background-color: #0d1a20; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_recordGroup);
    layout->setSpacing(6);
    layout->setContentsMargins(10, 14, 10, 10);
    
    // Status label
    m_recordStatusLabel = new QLabel("Not Recording", this);
    m_recordStatusLabel->setStyleSheet("QLabel { color: #888; font-size: 10px; }");
    m_recordStatusLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_recordStatusLabel);
    
    // Record and Snapshot buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(4);
    
    m_recordBtn = new QPushButton("RECORD", this);
    m_recordBtn->setStyleSheet(
        "QPushButton { background-color: #8a2a2a; color: white; font-weight: bold; "
        "padding: 6px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #aa3a3a; }"
        "QPushButton:checked { background-color: #cc4444; }");
    m_recordBtn->setCheckable(true);
    connect(m_recordBtn, &QPushButton::clicked, this, [this](bool checked) {
        if (checked) {
            emit startRecording();
        } else {
            emit stopRecording();
        }
    });
    btnLayout->addWidget(m_recordBtn);
    
    m_snapshotBtn = new QPushButton("SNAPSHOT", this);
    m_snapshotBtn->setStyleSheet(
        "QPushButton { background-color: #2a4a6a; color: white; font-weight: bold; "
        "padding: 6px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #3a5a7a; }");
    connect(m_snapshotBtn, &QPushButton::clicked, this, &QuickActionsPanel::takeSnapshot);
    btnLayout->addWidget(m_snapshotBtn);
    
    layout->addLayout(btnLayout);
}

void QuickActionsPanel::createEngagementGroup() {
    m_engageGroup = new QGroupBox("Engagement", this);
    m_engageGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #4a2a2a; border-radius: 6px; "
        "background-color: #0d1a20; margin-top: 10px; padding: 12px 8px 8px 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 2px 8px; "
        "color: #ff6666; background-color: #0d1a20; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_engageGroup);
    layout->setSpacing(6);
    layout->setContentsMargins(10, 14, 10, 10);
    
    // Engage button (large, prominent)
    m_engageBtn = new QPushButton("ENGAGE TARGET", this);
    m_engageBtn->setStyleSheet(
        "QPushButton { background-color: #aa2222; color: white; font-weight: bold; "
        "font-size: 12px; padding: 10px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #cc3333; }"
        "QPushButton:disabled { background-color: #555; color: #888; }");
    m_engageBtn->setToolTip("Engage selected track with selected effector");
    connect(m_engageBtn, &QPushButton::clicked, this, &QuickActionsPanel::engageSelected);
    layout->addWidget(m_engageBtn);
    
    // Abort button
    m_abortBtn = new QPushButton("ABORT", this);
    m_abortBtn->setStyleSheet(
        "QPushButton { background-color: #8a6a2a; color: white; font-weight: bold; "
        "padding: 6px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #aa8a3a; }"
        "QPushButton:disabled { background-color: #444; color: #888; }");
    m_abortBtn->setEnabled(false);
    m_abortBtn->setToolTip("Abort current engagement");
    connect(m_abortBtn, &QPushButton::clicked, this, &QuickActionsPanel::abortEngagement);
    layout->addWidget(m_abortBtn);
}

void QuickActionsPanel::setSimulationRunning(bool running) {
    m_simulationRunning = running;
    updateSimulationButtons();
    
    if (running) {
        m_simStatusLabel->setText("Status: RUNNING");
        m_simStatusLabel->setStyleSheet("QLabel { color: #4a4; font-size: 10px; font-weight: bold; }");
    } else {
        m_simStatusLabel->setText("Status: STOPPED");
        m_simStatusLabel->setStyleSheet("QLabel { color: #888; font-size: 10px; }");
    }
}

void QuickActionsPanel::setSimulationPaused(bool paused) {
    m_simulationPaused = paused;
    updateSimulationButtons();
    
    if (paused && m_simulationRunning) {
        m_simStatusLabel->setText("Status: PAUSED");
        m_simStatusLabel->setStyleSheet("QLabel { color: #aa4; font-size: 10px; font-weight: bold; }");
        m_pauseBtn->setText("RESUME");
    } else if (m_simulationRunning) {
        m_pauseBtn->setText("PAUSE");
    }
}

void QuickActionsPanel::setRecordingActive(bool active) {
    m_recordingActive = active;
    m_recordBtn->setChecked(active);
    
    if (active) {
        m_recordStatusLabel->setText("RECORDING...");
        m_recordStatusLabel->setStyleSheet("QLabel { color: #f44; font-size: 10px; font-weight: bold; }");
    } else {
        m_recordStatusLabel->setText("Not Recording");
        m_recordStatusLabel->setStyleSheet("QLabel { color: #888; font-size: 10px; }");
    }
}

void QuickActionsPanel::setDisplayMode(int mode) {
    for (int i = 0; i < m_displayModeCombo->count(); ++i) {
        if (m_displayModeCombo->itemData(i).toInt() == mode) {
            m_displayModeCombo->setCurrentIndex(i);
            break;
        }
    }
}

void QuickActionsPanel::updateSimulationButtons() {
    m_startBtn->setEnabled(!m_simulationRunning);
    m_stopBtn->setEnabled(m_simulationRunning);
    m_pauseBtn->setEnabled(m_simulationRunning);
}

} // namespace CounterUAS
