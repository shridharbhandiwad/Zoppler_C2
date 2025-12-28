#ifndef KINETICINTERCEPTOR_H
#define KINETICINTERCEPTOR_H

#include "effectors/EffectorInterface.h"
#include <QStateMachine>

namespace CounterUAS {

/**
 * @brief Kinetic interceptor configuration
 */
struct KineticInterceptorConfig {
    QString host = "127.0.0.1";
    int port = 5011;
    
    int magazineCapacity = 10;
    double minRangeM = 100.0;
    double maxRangeM = 1500.0;
    double maxAltitudeM = 500.0;
    
    int launchTimeMs = 2000;   // Time from command to launch
    int flightTimeMs = 10000;  // Max flight time
    int reloadTimeMs = 30000;  // Time to reload one round
    
    double interceptProbability = 0.85;
};

/**
 * @brief Kinetic interceptor effector
 */
class KineticInterceptor : public EffectorInterface {
    Q_OBJECT
    
public:
    explicit KineticInterceptor(const QString& effectorId, QObject* parent = nullptr);
    ~KineticInterceptor() override;
    
    QString effectorType() const override { return "KINETIC"; }
    
    // Configuration
    void setConfig(const KineticInterceptorConfig& config);
    KineticInterceptorConfig config() const { return m_config; }
    
    // EffectorInterface implementation
    bool engage(const GeoPosition& target) override;
    void disengage() override;
    bool isReady() const override;
    
    double minRange() const override { return m_config.minRangeM; }
    double maxRange() const override { return m_config.maxRangeM; }
    double effectiveness() const override { return m_config.interceptProbability; }
    
    // Interceptor-specific
    int remainingRounds() const { return m_remainingRounds; }
    int magazineCapacity() const { return m_config.magazineCapacity; }
    void reload();  // Manual reload (if supported)
    
    // Launch sequence status
    enum class LaunchPhase {
        Idle,
        Arming,
        Launching,
        InFlight,
        Terminal,
        Complete
    };
    LaunchPhase launchPhase() const { return m_launchPhase; }
    
signals:
    void launchPhaseChanged(LaunchPhase phase);
    void roundsFired(int remaining);
    void reloadStarted();
    void reloadComplete();
    void interceptResult(bool success);
    
private slots:
    void onArmingComplete();
    void onLaunchComplete();
    void onTerminalPhase();
    void onInterceptComplete();
    void onReloadComplete();
    
private:
    void transitionPhase(LaunchPhase phase);
    void sendLaunchCommand(const GeoPosition& target);
    void simulateIntercept();
    
    KineticInterceptorConfig m_config;
    int m_remainingRounds;
    LaunchPhase m_launchPhase = LaunchPhase::Idle;
    
    QTimer* m_armingTimer;
    QTimer* m_launchTimer;
    QTimer* m_flightTimer;
    QTimer* m_reloadTimer;
};

} // namespace CounterUAS

#endif // KINETICINTERCEPTOR_H
