/**
 * @file EffectorSimulator.h
 * @brief Complete effector simulation for Counter-UAS C2 system
 * 
 * Provides realistic simulation of:
 * - RF Jammers with frequency hopping and power control
 * - Kinetic Interceptors with launch sequences and flight dynamics
 * - Directed Energy Systems with tracking and dwell time simulation
 */

#ifndef EFFECTORSIMULATOR_H
#define EFFECTORSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QHash>
#include <QList>
#include <QRandomGenerator>
#include "core/Track.h"
#include "effectors/EffectorInterface.h"

namespace CounterUAS {

class EngagementManager;
class RFJammer;
class KineticInterceptor;
class DirectedEnergySystem;

/**
 * @brief Simulated effector engagement result
 */
struct SimulatedEngagementResult {
    QString effectorId;
    QString effectorType;
    QString trackId;
    GeoPosition targetPosition;
    bool success;
    double effectivenessAchieved;
    QString resultDetails;
    qint64 timestamp;
};

/**
 * @brief RF Jammer simulation state
 */
struct RFJammerSimState {
    bool active = false;
    double currentPowerW = 0.0;
    QList<double> activeFrequencies;
    qint64 engagementStartTime = 0;
    int targetInterferenceLevel = 0;  // 0-100
    double signalEffectiveness = 0.0;
    bool targetLinkDisrupted = false;
};

/**
 * @brief Kinetic Interceptor simulation state
 */
struct KineticSimState {
    enum class Phase { Idle, Arming, Launching, InFlight, Terminal, Complete };
    
    Phase phase = Phase::Idle;
    GeoPosition launcherPosition;
    GeoPosition currentPosition;
    GeoPosition targetPosition;
    VelocityVector velocity;
    double speed = 0.0;
    double distanceToTarget = 0.0;
    qint64 launchTime = 0;
    int roundsRemaining = 10;
    bool interceptSuccess = false;
};

/**
 * @brief Directed Energy simulation state
 */
struct DESimState {
    bool active = false;
    bool tracking = false;
    double currentPowerKW = 0.0;
    double dwellTimeS = 0.0;
    double requiredDwellS = 2.0;
    GeoPosition targetPosition;
    double trackingError = 0.0;
    bool effectAchieved = false;
    qint64 trackingStartTime = 0;
};

/**
 * @brief Comprehensive effector simulator for all effector types
 */
class EffectorSimulator : public QObject {
    Q_OBJECT
    
public:
    explicit EffectorSimulator(EngagementManager* manager, QObject* parent = nullptr);
    ~EffectorSimulator() override;
    
    void setEngagementManager(EngagementManager* manager);
    EngagementManager* engagementManager() const { return m_engagementManager; }
    
    // Simulation control
    void start();
    void stop();
    bool isRunning() const { return m_running; }
    
    void setUpdateRate(int hz);
    int updateRate() const { return m_updateRateHz; }
    
    // Simulation parameters
    void setRealisticMode(bool enable) { m_realisticMode = enable; }
    bool realisticMode() const { return m_realisticMode; }
    
    void setFailureRate(double rate) { m_failureRate = qBound(0.0, rate, 1.0); }
    double failureRate() const { return m_failureRate; }
    
    void setWeatherFactor(double factor) { m_weatherFactor = qBound(0.0, factor, 1.0); }
    double weatherFactor() const { return m_weatherFactor; }
    
    // Effector registration (auto-creates simulation state)
    void registerRFJammer(RFJammer* jammer);
    void registerKineticInterceptor(KineticInterceptor* interceptor);
    void registerDirectedEnergy(DirectedEnergySystem* de);
    
    void unregisterEffector(const QString& effectorId);
    void clearEffectors();
    
    // State access
    RFJammerSimState rfJammerState(const QString& id) const;
    KineticSimState kineticState(const QString& id) const;
    DESimState deState(const QString& id) const;
    
    // Manual engagement control (for testing)
    void simulateRFJamEngagement(const QString& jammerId, const GeoPosition& target);
    void simulateKineticLaunch(const QString& interceptorId, const GeoPosition& target);
    void simulateDEEngagement(const QString& deId, const GeoPosition& target);
    
    // Statistics
    struct Statistics {
        int totalSimulatedEngagements = 0;
        int rfJammerEngagements = 0;
        int kineticEngagements = 0;
        int deEngagements = 0;
        int successfulEngagements = 0;
        double avgEngagementTimeMs = 0.0;
    };
    Statistics statistics() const { return m_stats; }
    
signals:
    void rfJammerStateChanged(const QString& id, const RFJammerSimState& state);
    void kineticStateChanged(const QString& id, const KineticSimState& state);
    void deStateChanged(const QString& id, const DESimState& state);
    
    void engagementSimulated(const SimulatedEngagementResult& result);
    void interceptorInFlight(const QString& id, const GeoPosition& position, double distanceToTarget);
    void targetEffectSimulated(const QString& effectorId, const QString& trackId, double effectLevel);
    
    void simulationStarted();
    void simulationStopped();
    
private slots:
    void updateSimulation();
    void onRFJammerEngaged(bool engaged);
    void onKineticLaunched(int remaining);
    void onDEPowerChanged(double kw);
    
private:
    void updateRFJammerSimulation();
    void updateKineticSimulation();
    void updateDESimulation();
    
    double calculateRFJamEffectiveness(RFJammer* jammer, const GeoPosition& target);
    double calculateInterceptProbability(KineticInterceptor* interceptor, 
                                         const GeoPosition& target,
                                         double distanceToTarget);
    double calculateDEEffect(DirectedEnergySystem* de, 
                            const GeoPosition& target,
                            double dwellTime);
    
    void simulateInterceptorFlight(const QString& id, double dt);
    void checkInterceptCompletion(const QString& id);
    
    EngagementManager* m_engagementManager = nullptr;
    
    QTimer* m_updateTimer;
    int m_updateRateHz = 20;
    bool m_running = false;
    
    // Simulation parameters
    bool m_realisticMode = true;
    double m_failureRate = 0.05;  // 5% random failure rate
    double m_weatherFactor = 1.0;  // 1.0 = perfect conditions
    
    // Registered effectors
    QHash<QString, RFJammer*> m_rfJammers;
    QHash<QString, KineticInterceptor*> m_kineticInterceptors;
    QHash<QString, DirectedEnergySystem*> m_deSystems;
    
    // Simulation states
    QHash<QString, RFJammerSimState> m_rfJammerStates;
    QHash<QString, KineticSimState> m_kineticStates;
    QHash<QString, DESimState> m_deStates;
    
    Statistics m_stats;
};

} // namespace CounterUAS

#endif // EFFECTORSIMULATOR_H
