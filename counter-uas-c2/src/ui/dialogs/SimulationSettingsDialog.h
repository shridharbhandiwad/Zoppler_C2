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
#include "simulators/SystemSimulationManager.h"

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
    
private slots:
    void onScenarioSelected(int index);
    void onApply();
    void onLoadCustom();
    void onSaveCustom();
    void updatePreview();
    
private:
    void setupUI();
    void loadScenarioToUI(const SimulationScenario& scenario);
    
    SystemSimulationManager* m_simManager;
    
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
};

} // namespace CounterUAS

#endif // SIMULATIONSETTINGSDIALOG_H
