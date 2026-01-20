#ifndef HEADERBAR_H
#define HEADERBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QHBoxLayout>

namespace CounterUAS {

/**
 * HeaderBar - Top status bar for ZOPPLER interface
 * 
 * Displays:
 * - System Status (OPERATIONAL/DEGRADED/OFFLINE)
 * - Active Sector
 * - Threat Level
 * - Simulate Threat button
 * - Zulu Time
 */
class HeaderBar : public QWidget {
    Q_OBJECT
    
public:
    explicit HeaderBar(QWidget* parent = nullptr);
    ~HeaderBar() override = default;
    
    void setSystemStatus(const QString& status);
    void setActiveSector(const QString& sector);
    void setThreatLevel(const QString& level);
    void setThreatCount(int active, int total);
    void setSensorCount(int active, int total);
    
signals:
    void simulateThreatClicked();
    
private slots:
    void updateTime();
    
private:
    void setupUI();
    QWidget* createStatWidget(const QString& label, const QString& objectName);
    
    QLabel* m_systemStatusLabel;
    QLabel* m_systemStatusValue;
    QLabel* m_activeSectorLabel;
    QLabel* m_activeSectorValue;
    QLabel* m_threatLevelLabel;
    QLabel* m_threatLevelValue;
    QLabel* m_activeThreatsValue;
    QLabel* m_sensorNetValue;
    QLabel* m_latLabel;
    QLabel* m_lngLabel;
    QLabel* m_zoomLabel;
    
    QPushButton* m_simulateButton;
    QLabel* m_zuluTimeLabel;
    QLabel* m_zuluTimeSuffix;
    
    QTimer* m_timeTimer;
};

} // namespace CounterUAS

#endif // HEADERBAR_H
