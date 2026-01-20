#ifndef SENSORNETWORKPAGE_H
#define SENSORNETWORKPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QMap>

namespace CounterUAS {

/**
 * SensorCard - Widget displaying a single sensor's status
 */
class SensorCard : public QFrame {
    Q_OBJECT
    
public:
    explicit SensorCard(QWidget* parent = nullptr);
    
    void setSensorId(const QString& id);
    void setSensorName(const QString& name);
    void setSensorType(const QString& type);
    void setCoverage(double coverageKm);
    void setStatus(const QString& status);  // ONLINE, OFFLINE, DEGRADED
    void setCoordinates(double lat, double lng);
    void setAzimuth(double degrees);
    
signals:
    void clicked(const QString& sensorId);
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    
private:
    void setupUI();
    void updateStyle();
    
    QString m_sensorId;
    QString m_status;
    
    QLabel* m_iconLabel;
    QLabel* m_nameLabel;
    QLabel* m_typeLabel;
    QLabel* m_coverageLabel;
    QLabel* m_coverageValue;
    QLabel* m_statusLabel;
    QLabel* m_statusValue;
    QLabel* m_coordsLabel;
    QLabel* m_coordsValue;
    QLabel* m_azimuthLabel;
    QLabel* m_azimuthValue;
    QLabel* m_signalIcon;
};

/**
 * SensorNetworkPage - Sensor Network management view
 * 
 * Shows sensor cards in a grid layout with:
 * - Sensor name and type
 * - Coverage radius
 * - Status
 * - Coordinates
 * - Azimuth
 */
class SensorNetworkPage : public QWidget {
    Q_OBJECT
    
public:
    explicit SensorNetworkPage(QWidget* parent = nullptr);
    ~SensorNetworkPage() override = default;
    
    void addSensor(const QString& id, const QString& name, const QString& type);
    void updateSensorStatus(const QString& id, const QString& status);
    void updateSensorCoverage(const QString& id, double coverageKm);
    void updateSensorCoordinates(const QString& id, double lat, double lng);
    void updateSensorAzimuth(const QString& id, double degrees);
    
signals:
    void sensorSelected(const QString& sensorId);
    void addSensorClicked();
    
private slots:
    void onSensorCardClicked(const QString& sensorId);
    
private:
    void setupUI();
    void setupDefaultSensors();
    
    QHBoxLayout* m_cardsLayout;
    QMap<QString, SensorCard*> m_sensorCards;
    QPushButton* m_addButton;
};

} // namespace CounterUAS

#endif // SENSORNETWORKPAGE_H
