/**
 * @file SimulationSettingsDialog.cpp
 * @brief Implementation of simulation settings dialog
 */

#include "ui/dialogs/SimulationSettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

namespace CounterUAS {

SimulationSettingsDialog::SimulationSettingsDialog(SystemSimulationManager* simManager, QWidget* parent)
    : QDialog(parent)
    , m_simManager(simManager)
{
    setWindowTitle("Simulation Settings");
    setMinimumSize(600, 700);
    
    setupUI();
    
    // Load current scenario
    if (m_simManager) {
        loadScenarioToUI(m_simManager->currentScenario());
    }
}

void SimulationSettingsDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Scenario selection
    QGroupBox* scenarioGroup = new QGroupBox("Scenario", this);
    QHBoxLayout* scenarioLayout = new QHBoxLayout(scenarioGroup);
    
    m_scenarioCombo = new QComboBox(this);
    m_scenarioCombo->addItem("Default Scenario");
    m_scenarioCombo->addItem("Swarm Attack");
    m_scenarioCombo->addItem("Multi-Threat Environment");
    m_scenarioCombo->addItem("Stress Test");
    m_scenarioCombo->addItem("Custom");
    connect(m_scenarioCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SimulationSettingsDialog::onScenarioSelected);
    scenarioLayout->addWidget(m_scenarioCombo);
    
    QPushButton* loadBtn = new QPushButton("Load...", this);
    connect(loadBtn, &QPushButton::clicked, this, &SimulationSettingsDialog::onLoadCustom);
    scenarioLayout->addWidget(loadBtn);
    
    QPushButton* saveBtn = new QPushButton("Save...", this);
    connect(saveBtn, &QPushButton::clicked, this, &SimulationSettingsDialog::onSaveCustom);
    scenarioLayout->addWidget(saveBtn);
    
    mainLayout->addWidget(scenarioGroup);
    
    // General settings
    QGroupBox* generalGroup = new QGroupBox("General", this);
    QFormLayout* generalLayout = new QFormLayout(generalGroup);
    
    m_durationMinutes = new QSpinBox(this);
    m_durationMinutes->setRange(1, 120);
    m_durationMinutes->setValue(10);
    m_durationMinutes->setSuffix(" min");
    generalLayout->addRow("Duration:", m_durationMinutes);
    
    m_maxTargets = new QSpinBox(this);
    m_maxTargets->setRange(1, 100);
    m_maxTargets->setValue(10);
    generalLayout->addRow("Max Targets:", m_maxTargets);
    
    m_spawnRate = new QDoubleSpinBox(this);
    m_spawnRate->setRange(0.01, 5.0);
    m_spawnRate->setSingleStep(0.1);
    m_spawnRate->setValue(0.2);
    m_spawnRate->setSuffix(" /sec");
    generalLayout->addRow("Spawn Rate:", m_spawnRate);
    
    m_timeScale = new QDoubleSpinBox(this);
    m_timeScale->setRange(0.1, 10.0);
    m_timeScale->setSingleStep(0.1);
    m_timeScale->setValue(1.0);
    m_timeScale->setSuffix("x");
    generalLayout->addRow("Time Scale:", m_timeScale);
    
    mainLayout->addWidget(generalGroup);
    
    // Environment settings
    QGroupBox* envGroup = new QGroupBox("Environment", this);
    QFormLayout* envLayout = new QFormLayout(envGroup);
    
    QHBoxLayout* weatherLayout = new QHBoxLayout();
    m_weatherSlider = new QSlider(Qt::Horizontal, this);
    m_weatherSlider->setRange(0, 100);
    m_weatherSlider->setValue(100);
    m_weatherLabel = new QLabel("100%", this);
    connect(m_weatherSlider, &QSlider::valueChanged, this, [this](int v) {
        m_weatherLabel->setText(QString("%1%").arg(v));
    });
    weatherLayout->addWidget(m_weatherSlider);
    weatherLayout->addWidget(m_weatherLabel);
    envLayout->addRow("Weather:", weatherLayout);
    
    QHBoxLayout* clutterLayout = new QHBoxLayout();
    m_clutterSlider = new QSlider(Qt::Horizontal, this);
    m_clutterSlider->setRange(0, 100);
    m_clutterSlider->setValue(10);
    m_clutterLabel = new QLabel("10%", this);
    connect(m_clutterSlider, &QSlider::valueChanged, this, [this](int v) {
        m_clutterLabel->setText(QString("%1%").arg(v));
    });
    clutterLayout->addWidget(m_clutterSlider);
    clutterLayout->addWidget(m_clutterLabel);
    envLayout->addRow("Clutter:", clutterLayout);
    
    QHBoxLayout* noiseLayout = new QHBoxLayout();
    m_noiseSlider = new QSlider(Qt::Horizontal, this);
    m_noiseSlider->setRange(0, 100);
    m_noiseSlider->setValue(5);
    m_noiseLabel = new QLabel("5%", this);
    connect(m_noiseSlider, &QSlider::valueChanged, this, [this](int v) {
        m_noiseLabel->setText(QString("%1%").arg(v));
    });
    noiseLayout->addWidget(m_noiseSlider);
    noiseLayout->addWidget(m_noiseLabel);
    envLayout->addRow("Noise:", noiseLayout);
    
    mainLayout->addWidget(envGroup);
    
    // Threat parameters
    QGroupBox* threatGroup = new QGroupBox("Threat Parameters", this);
    QFormLayout* threatLayout = new QFormLayout(threatGroup);
    
    m_minThreatLevel = new QSpinBox(this);
    m_minThreatLevel->setRange(1, 5);
    m_minThreatLevel->setValue(1);
    threatLayout->addRow("Min Threat Level:", m_minThreatLevel);
    
    m_maxThreatLevel = new QSpinBox(this);
    m_maxThreatLevel->setRange(1, 5);
    m_maxThreatLevel->setValue(5);
    threatLayout->addRow("Max Threat Level:", m_maxThreatLevel);
    
    m_hostileProbability = new QDoubleSpinBox(this);
    m_hostileProbability->setRange(0, 1);
    m_hostileProbability->setSingleStep(0.05);
    m_hostileProbability->setValue(0.7);
    threatLayout->addRow("Hostile Probability:", m_hostileProbability);
    
    mainLayout->addWidget(threatGroup);
    
    // Simulator enables
    QGroupBox* simGroup = new QGroupBox("Enabled Simulators", this);
    QVBoxLayout* simLayout = new QVBoxLayout(simGroup);
    
    m_enableRadar = new QCheckBox("Radar Simulation", this);
    m_enableRadar->setChecked(true);
    simLayout->addWidget(m_enableRadar);
    
    m_enableRF = new QCheckBox("RF Detection Simulation", this);
    m_enableRF->setChecked(true);
    simLayout->addWidget(m_enableRF);
    
    m_enableCamera = new QCheckBox("Camera Simulation", this);
    m_enableCamera->setChecked(true);
    simLayout->addWidget(m_enableCamera);
    
    m_enableVideo = new QCheckBox("Video Feed Simulation", this);
    m_enableVideo->setChecked(true);
    simLayout->addWidget(m_enableVideo);
    
    m_enableEffectors = new QCheckBox("Effector Simulation", this);
    m_enableEffectors->setChecked(true);
    simLayout->addWidget(m_enableEffectors);
    
    mainLayout->addWidget(simGroup);
    
    // Base position
    QGroupBox* posGroup = new QGroupBox("Base Position", this);
    QFormLayout* posLayout = new QFormLayout(posGroup);
    
    m_baseLat = new QDoubleSpinBox(this);
    m_baseLat->setRange(-90, 90);
    m_baseLat->setDecimals(6);
    m_baseLat->setValue(34.0522);
    m_baseLat->setSuffix("°");
    posLayout->addRow("Latitude:", m_baseLat);
    
    m_baseLon = new QDoubleSpinBox(this);
    m_baseLon->setRange(-180, 180);
    m_baseLon->setDecimals(6);
    m_baseLon->setValue(-118.2437);
    m_baseLon->setSuffix("°");
    posLayout->addRow("Longitude:", m_baseLon);
    
    m_baseAlt = new QDoubleSpinBox(this);
    m_baseAlt->setRange(0, 10000);
    m_baseAlt->setValue(100);
    m_baseAlt->setSuffix(" m");
    posLayout->addRow("Altitude:", m_baseAlt);
    
    mainLayout->addWidget(posGroup);
    
    // Dialog buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* applyBtn = new QPushButton("Apply", this);
    connect(applyBtn, &QPushButton::clicked, this, &SimulationSettingsDialog::onApply);
    QPushButton* okBtn = new QPushButton("OK", this);
    connect(okBtn, &QPushButton::clicked, this, [this]() {
        onApply();
        accept();
    });
    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    
    btnLayout->addStretch();
    btnLayout->addWidget(applyBtn);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);
}

void SimulationSettingsDialog::loadScenarioToUI(const SimulationScenario& scenario) {
    m_durationMinutes->setValue(scenario.durationMinutes);
    m_maxTargets->setValue(scenario.maxTargets);
    m_spawnRate->setValue(scenario.threatSpawnRate);
    
    m_weatherSlider->setValue(static_cast<int>(scenario.weatherFactor * 100));
    m_clutterSlider->setValue(static_cast<int>(scenario.clutterLevel * 100));
    m_noiseSlider->setValue(static_cast<int>(scenario.noiseLevel * 100));
    
    m_minThreatLevel->setValue(scenario.minThreatLevel);
    m_maxThreatLevel->setValue(scenario.maxThreatLevel);
    m_hostileProbability->setValue(scenario.hostileProbability);
    
    m_enableRadar->setChecked(scenario.enableRadarSim);
    m_enableRF->setChecked(scenario.enableRFSim);
    m_enableCamera->setChecked(scenario.enableCameraSim);
    m_enableVideo->setChecked(scenario.enableVideoSim);
    m_enableEffectors->setChecked(scenario.enableEffectorSim);
    
    m_baseLat->setValue(scenario.basePosition.latitude);
    m_baseLon->setValue(scenario.basePosition.longitude);
    m_baseAlt->setValue(scenario.basePosition.altitude);
}

SimulationScenario SimulationSettingsDialog::getScenario() const {
    SimulationScenario scenario;
    scenario.name = m_scenarioCombo->currentText();
    scenario.durationMinutes = m_durationMinutes->value();
    scenario.maxTargets = m_maxTargets->value();
    scenario.threatSpawnRate = m_spawnRate->value();
    
    scenario.weatherFactor = m_weatherSlider->value() / 100.0;
    scenario.clutterLevel = m_clutterSlider->value() / 100.0;
    scenario.noiseLevel = m_noiseSlider->value() / 100.0;
    
    scenario.minThreatLevel = m_minThreatLevel->value();
    scenario.maxThreatLevel = m_maxThreatLevel->value();
    scenario.hostileProbability = m_hostileProbability->value();
    
    scenario.enableRadarSim = m_enableRadar->isChecked();
    scenario.enableRFSim = m_enableRF->isChecked();
    scenario.enableCameraSim = m_enableCamera->isChecked();
    scenario.enableVideoSim = m_enableVideo->isChecked();
    scenario.enableEffectorSim = m_enableEffectors->isChecked();
    
    scenario.basePosition.latitude = m_baseLat->value();
    scenario.basePosition.longitude = m_baseLon->value();
    scenario.basePosition.altitude = m_baseAlt->value();
    
    return scenario;
}

void SimulationSettingsDialog::onScenarioSelected(int index) {
    if (!m_simManager) return;
    
    switch (index) {
        case 0:
            m_simManager->loadDefaultScenario();
            break;
        case 1:
            m_simManager->loadSwarmAttackScenario();
            break;
        case 2:
            m_simManager->loadMultiThreatScenario();
            break;
        case 3:
            m_simManager->loadStressTestScenario();
            break;
        default:
            return;  // Custom - don't change
    }
    
    loadScenarioToUI(m_simManager->currentScenario());
}

void SimulationSettingsDialog::onApply() {
    SimulationScenario scenario = getScenario();
    
    if (m_simManager) {
        m_simManager->setScenario(scenario);
        m_simManager->setTimeScale(m_timeScale->value());
    }
    
    emit scenarioChanged(scenario);
}

void SimulationSettingsDialog::onLoadCustom() {
    QString path = QFileDialog::getOpenFileName(this, "Load Scenario",
                                                QString(), "JSON Files (*.json)");
    if (!path.isEmpty() && m_simManager) {
        m_simManager->loadScenario(path);
        loadScenarioToUI(m_simManager->currentScenario());
        m_scenarioCombo->setCurrentIndex(4);  // Custom
    }
}

void SimulationSettingsDialog::onSaveCustom() {
    QString path = QFileDialog::getSaveFileName(this, "Save Scenario",
                                                "scenario.json", "JSON Files (*.json)");
    if (!path.isEmpty() && m_simManager) {
        m_simManager->setScenario(getScenario());
        m_simManager->saveScenario(path);
        QMessageBox::information(this, "Saved", "Scenario saved successfully.");
    }
}

void SimulationSettingsDialog::updatePreview() {
    // Could show a preview of the scenario parameters
}

} // namespace CounterUAS
