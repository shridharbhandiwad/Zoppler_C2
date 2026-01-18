/**
 * @file SimulationSettingsDialog.h
 * @brief Dialog for configuring simulation parameters
 */

#ifndef SIMULATIONSETTINGSDIALOG_H
#define SIMULATIONSETTINGSDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QTabWidget>
#include "simulators/SystemSimulationManager.h"
#include "simulators/TrackSimulator.h"

namespace CounterUAS {

/**
 * @brief Dialog for configuring simulation settings
 */
class SimulationSettingsDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit SimulationSettingsDialog(SystemSimulationManager* simManager, QWidget* parent = nullptr);
    
    SimulationScenario getScenario() const;
    
signals:
    void scenarioChanged(const SimulationScenario& scenario);
    void targetInjected(const QString& targetId);
    
private slots:
    void onScenarioSelected(int index);
    void onApply();
    void onLoadCustom();
    void onSaveCustom();
    void updatePreview();
    
    // Manual target injection
    void onInjectTarget();
    void onClearTargets();
    void onAutoSpawnToggled(bool enabled);
    void onTargetInjected(const QString& targetId, const GeoPosition& position);
    void onTargetRemoved(const QString& targetId);
    
private:
    void setupUI();
    void setupScenarioTab(QWidget* tab);
    void setupTargetControlTab(QWidget* tab);
    void setupEnvironmentTab(QWidget* tab);
    void loadScenarioToUI(const SimulationScenario& scenario);
    void updateTargetList();
    
    SystemSimulationManager* m_simManager;
    
    // Main tab widget
    QTabWidget* m_tabWidget;
    
    // Scenario selection
    QComboBox* m_scenarioCombo;
    
    // General settings
    QSpinBox* m_durationMinutes;
    QSpinBox* m_maxTargets;
    QDoubleSpinBox* m_spawnRate;
    QDoubleSpinBox* m_timeScale;
    
    // Environment
    QSlider* m_weatherSlider;
    QLabel* m_weatherLabel;
    QSlider* m_clutterSlider;
    QLabel* m_clutterLabel;
    QSlider* m_noiseSlider;
    QLabel* m_noiseLabel;
    
    // Threat parameters
    QSpinBox* m_minThreatLevel;
    QSpinBox* m_maxThreatLevel;
    QDoubleSpinBox* m_hostileProbability;
    
    // Enable/disable simulators
    QCheckBox* m_enableRadar;
    QCheckBox* m_enableRF;
    QCheckBox* m_enableCamera;
    QCheckBox* m_enableVideo;
    QCheckBox* m_enableEffectors;
    
    // Base position
    QDoubleSpinBox* m_baseLat;
    QDoubleSpinBox* m_baseLon;
    QDoubleSpinBox* m_baseAlt;
    
    // Target spawn control
    QCheckBox* m_autoSpawnEnabled;
    QSpinBox* m_spawnIntervalSec;
    
    // Manual target injection parameters
    QDoubleSpinBox* m_targetRange;       // Distance from base (meters)
    QDoubleSpinBox* m_targetBearing;     // Bearing from base (degrees)
    QDoubleSpinBox* m_targetAltitude;    // Altitude (meters)
    QDoubleSpinBox* m_targetSpeed;       // Speed (m/s)
    QDoubleSpinBox* m_targetHeading;     // Heading/direction of movement (degrees)
    QDoubleSpinBox* m_targetClimbRate;   // Vertical speed (m/s)
    QComboBox* m_targetClassification;   // Classification type
    
    // Target management
    QPushButton* m_injectTargetBtn;
    QPushButton* m_clearTargetsBtn;
    QListWidget* m_activeTargetsList;
    QLabel* m_targetCountLabel;
};

} // namespace CounterUAS

#endif // SIMULATIONSETTINGSDIALOG_H
