/**
 * @file SystemSimulationManager.cpp
 * @brief Implementation of master simulation controller
 */

#include "simulators/SystemSimulationManager.h"
#include "simulators/TrackSimulator.h"
#include "simulators/SensorSimulator.h"
#include "simulators/EffectorSimulator.h"
#include "simulators/VideoSimulator.h"
#include "core/TrackManager.h"
#include "core/ThreatAssessor.h"
#include "core/EngagementManager.h"
#include "video/VideoStreamManager.h"
#include "sensors/RadarSensor.h"
#include "sensors/RFDetector.h"
#include "sensors/CameraSystem.h"
#include "effectors/RFJammer.h"
#include "effectors/KineticInterceptor.h"
#include "effectors/DirectedEnergySystem.h"
#include "utils/Logger.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>

namespace CounterUAS {

QJsonObject SimulationScenario::toJson() const {
    QJsonObject obj;
    obj["name"] = name;
    obj["description"] = description;
    obj["basePosition"] = basePosition.toJson();
    obj["durationMinutes"] = durationMinutes;
    obj["maxTargets"] = maxTargets;
    obj["threatSpawnRate"] = threatSpawnRate;
    obj["enableRadarSim"] = enableRadarSim;
    obj["enableRFSim"] = enableRFSim;
    obj["enableCameraSim"] = enableCameraSim;
    obj["enableVideoSim"] = enableVideoSim;
    obj["enableEffectorSim"] = enableEffectorSim;
    obj["weatherFactor"] = weatherFactor;
    obj["clutterLevel"] = clutterLevel;
    obj["noiseLevel"] = noiseLevel;
    obj["minThreatLevel"] = minThreatLevel;
    obj["maxThreatLevel"] = maxThreatLevel;
    obj["hostileProbability"] = hostileProbability;
    return obj;
}

SimulationScenario SimulationScenario::fromJson(const QJsonObject& obj) {
    SimulationScenario s;
    s.name = obj["name"].toString("Default");
    s.description = obj["description"].toString();
    s.basePosition = GeoPosition::fromJson(obj["basePosition"].toObject());
    s.durationMinutes = obj["durationMinutes"].toInt(10);
    s.maxTargets = obj["maxTargets"].toInt(10);
    s.threatSpawnRate = obj["threatSpawnRate"].toDouble(0.2);
    s.enableRadarSim = obj["enableRadarSim"].toBool(true);
    s.enableRFSim = obj["enableRFSim"].toBool(true);
    s.enableCameraSim = obj["enableCameraSim"].toBool(true);
    s.enableVideoSim = obj["enableVideoSim"].toBool(true);
    s.enableEffectorSim = obj["enableEffectorSim"].toBool(true);
    s.weatherFactor = obj["weatherFactor"].toDouble(1.0);
    s.clutterLevel = obj["clutterLevel"].toDouble(0.1);
    s.noiseLevel = obj["noiseLevel"].toDouble(0.05);
    s.minThreatLevel = obj["minThreatLevel"].toInt(1);
    s.maxThreatLevel = obj["maxThreatLevel"].toInt(5);
    s.hostileProbability = obj["hostileProbability"].toDouble(0.7);
    return s;
}

SystemSimulationManager::SystemSimulationManager(QObject* parent)
    : QObject(parent)
    , m_updateTimer(new QTimer(this))
    , m_statsTimer(new QTimer(this))
{
    // Default base position
    m_basePosition.latitude = 34.0522;
    m_basePosition.longitude = -118.2437;
    m_basePosition.altitude = 100.0;
    
    m_updateTimer->setInterval(100);  // 10 Hz update
    connect(m_updateTimer, &QTimer::timeout, this, &SystemSimulationManager::updateSimulation);
    
    m_statsTimer->setInterval(1000);  // Update stats every second
    connect(m_statsTimer, &QTimer::timeout, this, &SystemSimulationManager::updateStatistics);
    
    // Load default scenario
    loadDefaultScenario();
}

SystemSimulationManager::~SystemSimulationManager() {
    stop();
    
    // Clean up owned objects
    qDeleteAll(m_simulatedRadars);
    qDeleteAll(m_simulatedRFDetectors);
    qDeleteAll(m_simulatedCameras);
    qDeleteAll(m_simulatedJammers);
    qDeleteAll(m_simulatedInterceptors);
    qDeleteAll(m_simulatedDESystems);
}

void SystemSimulationManager::setTrackManager(TrackManager* manager) {
    m_trackManager = manager;
    initializeSimulators();
}

void SystemSimulationManager::setThreatAssessor(ThreatAssessor* assessor) {
    m_threatAssessor = assessor;
}

void SystemSimulationManager::setEngagementManager(EngagementManager* manager) {
    m_engagementManager = manager;
    if (m_effectorSimulator) {
        m_effectorSimulator->setEngagementManager(manager);
    }
}

void SystemSimulationManager::setVideoManager(VideoStreamManager* manager) {
    m_videoManager = manager;
    if (m_videoSimulator) {
        m_videoSimulator->setVideoManager(manager);
    }
}

void SystemSimulationManager::initializeSimulators() {
    if (!m_trackManager) return;
    
    // Create track simulator
    if (!m_trackSimulator) {
        m_trackSimulator = new TrackSimulator(m_trackManager, this);
        m_trackSimulator->setBasePosition(m_basePosition);
    }
    
    // Create sensor simulator
    if (!m_sensorSimulator) {
        m_sensorSimulator = new SensorSimulator(m_trackManager, this);
        m_sensorSimulator->setBasePosition(m_basePosition);
    }
    
    // Create effector simulator
    if (!m_effectorSimulator) {
        m_effectorSimulator = new EffectorSimulator(m_engagementManager, this);
    }
    
    // Create video simulator
    if (!m_videoSimulator) {
        m_videoSimulator = new VideoSimulator(this);
        m_videoSimulator->setVideoManager(m_videoManager);
    }
    
    connectSignals();
    
    Logger::instance().info("SystemSimulationManager", "Simulators initialized");
}

void SystemSimulationManager::connectSignals() {
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackCreated,
                this, &SystemSimulationManager::onTrackCreated);
        connect(m_trackManager, &TrackManager::trackDropped,
                this, &SystemSimulationManager::onTrackDropped);
    }
}

void SystemSimulationManager::start() {
    if (m_running) return;
    
    m_running = true;
    m_paused = false;
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    m_stats = SimulationStats();
    m_stats.startTime = m_startTime;
    
    // Apply scenario settings
    if (m_sensorSimulator) {
        m_sensorSimulator->setClutterLevel(m_scenario.clutterLevel);
        m_sensorSimulator->setNoiseLevel(m_scenario.noiseLevel);
    }
    
    if (m_effectorSimulator) {
        m_effectorSimulator->setWeatherFactor(m_scenario.weatherFactor);
    }
    
    // Configure track simulator with scenario settings
    if (m_trackSimulator) {
        m_trackSimulator->setMaxTargets(m_scenario.maxTargets);
        m_trackSimulator->setAutoSpawnEnabled(m_autoSpawnTargets);
        // Convert spawn rate to interval (targets/sec -> msec between spawns)
        if (m_scenario.threatSpawnRate > 0) {
            int intervalMs = static_cast<int>(1000.0 / m_scenario.threatSpawnRate);
            m_trackSimulator->setSpawnInterval(intervalMs);
        }
        m_trackSimulator->start();
    }
    
    if (m_sensorSimulator && m_scenario.enableRadarSim) m_sensorSimulator->start();
    if (m_effectorSimulator && m_scenario.enableEffectorSim) m_effectorSimulator->start();
    if (m_videoSimulator && m_scenario.enableVideoSim) m_videoSimulator->start();
    
    m_updateTimer->start();
    m_statsTimer->start();
    
    Logger::instance().info("SystemSimulationManager", 
        QString("Simulation started: %1 (auto-spawn: %2)")
            .arg(m_scenario.name)
            .arg(m_autoSpawnTargets ? "enabled" : "disabled"));
    
    emit runningChanged(true);
}

void SystemSimulationManager::stop() {
    if (!m_running) return;
    
    m_running = false;
    m_paused = false;
    
    m_updateTimer->stop();
    m_statsTimer->stop();
    
    // Stop all simulators
    if (m_trackSimulator) m_trackSimulator->stop();
    if (m_sensorSimulator) m_sensorSimulator->stop();
    if (m_effectorSimulator) m_effectorSimulator->stop();
    if (m_videoSimulator) m_videoSimulator->stop();
    
    Logger::instance().info("SystemSimulationManager", "Simulation stopped");
    
    emit runningChanged(false);
}

void SystemSimulationManager::pause() {
    if (!m_running || m_paused) return;
    
    m_paused = true;
    m_pauseTime = QDateTime::currentMSecsSinceEpoch();
    
    m_updateTimer->stop();
    
    Logger::instance().info("SystemSimulationManager", "Simulation paused");
    emit pausedChanged(true);
}

void SystemSimulationManager::resume() {
    if (!m_running || !m_paused) return;
    
    m_paused = false;
    
    // Adjust start time for pause duration
    qint64 pauseDuration = QDateTime::currentMSecsSinceEpoch() - m_pauseTime;
    m_startTime += pauseDuration;
    
    m_updateTimer->start();
    
    Logger::instance().info("SystemSimulationManager", "Simulation resumed");
    emit pausedChanged(false);
}

void SystemSimulationManager::reset() {
    // First stop all simulation activity
    stop();
    
    // Clear track simulator targets BEFORE clearing track manager
    // This prevents the track simulator from updating deleted tracks
    if (m_trackSimulator) {
        m_trackSimulator->clearTargets();
    }
    
    // Clear sensor simulator injected targets
    if (m_sensorSimulator) {
        m_sensorSimulator->clearInjectedTargets();
    }
    
    // Reset video simulator
    if (m_videoSimulator) {
        m_videoSimulator->clearTrackedTargets();
    }
    
    // Now clear track manager - this emits trackDropped signals so connected
    // UI widgets can clean up their track references safely
    if (m_trackManager) {
        m_trackManager->clearAllTracks();
    }
    
    // Clear sensor registrations after track manager is cleared
    if (m_sensorSimulator) {
        m_sensorSimulator->clearSensors();
    }
    
    if (m_effectorSimulator) {
        m_effectorSimulator->clearEffectors();
    }
    
    // Clean up owned simulated sensors - these are parented to 'this' but
    // we need to clear the lists to prevent duplicates on next createFullSimulationEnvironment()
    qDeleteAll(m_simulatedRadars);
    m_simulatedRadars.clear();
    
    qDeleteAll(m_simulatedRFDetectors);
    m_simulatedRFDetectors.clear();
    
    qDeleteAll(m_simulatedCameras);
    m_simulatedCameras.clear();
    
    // Clean up owned simulated effectors
    qDeleteAll(m_simulatedJammers);
    m_simulatedJammers.clear();
    
    qDeleteAll(m_simulatedInterceptors);
    m_simulatedInterceptors.clear();
    
    qDeleteAll(m_simulatedDESystems);
    m_simulatedDESystems.clear();
    
    // Reset statistics
    m_stats = SimulationStats();
    
    Logger::instance().info("SystemSimulationManager", "Simulation reset");
}

void SystemSimulationManager::setTimeScale(double scale) {
    m_timeScale = qBound(0.1, scale, 10.0);
}

qint64 SystemSimulationManager::simulationTime() const {
    if (!m_running) return 0;
    
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    return static_cast<qint64>((now - m_startTime) * m_timeScale);
}

void SystemSimulationManager::loadScenario(const QString& scenarioPath) {
    QFile file(scenarioPath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit error("Failed to open scenario file: " + scenarioPath);
        return;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    m_scenario = SimulationScenario::fromJson(doc.object());
    m_basePosition = m_scenario.basePosition;
    
    if (m_trackSimulator) m_trackSimulator->setBasePosition(m_basePosition);
    if (m_sensorSimulator) m_sensorSimulator->setBasePosition(m_basePosition);
    
    Logger::instance().info("SystemSimulationManager", 
        "Loaded scenario: " + m_scenario.name);
    
    emit scenarioLoaded(m_scenario.name);
}

void SystemSimulationManager::saveScenario(const QString& scenarioPath) {
    QFile file(scenarioPath);
    if (!file.open(QIODevice::WriteOnly)) {
        emit error("Failed to save scenario file: " + scenarioPath);
        return;
    }
    
    QJsonDocument doc(m_scenario.toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    
    Logger::instance().info("SystemSimulationManager", 
        "Saved scenario: " + scenarioPath);
}

void SystemSimulationManager::setScenario(const SimulationScenario& scenario) {
    m_scenario = scenario;
    m_basePosition = scenario.basePosition;
    
    if (m_trackSimulator) m_trackSimulator->setBasePosition(m_basePosition);
    if (m_sensorSimulator) m_sensorSimulator->setBasePosition(m_basePosition);
    
    emit scenarioLoaded(scenario.name);
}

void SystemSimulationManager::loadDefaultScenario() {
    m_scenario.name = "Default Scenario";
    m_scenario.description = "Standard counter-UAS simulation with moderate threat activity";
    m_scenario.basePosition = m_basePosition;
    m_scenario.durationMinutes = 10;
    m_scenario.maxTargets = 10;
    m_scenario.threatSpawnRate = 0.2;
    m_scenario.enableRadarSim = true;
    m_scenario.enableRFSim = true;
    m_scenario.enableCameraSim = true;
    m_scenario.enableVideoSim = true;
    m_scenario.enableEffectorSim = true;
    m_scenario.weatherFactor = 1.0;
    m_scenario.clutterLevel = 0.1;
    m_scenario.noiseLevel = 0.05;
    m_scenario.hostileProbability = 0.7;
    
    emit scenarioLoaded(m_scenario.name);
}

void SystemSimulationManager::loadSwarmAttackScenario() {
    m_scenario.name = "Swarm Attack";
    m_scenario.description = "Multiple coordinated drone threats approaching from multiple directions";
    m_scenario.basePosition = m_basePosition;
    m_scenario.durationMinutes = 5;
    m_scenario.maxTargets = 20;
    m_scenario.threatSpawnRate = 1.0;
    m_scenario.enableRadarSim = true;
    m_scenario.enableRFSim = true;
    m_scenario.enableCameraSim = true;
    m_scenario.enableVideoSim = true;
    m_scenario.enableEffectorSim = true;
    m_scenario.weatherFactor = 0.9;
    m_scenario.clutterLevel = 0.2;
    m_scenario.noiseLevel = 0.1;
    m_scenario.minThreatLevel = 3;
    m_scenario.maxThreatLevel = 5;
    m_scenario.hostileProbability = 0.95;
    
    emit scenarioLoaded(m_scenario.name);
}

void SystemSimulationManager::loadMultiThreatScenario() {
    m_scenario.name = "Multi-Threat Environment";
    m_scenario.description = "Mixed threat types with varying classifications";
    m_scenario.basePosition = m_basePosition;
    m_scenario.durationMinutes = 15;
    m_scenario.maxTargets = 15;
    m_scenario.threatSpawnRate = 0.3;
    m_scenario.enableRadarSim = true;
    m_scenario.enableRFSim = true;
    m_scenario.enableCameraSim = true;
    m_scenario.enableVideoSim = true;
    m_scenario.enableEffectorSim = true;
    m_scenario.weatherFactor = 0.8;
    m_scenario.clutterLevel = 0.15;
    m_scenario.noiseLevel = 0.08;
    m_scenario.minThreatLevel = 1;
    m_scenario.maxThreatLevel = 5;
    m_scenario.hostileProbability = 0.5;  // Mixed friendly/hostile
    
    emit scenarioLoaded(m_scenario.name);
}

void SystemSimulationManager::loadStressTestScenario() {
    m_scenario.name = "Stress Test";
    m_scenario.description = "Maximum load testing with high target count and noise";
    m_scenario.basePosition = m_basePosition;
    m_scenario.durationMinutes = 3;
    m_scenario.maxTargets = 50;
    m_scenario.threatSpawnRate = 2.0;
    m_scenario.enableRadarSim = true;
    m_scenario.enableRFSim = true;
    m_scenario.enableCameraSim = true;
    m_scenario.enableVideoSim = true;
    m_scenario.enableEffectorSim = true;
    m_scenario.weatherFactor = 0.6;
    m_scenario.clutterLevel = 0.4;
    m_scenario.noiseLevel = 0.2;
    m_scenario.hostileProbability = 0.8;
    
    emit scenarioLoaded(m_scenario.name);
}

void SystemSimulationManager::registerRadar(RadarSensor* radar) {
    if (m_sensorSimulator) {
        m_sensorSimulator->registerRadar(radar);
    }
}

void SystemSimulationManager::registerRFDetector(RFDetector* detector) {
    if (m_sensorSimulator) {
        m_sensorSimulator->registerRFDetector(detector);
    }
}

void SystemSimulationManager::registerCamera(CameraSystem* camera) {
    if (m_sensorSimulator) {
        m_sensorSimulator->registerCamera(camera);
    }
}

void SystemSimulationManager::registerRFJammer(RFJammer* jammer) {
    if (m_effectorSimulator) {
        m_effectorSimulator->registerRFJammer(jammer);
    }
    if (m_engagementManager) {
        m_engagementManager->registerEffector(jammer);
    }
}

void SystemSimulationManager::registerKineticInterceptor(KineticInterceptor* interceptor) {
    if (m_effectorSimulator) {
        m_effectorSimulator->registerKineticInterceptor(interceptor);
    }
    if (m_engagementManager) {
        m_engagementManager->registerEffector(interceptor);
    }
}

void SystemSimulationManager::registerDirectedEnergy(DirectedEnergySystem* de) {
    if (m_effectorSimulator) {
        m_effectorSimulator->registerDirectedEnergy(de);
    }
    if (m_engagementManager) {
        m_engagementManager->registerEffector(de);
    }
}

void SystemSimulationManager::createDefaultSensors() {
    // Clean up any existing sensors first to prevent memory leaks
    if (!m_simulatedRadars.isEmpty() || !m_simulatedRFDetectors.isEmpty() || 
        !m_simulatedCameras.isEmpty()) {
        if (m_sensorSimulator) {
            m_sensorSimulator->clearSensors();
        }
        qDeleteAll(m_simulatedRadars);
        m_simulatedRadars.clear();
        qDeleteAll(m_simulatedRFDetectors);
        m_simulatedRFDetectors.clear();
        qDeleteAll(m_simulatedCameras);
        m_simulatedCameras.clear();
    }
    
    // Create simulated radar
    auto* radar1 = new RadarSensor("SIM-RADAR-001", this);
    RadarConfig radarConfig;
    radarConfig.host = "127.0.0.1";
    radarConfig.port = 5001;
    radarConfig.maxRangeM = 5000.0;
    radar1->setConfig(radarConfig);
    radar1->setName("Primary Surveillance Radar");
    radar1->setPosition(m_basePosition);
    m_simulatedRadars.append(radar1);
    registerRadar(radar1);
    
    // Create secondary radar
    auto* radar2 = new RadarSensor("SIM-RADAR-002", this);
    RadarConfig radar2Config;
    radar2Config.maxRangeM = 3000.0;
    radar2->setConfig(radar2Config);
    radar2->setName("Secondary Radar");
    GeoPosition radar2Pos = m_basePosition;
    radar2Pos.latitude += 0.001;
    radar2->setPosition(radar2Pos);
    m_simulatedRadars.append(radar2);
    registerRadar(radar2);
    
    // Create RF detector
    auto* rfDetector = new RFDetector("SIM-RF-001", this);
    rfDetector->setName("RF Detection System");
    rfDetector->setPosition(m_basePosition);
    m_simulatedRFDetectors.append(rfDetector);
    registerRFDetector(rfDetector);
    
    // Create cameras
    auto* camera1 = new CameraSystem("SIM-CAM-001", this);
    CameraSystemConfig camConfig;
    camConfig.cameraName = "Main EO Tracker";
    camConfig.hasPTZ = true;
    camConfig.mountPosition = m_basePosition;
    camera1->setConfig(camConfig);
    m_simulatedCameras.append(camera1);
    registerCamera(camera1);
    
    auto* camera2 = new CameraSystem("SIM-CAM-002", this);
    CameraSystemConfig cam2Config;
    cam2Config.cameraName = "Thermal Camera";
    cam2Config.hasPTZ = true;
    cam2Config.mountPosition = m_basePosition;
    camera2->setConfig(cam2Config);
    m_simulatedCameras.append(camera2);
    registerCamera(camera2);
    
    Logger::instance().info("SystemSimulationManager", 
        QString("Created %1 simulated sensors")
            .arg(m_simulatedRadars.size() + m_simulatedRFDetectors.size() + 
                 m_simulatedCameras.size()));
}

void SystemSimulationManager::createDefaultEffectors() {
    // Clean up any existing effectors first to prevent memory leaks
    if (!m_simulatedJammers.isEmpty() || !m_simulatedInterceptors.isEmpty() || 
        !m_simulatedDESystems.isEmpty()) {
        if (m_effectorSimulator) {
            m_effectorSimulator->clearEffectors();
        }
        // Also unregister from engagement manager
        if (m_engagementManager) {
            for (auto* jammer : m_simulatedJammers) {
                m_engagementManager->unregisterEffector(jammer->effectorId());
            }
            for (auto* interceptor : m_simulatedInterceptors) {
                m_engagementManager->unregisterEffector(interceptor->effectorId());
            }
            for (auto* de : m_simulatedDESystems) {
                m_engagementManager->unregisterEffector(de->effectorId());
            }
        }
        qDeleteAll(m_simulatedJammers);
        m_simulatedJammers.clear();
        qDeleteAll(m_simulatedInterceptors);
        m_simulatedInterceptors.clear();
        qDeleteAll(m_simulatedDESystems);
        m_simulatedDESystems.clear();
    }
    
    // Create RF Jammer
    auto* jammer1 = new RFJammer("SIM-JAMMER-001", this);
    jammer1->setDisplayName("RF Jammer Alpha");
    jammer1->setPosition(m_basePosition);
    jammer1->initialize();
    m_simulatedJammers.append(jammer1);
    registerRFJammer(jammer1);
    
    auto* jammer2 = new RFJammer("SIM-JAMMER-002", this);
    jammer2->setDisplayName("RF Jammer Bravo");
    GeoPosition jammer2Pos = m_basePosition;
    jammer2Pos.longitude += 0.001;
    jammer2->setPosition(jammer2Pos);
    jammer2->initialize();
    m_simulatedJammers.append(jammer2);
    registerRFJammer(jammer2);
    
    // Create Kinetic Interceptor
    auto* interceptor = new KineticInterceptor("SIM-KINETIC-001", this);
    interceptor->setDisplayName("Interceptor Launcher");
    interceptor->setPosition(m_basePosition);
    interceptor->initialize();
    m_simulatedInterceptors.append(interceptor);
    registerKineticInterceptor(interceptor);
    
    // Create Directed Energy System
    auto* de = new DirectedEnergySystem("SIM-DE-001", this);
    de->setDisplayName("Directed Energy System");
    de->setPosition(m_basePosition);
    de->initialize();
    m_simulatedDESystems.append(de);
    registerDirectedEnergy(de);
    
    Logger::instance().info("SystemSimulationManager", 
        QString("Created %1 simulated effectors")
            .arg(m_simulatedJammers.size() + m_simulatedInterceptors.size() + 
                 m_simulatedDESystems.size()));
}

void SystemSimulationManager::createFullSimulationEnvironment() {
    createDefaultSensors();
    createDefaultEffectors();
    
    // Setup video simulation
    if (m_videoSimulator) {
        m_videoSimulator->setupDefaultCameras();
    }
    
    Logger::instance().info("SystemSimulationManager", 
        "Full simulation environment created");
}

void SystemSimulationManager::updateSimulation() {
    if (!m_running || m_paused) return;
    
    // Check if scenario duration exceeded
    if (m_scenario.durationMinutes > 0) {
        qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - m_startTime;
        qint64 maxDuration = m_scenario.durationMinutes * 60 * 1000;
        
        if (elapsed >= maxDuration) {
            Logger::instance().info("SystemSimulationManager", "Scenario duration completed");
            stop();
            return;
        }
        
        int progress = static_cast<int>((elapsed * 100) / maxDuration);
        emit simulationProgress(progress);
    }
}

void SystemSimulationManager::updateStatistics() {
    if (!m_running) return;
    
    m_stats.elapsedMs = QDateTime::currentMSecsSinceEpoch() - m_startTime;
    
    if (m_trackManager) {
        m_stats.totalTracksCreated = m_trackManager->trackCount();
    }
    
    if (m_sensorSimulator) {
        auto sensorStats = m_sensorSimulator->statistics();
        m_stats.sensorDetections = sensorStats.totalDetections;
    }
    
    emit statisticsUpdated(m_stats);
}

void SystemSimulationManager::onTrackCreated(const QString& trackId) {
    m_stats.totalTargetsSpawned++;
    
    if (m_trackManager) {
        Track* track = m_trackManager->track(trackId);
        if (track) {
            emit targetSpawned(trackId, track->position());
        }
    }
}

void SystemSimulationManager::onTrackDropped(const QString& trackId) {
    emit targetDestroyed(trackId);
}

void SystemSimulationManager::onEngagementComplete(const QString& engagementId, bool success) {
    m_stats.totalEngagements++;
    if (success) {
        m_stats.successfulEngagements++;
    }
}

void SystemSimulationManager::setBasePosition(const GeoPosition& pos) {
    m_basePosition = pos;
    m_scenario.basePosition = pos;
    
    if (m_trackSimulator) m_trackSimulator->setBasePosition(pos);
    if (m_sensorSimulator) m_sensorSimulator->setBasePosition(pos);
}

} // namespace CounterUAS
