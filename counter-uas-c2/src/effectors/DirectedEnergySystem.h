#ifndef DIRECTEDENERGYSYSTEM_H
#define DIRECTEDENERGYSYSTEM_H

#include "effectors/EffectorInterface.h"

namespace CounterUAS {

/**
 * @brief Directed energy system configuration
 */
struct DESystemConfig {
    QString host = "127.0.0.1";
    int port = 5012;
    
    double maxPowerKW = 30.0;
    double minRangeM = 100.0;
    double maxRangeM = 1000.0;
    double beamDiameterMrad = 2.0;
    
    int maxEngagementTimeMs = 10000;
    int cooldownTimeMs = 15000;
    
    double dwellTimeRequiredS = 2.0;  // Time on target for effect
};

/**
 * @brief Directed energy weapon system
 */
class DirectedEnergySystem : public EffectorInterface {
    Q_OBJECT
    
public:
    explicit DirectedEnergySystem(const QString& effectorId, QObject* parent = nullptr);
    ~DirectedEnergySystem() override;
    
    QString effectorType() const override { return "DIRECTED_ENERGY"; }
    
    void setConfig(const DESystemConfig& config);
    DESystemConfig config() const { return m_config; }
    
    bool engage(const GeoPosition& target) override;
    void disengage() override;
    
    double minRange() const override { return m_config.minRangeM; }
    double maxRange() const override { return m_config.maxRangeM; }
    double effectiveness() const override { return 0.75; }
    
    // DE-specific
    void setPower(double kw);
    double currentPowerKW() const { return m_currentPowerKW; }
    double dwellTimeS() const;
    bool isTracking() const { return m_tracking; }
    
signals:
    void powerChanged(double kw);
    void trackingStatus(bool tracking, double dwellTime);
    void targetEffect();  // When target has been damaged
    
private slots:
    void onDwellComplete();
    void onCooldownComplete();
    void updateTracking();
    
private:
    void sendPointingCommand(const GeoPosition& target);
    void startTracking();
    void stopTracking();
    
    DESystemConfig m_config;
    double m_currentPowerKW = 0.0;
    bool m_tracking = false;
    qint64 m_dwellStartTime = 0;
    
    QTimer* m_dwellTimer;
    QTimer* m_cooldownTimer;
    QTimer* m_trackingTimer;
};

} // namespace CounterUAS

#endif // DIRECTEDENERGYSYSTEM_H
