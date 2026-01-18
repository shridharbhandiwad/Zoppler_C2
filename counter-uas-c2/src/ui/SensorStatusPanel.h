#ifndef SENSORSTATUSPANEL_H
#define SENSORSTATUSPANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QTimer>
#include <QHash>
#include "sensors/SensorInterface.h"

namespace CounterUAS {

class SensorSimulator;

/**
 * @brief Panel showing real-time sensor status
 */
class SensorStatusPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit SensorStatusPanel(QWidget* parent = nullptr);
    
    void setSensorSimulator(SensorSimulator* simulator);
    
    void addSensor(const QString& id, const QString& name, const QString& type);
    void removeSensor(const QString& id);
    void updateSensorStatus(const QString& id, const QString& status);
    void updateSensorHealth(const QString& id, const SensorHealth& health);
    
    void clearSensors();
    
signals:
    void sensorSelected(const QString& sensorId);
    void sensorDoubleClicked(const QString& sensorId);
    
private slots:
    void refreshStatus();
    void onTableItemClicked(int row, int column);
    void onTableItemDoubleClicked(int row, int column);
    
private:
    void setupUI();
    QColor statusToColor(const QString& status);
    QColor healthToColor(double signalQuality);
    
    QTableWidget* m_table;
    SensorSimulator* m_simulator = nullptr;
    QTimer* m_refreshTimer;
    
    QHash<QString, int> m_sensorRows;  // sensorId -> row index
};

} // namespace CounterUAS

#endif // SENSORSTATUSPANEL_H
