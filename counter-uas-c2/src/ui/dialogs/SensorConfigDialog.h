/**
 * @file SensorConfigDialog.h
 * @brief Dialog for configuring sensors
 */

#ifndef SENSORCONFIGDIALOG_H
#define SENSORCONFIGDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "sensors/SensorInterface.h"

namespace CounterUAS {

class SensorSimulator;

/**
 * @brief Dialog for configuring and managing sensors
 */
class SensorConfigDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit SensorConfigDialog(SensorSimulator* simulator, QWidget* parent = nullptr);
    
signals:
    void sensorConfigChanged();
    void sensorAdded(const QString& sensorId);
    void sensorRemoved(const QString& sensorId);
    
private slots:
    void onAddSensor();
    void onRemoveSensor();
    void onSensorSelected(int row);
    void onApplyChanges();
    void refreshSensorList();
    
private:
    void setupUI();
    void setupRadarTab();
    void setupRFDetectorTab();
    void setupCameraTab();
    
    SensorSimulator* m_simulator;
    
    QTabWidget* m_tabWidget;
    QTableWidget* m_sensorTable;
    
    // Radar configuration
    QLineEdit* m_radarHost;
    QSpinBox* m_radarPort;
    QDoubleSpinBox* m_radarMinRange;
    QDoubleSpinBox* m_radarMaxRange;
    QSpinBox* m_radarUpdateRate;
    QCheckBox* m_radarFilterClutter;
    QDoubleSpinBox* m_radarClutterThreshold;
    
    // RF Detector configuration
    QComboBox* m_rfConnectionType;
    QLineEdit* m_rfHost;
    QSpinBox* m_rfPort;
    QDoubleSpinBox* m_rfMinFreq;
    QDoubleSpinBox* m_rfMaxFreq;
    QDoubleSpinBox* m_rfThreshold;
    QCheckBox* m_rfEnableDF;
    
    // Camera configuration
    QLineEdit* m_cameraStreamUrl;
    QCheckBox* m_cameraPTZ;
    QDoubleSpinBox* m_cameraPanMin;
    QDoubleSpinBox* m_cameraPanMax;
    QDoubleSpinBox* m_cameraTiltMin;
    QDoubleSpinBox* m_cameraTiltMax;
    QDoubleSpinBox* m_cameraZoomMax;
    
    QPushButton* m_applyBtn;
    QPushButton* m_addBtn;
    QPushButton* m_removeBtn;
    
    QString m_selectedSensorId;
};

} // namespace CounterUAS

#endif // SENSORCONFIGDIALOG_H
