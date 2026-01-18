/**
 * @file SensorConfigDialog.cpp
 * @brief Implementation of sensor configuration dialog
 */

#include "ui/dialogs/SensorConfigDialog.h"
#include "simulators/SensorSimulator.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>

namespace CounterUAS {

SensorConfigDialog::SensorConfigDialog(SensorSimulator* simulator, QWidget* parent)
    : QDialog(parent)
    , m_simulator(simulator)
{
    setWindowTitle("Sensor Configuration");
    setMinimumSize(700, 500);
    
    setupUI();
    refreshSensorList();
}

void SensorConfigDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Sensor list
    QGroupBox* listGroup = new QGroupBox("Configured Sensors", this);
    QVBoxLayout* listLayout = new QVBoxLayout(listGroup);
    
    m_sensorTable = new QTableWidget(this);
    m_sensorTable->setColumnCount(4);
    m_sensorTable->setHorizontalHeaderLabels({"ID", "Name", "Type", "Status"});
    m_sensorTable->horizontalHeader()->setStretchLastSection(true);
    m_sensorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_sensorTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_sensorTable, &QTableWidget::currentCellChanged,
            this, [this](int row, int, int, int) { onSensorSelected(row); });
    listLayout->addWidget(m_sensorTable);
    
    QHBoxLayout* listBtnLayout = new QHBoxLayout();
    m_addBtn = new QPushButton("Add Sensor", this);
    m_removeBtn = new QPushButton("Remove Sensor", this);
    m_removeBtn->setEnabled(false);
    connect(m_addBtn, &QPushButton::clicked, this, &SensorConfigDialog::onAddSensor);
    connect(m_removeBtn, &QPushButton::clicked, this, &SensorConfigDialog::onRemoveSensor);
    listBtnLayout->addWidget(m_addBtn);
    listBtnLayout->addWidget(m_removeBtn);
    listBtnLayout->addStretch();
    listLayout->addLayout(listBtnLayout);
    
    mainLayout->addWidget(listGroup);
    
    // Configuration tabs
    m_tabWidget = new QTabWidget(this);
    setupRadarTab();
    setupRFDetectorTab();
    setupCameraTab();
    mainLayout->addWidget(m_tabWidget);
    
    // Dialog buttons
    QHBoxLayout* btnLayout = new QHBoxLayout();
    m_applyBtn = new QPushButton("Apply", this);
    QPushButton* closeBtn = new QPushButton("Close", this);
    connect(m_applyBtn, &QPushButton::clicked, this, &SensorConfigDialog::onApplyChanges);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addStretch();
    btnLayout->addWidget(m_applyBtn);
    btnLayout->addWidget(closeBtn);
    mainLayout->addLayout(btnLayout);
}

void SensorConfigDialog::setupRadarTab() {
    QWidget* radarTab = new QWidget(this);
    QFormLayout* layout = new QFormLayout(radarTab);
    
    m_radarHost = new QLineEdit("127.0.0.1", this);
    layout->addRow("Host:", m_radarHost);
    
    m_radarPort = new QSpinBox(this);
    m_radarPort->setRange(1, 65535);
    m_radarPort->setValue(5001);
    layout->addRow("Port:", m_radarPort);
    
    m_radarMinRange = new QDoubleSpinBox(this);
    m_radarMinRange->setRange(0, 1000);
    m_radarMinRange->setValue(50);
    m_radarMinRange->setSuffix(" m");
    layout->addRow("Min Range:", m_radarMinRange);
    
    m_radarMaxRange = new QDoubleSpinBox(this);
    m_radarMaxRange->setRange(100, 20000);
    m_radarMaxRange->setValue(5000);
    m_radarMaxRange->setSuffix(" m");
    layout->addRow("Max Range:", m_radarMaxRange);
    
    m_radarUpdateRate = new QSpinBox(this);
    m_radarUpdateRate->setRange(1, 100);
    m_radarUpdateRate->setValue(10);
    m_radarUpdateRate->setSuffix(" Hz");
    layout->addRow("Update Rate:", m_radarUpdateRate);
    
    m_radarFilterClutter = new QCheckBox("Enable", this);
    m_radarFilterClutter->setChecked(true);
    layout->addRow("Filter Clutter:", m_radarFilterClutter);
    
    m_radarClutterThreshold = new QDoubleSpinBox(this);
    m_radarClutterThreshold->setRange(0, 1);
    m_radarClutterThreshold->setSingleStep(0.01);
    m_radarClutterThreshold->setValue(0.1);
    layout->addRow("Clutter Threshold:", m_radarClutterThreshold);
    
    m_tabWidget->addTab(radarTab, "Radar");
}

void SensorConfigDialog::setupRFDetectorTab() {
    QWidget* rfTab = new QWidget(this);
    QFormLayout* layout = new QFormLayout(rfTab);
    
    m_rfConnectionType = new QComboBox(this);
    m_rfConnectionType->addItem("UDP");
    m_rfConnectionType->addItem("Serial");
    layout->addRow("Connection:", m_rfConnectionType);
    
    m_rfHost = new QLineEdit("127.0.0.1", this);
    layout->addRow("Host:", m_rfHost);
    
    m_rfPort = new QSpinBox(this);
    m_rfPort->setRange(1, 65535);
    m_rfPort->setValue(5002);
    layout->addRow("Port:", m_rfPort);
    
    m_rfMinFreq = new QDoubleSpinBox(this);
    m_rfMinFreq->setRange(100, 10000);
    m_rfMinFreq->setValue(900);
    m_rfMinFreq->setSuffix(" MHz");
    layout->addRow("Min Frequency:", m_rfMinFreq);
    
    m_rfMaxFreq = new QDoubleSpinBox(this);
    m_rfMaxFreq->setRange(100, 10000);
    m_rfMaxFreq->setValue(6000);
    m_rfMaxFreq->setSuffix(" MHz");
    layout->addRow("Max Frequency:", m_rfMaxFreq);
    
    m_rfThreshold = new QDoubleSpinBox(this);
    m_rfThreshold->setRange(-120, 0);
    m_rfThreshold->setValue(-80);
    m_rfThreshold->setSuffix(" dBm");
    layout->addRow("Signal Threshold:", m_rfThreshold);
    
    m_rfEnableDF = new QCheckBox("Enable", this);
    m_rfEnableDF->setChecked(true);
    layout->addRow("Direction Finding:", m_rfEnableDF);
    
    m_tabWidget->addTab(rfTab, "RF Detector");
}

void SensorConfigDialog::setupCameraTab() {
    QWidget* cameraTab = new QWidget(this);
    QFormLayout* layout = new QFormLayout(cameraTab);
    
    m_cameraStreamUrl = new QLineEdit("rtsp://localhost:8554/stream", this);
    layout->addRow("Stream URL:", m_cameraStreamUrl);
    
    m_cameraPTZ = new QCheckBox("Enable PTZ Control", this);
    m_cameraPTZ->setChecked(true);
    layout->addRow("PTZ:", m_cameraPTZ);
    
    m_cameraPanMin = new QDoubleSpinBox(this);
    m_cameraPanMin->setRange(-180, 180);
    m_cameraPanMin->setValue(-180);
    m_cameraPanMin->setSuffix("°");
    layout->addRow("Pan Min:", m_cameraPanMin);
    
    m_cameraPanMax = new QDoubleSpinBox(this);
    m_cameraPanMax->setRange(-180, 180);
    m_cameraPanMax->setValue(180);
    m_cameraPanMax->setSuffix("°");
    layout->addRow("Pan Max:", m_cameraPanMax);
    
    m_cameraTiltMin = new QDoubleSpinBox(this);
    m_cameraTiltMin->setRange(-90, 90);
    m_cameraTiltMin->setValue(-45);
    m_cameraTiltMin->setSuffix("°");
    layout->addRow("Tilt Min:", m_cameraTiltMin);
    
    m_cameraTiltMax = new QDoubleSpinBox(this);
    m_cameraTiltMax->setRange(-90, 90);
    m_cameraTiltMax->setValue(90);
    m_cameraTiltMax->setSuffix("°");
    layout->addRow("Tilt Max:", m_cameraTiltMax);
    
    m_cameraZoomMax = new QDoubleSpinBox(this);
    m_cameraZoomMax->setRange(1, 100);
    m_cameraZoomMax->setValue(30);
    m_cameraZoomMax->setSuffix("x");
    layout->addRow("Max Zoom:", m_cameraZoomMax);
    
    m_tabWidget->addTab(cameraTab, "Camera");
}

void SensorConfigDialog::refreshSensorList() {
    m_sensorTable->setRowCount(0);
    
    // Add simulated sensors to list
    QStringList sensors = {"SIM-RADAR-001", "SIM-RADAR-002", "SIM-RF-001", 
                          "SIM-CAM-001", "SIM-CAM-002"};
    QStringList names = {"Primary Radar", "Secondary Radar", "RF Detector",
                        "Main EO Tracker", "Thermal Camera"};
    QStringList types = {"RADAR", "RADAR", "RF_DETECTOR", "CAMERA", "CAMERA"};
    
    for (int i = 0; i < sensors.size(); i++) {
        int row = m_sensorTable->rowCount();
        m_sensorTable->insertRow(row);
        m_sensorTable->setItem(row, 0, new QTableWidgetItem(sensors[i]));
        m_sensorTable->setItem(row, 1, new QTableWidgetItem(names[i]));
        m_sensorTable->setItem(row, 2, new QTableWidgetItem(types[i]));
        m_sensorTable->setItem(row, 3, new QTableWidgetItem("ONLINE"));
    }
}

void SensorConfigDialog::onSensorSelected(int row) {
    if (row < 0) {
        m_removeBtn->setEnabled(false);
        m_selectedSensorId.clear();
        return;
    }
    
    m_removeBtn->setEnabled(true);
    m_selectedSensorId = m_sensorTable->item(row, 0)->text();
    
    // Switch to appropriate tab based on sensor type
    QString type = m_sensorTable->item(row, 2)->text();
    if (type == "RADAR") {
        m_tabWidget->setCurrentIndex(0);
    } else if (type == "RF_DETECTOR") {
        m_tabWidget->setCurrentIndex(1);
    } else if (type == "CAMERA") {
        m_tabWidget->setCurrentIndex(2);
    }
}

void SensorConfigDialog::onAddSensor() {
    // In a full implementation, this would open a dialog to add a new sensor
    QMessageBox::information(this, "Add Sensor", 
        "Add sensor functionality - would open a dialog to create a new sensor configuration.");
}

void SensorConfigDialog::onRemoveSensor() {
    if (m_selectedSensorId.isEmpty()) return;
    
    int ret = QMessageBox::question(this, "Remove Sensor",
        QString("Are you sure you want to remove sensor %1?").arg(m_selectedSensorId));
    
    if (ret == QMessageBox::Yes) {
        if (m_simulator) {
            m_simulator->unregisterSensor(m_selectedSensorId);
        }
        emit sensorRemoved(m_selectedSensorId);
        refreshSensorList();
    }
}

void SensorConfigDialog::onApplyChanges() {
    emit sensorConfigChanged();
    QMessageBox::information(this, "Configuration", "Sensor configuration applied.");
}

} // namespace CounterUAS
