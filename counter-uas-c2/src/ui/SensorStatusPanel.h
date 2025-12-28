#ifndef SENSORSTATUSPANEL_H
#define SENSORSTATUSPANEL_H

#include <QWidget>
#include <QTableWidget>

namespace CounterUAS {

class SensorStatusPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit SensorStatusPanel(QWidget* parent = nullptr);
    void addSensor(const QString& id, const QString& name, const QString& type);
    void updateSensorStatus(const QString& id, const QString& status);
    
private:
    QTableWidget* m_table;
};

} // namespace CounterUAS

#endif // SENSORSTATUSPANEL_H
