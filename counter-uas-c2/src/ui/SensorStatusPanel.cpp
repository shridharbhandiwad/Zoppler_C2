#include "ui/SensorStatusPanel.h"
#include "simulators/SensorSimulator.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>

namespace CounterUAS {

SensorStatusPanel::SensorStatusPanel(QWidget* parent)
    : QWidget(parent)
    , m_refreshTimer(new QTimer(this))
{
    setupUI();
    
    connect(m_refreshTimer, &QTimer::timeout, this, &SensorStatusPanel::refreshStatus);
}

void SensorStatusPanel::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    QLabel* titleLabel = new QLabel("Sensor Status", this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 12px;");
    layout->addWidget(titleLabel);
    
    m_table = new QTableWidget(this);
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({"Sensor ID", "Type", "Status", "Signal", "Detections"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setAlternatingRowColors(true);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    connect(m_table, &QTableWidget::cellClicked, this, &SensorStatusPanel::onTableItemClicked);
    connect(m_table, &QTableWidget::cellDoubleClicked, this, &SensorStatusPanel::onTableItemDoubleClicked);
    
    layout->addWidget(m_table);
}

void SensorStatusPanel::setSensorSimulator(SensorSimulator* simulator) {
    m_simulator = simulator;
    
    if (m_simulator) {
        m_refreshTimer->start(500);  // Refresh every 500ms
        
        // Connect to simulator signals
        connect(m_simulator, &SensorSimulator::radarStateChanged, this, [this](const QString& id, const RadarSimState& state) {
            int row = m_sensorRows.value(id, -1);
            if (row >= 0 && row < m_table->rowCount()) {
                m_table->item(row, 2)->setText(state.active ? "Active" : "Inactive");
                m_table->item(row, 2)->setForeground(state.active ? QColor(0, 200, 0) : QColor(128, 128, 128));
                m_table->item(row, 3)->setText(QString("%1%").arg(static_cast<int>(state.signalQuality * 100)));
                m_table->item(row, 3)->setForeground(healthToColor(state.signalQuality));
                m_table->item(row, 4)->setText(QString::number(state.detectedTargets));
            }
        });
        
        connect(m_simulator, &SensorSimulator::rfDetectorStateChanged, this, [this](const QString& id, const RFDetectorSimState& state) {
            int row = m_sensorRows.value(id, -1);
            if (row >= 0 && row < m_table->rowCount()) {
                m_table->item(row, 2)->setText(state.active ? "Active" : "Inactive");
                m_table->item(row, 2)->setForeground(state.active ? QColor(0, 200, 0) : QColor(128, 128, 128));
                m_table->item(row, 3)->setText(QString("%1 dBm").arg(state.noiseFloor, 0, 'f', 0));
                m_table->item(row, 4)->setText(QString::number(state.detectedEmissions));
            }
        });
        
        connect(m_simulator, &SensorSimulator::cameraStateChanged, this, [this](const QString& id, const CameraSimState& state) {
            int row = m_sensorRows.value(id, -1);
            if (row >= 0 && row < m_table->rowCount()) {
                m_table->item(row, 2)->setText(state.active ? "Active" : "Inactive");
                m_table->item(row, 2)->setForeground(state.active ? QColor(0, 200, 0) : QColor(128, 128, 128));
                m_table->item(row, 3)->setText(QString("Z:%1x").arg(state.zoom, 0, 'f', 1));
                m_table->item(row, 4)->setText(QString::number(state.detectedObjects));
            }
        });
    } else {
        m_refreshTimer->stop();
    }
}

void SensorStatusPanel::addSensor(const QString& id, const QString& name, const QString& type) {
    if (m_sensorRows.contains(id)) {
        return;  // Sensor already exists
    }
    
    int row = m_table->rowCount();
    m_table->insertRow(row);
    
    QTableWidgetItem* idItem = new QTableWidgetItem(name.isEmpty() ? id : name);
    idItem->setData(Qt::UserRole, id);  // Store the ID
    m_table->setItem(row, 0, idItem);
    
    m_table->setItem(row, 1, new QTableWidgetItem(type));
    
    QTableWidgetItem* statusItem = new QTableWidgetItem("Inactive");
    statusItem->setForeground(QColor(128, 128, 128));
    m_table->setItem(row, 2, statusItem);
    
    QTableWidgetItem* signalItem = new QTableWidgetItem("-");
    m_table->setItem(row, 3, signalItem);
    
    QTableWidgetItem* detectionsItem = new QTableWidgetItem("0");
    m_table->setItem(row, 4, detectionsItem);
    
    m_sensorRows[id] = row;
}

void SensorStatusPanel::removeSensor(const QString& id) {
    int row = m_sensorRows.value(id, -1);
    if (row >= 0 && row < m_table->rowCount()) {
        m_table->removeRow(row);
        m_sensorRows.remove(id);
        
        // Update row indices
        for (auto it = m_sensorRows.begin(); it != m_sensorRows.end(); ++it) {
            if (it.value() > row) {
                it.value()--;
            }
        }
    }
}

void SensorStatusPanel::updateSensorStatus(const QString& id, const QString& status) {
    int row = m_sensorRows.value(id, -1);
    if (row >= 0 && row < m_table->rowCount()) {
        m_table->item(row, 2)->setText(status);
        m_table->item(row, 2)->setForeground(statusToColor(status));
    }
}

void SensorStatusPanel::updateSensorHealth(const QString& id, const SensorHealth& health) {
    int row = m_sensorRows.value(id, -1);
    if (row >= 0 && row < m_table->rowCount()) {
        m_table->item(row, 3)->setText(QString("%1%").arg(static_cast<int>(health.signalQuality * 100)));
        m_table->item(row, 3)->setForeground(healthToColor(health.signalQuality));
    }
}

void SensorStatusPanel::clearSensors() {
    m_table->setRowCount(0);
    m_sensorRows.clear();
}

void SensorStatusPanel::refreshStatus() {
    // This is called periodically to update any manual updates
    // The main updates come from simulator signals
}

void SensorStatusPanel::onTableItemClicked(int row, int column) {
    Q_UNUSED(column)
    
    if (row >= 0 && row < m_table->rowCount()) {
        QTableWidgetItem* item = m_table->item(row, 0);
        if (item) {
            QString sensorId = item->data(Qt::UserRole).toString();
            emit sensorSelected(sensorId);
        }
    }
}

void SensorStatusPanel::onTableItemDoubleClicked(int row, int column) {
    Q_UNUSED(column)
    
    if (row >= 0 && row < m_table->rowCount()) {
        QTableWidgetItem* item = m_table->item(row, 0);
        if (item) {
            QString sensorId = item->data(Qt::UserRole).toString();
            emit sensorDoubleClicked(sensorId);
        }
    }
}

QColor SensorStatusPanel::statusToColor(const QString& status) {
    if (status == "Active" || status == "Online" || status == "Ready") {
        return QColor(0, 200, 0);
    } else if (status == "Inactive" || status == "Offline") {
        return QColor(128, 128, 128);
    } else if (status == "Error" || status == "Fault") {
        return QColor(255, 0, 0);
    } else if (status == "Warning" || status == "Degraded") {
        return QColor(255, 165, 0);
    }
    return QColor(255, 255, 255);
}

QColor SensorStatusPanel::healthToColor(double signalQuality) {
    if (signalQuality >= 0.8) {
        return QColor(0, 200, 0);
    } else if (signalQuality >= 0.5) {
        return QColor(255, 165, 0);
    } else if (signalQuality >= 0.2) {
        return QColor(255, 100, 0);
    }
    return QColor(255, 0, 0);
}

} // namespace CounterUAS
