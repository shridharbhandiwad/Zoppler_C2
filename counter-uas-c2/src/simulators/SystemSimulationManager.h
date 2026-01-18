/**
 * @file SystemSimulationManager.h
 * @brief Master simulation controller for Counter-UAS C2 system
 * 
 * Coordinates all simulation subsystems:
 * - Track simulation (target generation and movement)
 * - Sensor simulation (radar, RF, camera)
 * - Effector simulation (jammer, kinetic, DE)
 * - Video simulation (camera feeds)
 * - Scenario management
 */

#ifndef SYSTEMSIMULATIONMANAGER_H
#define SYSTEMSIMULATIONMANAGER_H

#include <QObject>
#include <QTimer>
#include <QString>
#include <QJsonObject>
#include "core/Track.h"

namespace CounterUAS {

class TrackManager;
class ThreatAssessor;
class EngagementManager;
class TrackSimulator;
class SensorSimulator;
class EffectorSimulator;
class VideoSimulator;
class VideoStreamManager;

// Forward declarations for sensor/effector types
class RadarSensor;
class RFDetector;
class CameraSystem;
class RFJammer;
class KineticInterceptor;
class DirectedEnergySystem;

/**
 * @brief Scenario configuration
 */
struct SimulationScenario {
    QString name;
    QString description;
    GeoPosition basePosition;
    int durationMinutes = 10;
    int maxTargets = 10;
    double threatSpawnRate = 0.2;  // Targets per second
    
    // Scenario parameters
    bool enableRadarSim = true;
    bool enableRFSim = true;
    bool enableCameraSim = true;
    bool enableVideoSim = true;
    bool enableEffectorSim = true;
    
    // Environmental conditions
    double weatherFactor = 1.0;     // 1.0 = perfect
    double clutterLevel = 0.1;      // 0-1
    double noiseLevel = 0.05;       // 0-1
    
    // Threat parameters
    int minThreatLevel = 1;
    int maxThreatLevel = 5;
    double hostileProbability = 0.7;
    
    QJsonObject toJson() const;
    static SimulationScenario fromJson(const QJsonObject& obj);
};

/**
 * @brief Simulation statistics
 */
struct SimulationStats {
    qint64 startTime = 0;
    qint64 elapsedMs = 0;
    int totalTargetsSpawned = 0;
    int totalTracksCreated = 0;
    int totalEngagements = 0;
    int successfulEngagements = 0;
    int sensorDetections = 0;
    double avgThreatLevel = 0.0;
};

/**
 * @brief Master simulation controller
 */
class SystemSimulationManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(bool paused READ isPaused NOTIFY pausedChanged)
    
public:
    explicit SystemSimulationManager(QObject* parent = nullptr);
    ~SystemSimulationManager() override;
    
    // Core system references
    void setTrackManager(TrackManager* manager);
    void setThreatAssessor(ThreatAssessor* assessor);
    void setEngagementManager(EngagementManager* manager);
    void setVideoManager(VideoStreamManager* manager);
    
    TrackManager* trackManager() const { return m_trackManager; }
    ThreatAssessor* threatAssessor() const { return m_threatAssessor; }
    EngagementManager* engagementManager() const { return m_engagementManager; }
    
    // Simulator access
    TrackSimulator* trackSimulator() const { return m_trackSimulator; }
    SensorSimulator* sensorSimulator() const { return m_sensorSimulator; }
    EffectorSimulator* effectorSimulator() const { return m_effectorSimulator; }
    VideoSimulator* videoSimulator() const { return m_videoSimulator; }
    
    // Simulation control
    void start();
    void stop();
    void pause();
    void resume();
    void reset();
    
    bool isRunning() const { return m_running; }
    bool isPaused() const { return m_paused; }
    
    // Time control
    void setTimeScale(double scale);
    double timeScale() const { return m_timeScale; }
    qint64 simulationTime() const;
    
    // Scenario management
    void loadScenario(const QString& scenarioPath);
    void saveScenario(const QString& scenarioPath);
    void setScenario(const SimulationScenario& scenario);
    SimulationScenario currentScenario() const { return m_scenario; }
    
    // Predefined scenarios
    void loadDefaultScenario();
    void loadSwarmAttackScenario();
    void loadMultiThreatScenario();
    void loadStressTestScenario();
    
    // Sensor registration
    void registerRadar(RadarSensor* radar);
    void registerRFDetector(RFDetector* detector);
    void registerCamera(CameraSystem* camera);
    
    // Effector registration
    void registerRFJammer(RFJammer* jammer);
    void registerKineticInterceptor(KineticInterceptor* interceptor);
    void registerDirectedEnergy(DirectedEnergySystem* de);
    
    // Create default simulated sensors/effectors
    void createDefaultSensors();
    void createDefaultEffectors();
    void createFullSimulationEnvironment();
    
    // Statistics
    SimulationStats statistics() const { return m_stats; }
    
    // Configuration
    void setBasePosition(const GeoPosition& pos);
    GeoPosition basePosition() const { return m_basePosition; }
    
    void setAutoSpawnTargets(bool enable) { m_autoSpawnTargets = enable; }
    bool autoSpawnTargets() const { return m_autoSpawnTargets; }
    
    void setMaxSimulatedTargets(int max) { m_maxTargets = max; }
    int maxSimulatedTargets() const { return m_maxTargets; }
    
signals:
    void runningChanged(bool running);
    void pausedChanged(bool paused);
    void scenarioLoaded(const QString& name);
    void simulationProgress(int percent);
    void statisticsUpdated(const SimulationStats& stats);
    
    void targetSpawned(const QString& trackId, const GeoPosition& position);
    void targetDestroyed(const QString& trackId);
    void engagementStarted(const QString& trackId, const QString& effectorId);
    void engagementCompleted(const QString& trackId, bool success);
    
    void sensorStatusChanged(const QString& sensorId, bool online);
    void effectorStatusChanged(const QString& effectorId, bool ready);
    
    void error(const QString& message);
    
private slots:
    void updateSimulation();
    void onTrackCreated(const QString& trackId);
    void onTrackDropped(const QString& trackId);
    void onEngagementComplete(const QString& engagementId, bool success);
    
private:
    void initializeSimulators();
    void connectSignals();
    void updateStatistics();
    
    // Core systems
    TrackManager* m_trackManager = nullptr;
    ThreatAssessor* m_threatAssessor = nullptr;
    EngagementManager* m_engagementManager = nullptr;
    VideoStreamManager* m_videoManager = nullptr;
    
    // Simulators
    TrackSimulator* m_trackSimulator = nullptr;
    SensorSimulator* m_sensorSimulator = nullptr;
    EffectorSimulator* m_effectorSimulator = nullptr;
    VideoSimulator* m_videoSimulator = nullptr;
    
    // Owned sensor instances (for simulation)
    QList<RadarSensor*> m_simulatedRadars;
    QList<RFDetector*> m_simulatedRFDetectors;
    QList<CameraSystem*> m_simulatedCameras;
    
    // Owned effector instances (for simulation)
    QList<RFJammer*> m_simulatedJammers;
    QList<KineticInterceptor*> m_simulatedInterceptors;
    QList<DirectedEnergySystem*> m_simulatedDESystems;
    
    // Simulation state
    bool m_running = false;
    bool m_paused = false;
    double m_timeScale = 1.0;
    qint64 m_startTime = 0;
    qint64 m_pauseTime = 0;
    
    // Configuration
    GeoPosition m_basePosition;
    bool m_autoSpawnTargets = true;
    int m_maxTargets = 10;
    
    SimulationScenario m_scenario;
    SimulationStats m_stats;
    
    QTimer* m_updateTimer;
    QTimer* m_statsTimer;
};

} // namespace CounterUAS

#endif // SYSTEMSIMULATIONMANAGER_H
