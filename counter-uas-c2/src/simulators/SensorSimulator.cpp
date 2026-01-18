/**
 * @file SensorSimulator.cpp
 * @brief Implementation of comprehensive sensor simulation
 */

#include "simulators/SensorSimulator.h"
#include "core/TrackManager.h"
#include "sensors/RadarSensor.h"
#include "sensors/RFDetector.h"
#include "sensors/CameraSystem.h"
#include "utils/Logger.h"
#include <QDateTime>
#include <QRandomGenerator>
#include <QtMath>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace CounterUAS {

SensorSimulator::SensorSimulator(TrackManager* manager, QObject* parent)
    : QObject(parent)
    , m_trackManager(manager)
    , m_updateTimer(new QTimer(this))
    , m_detectionTimer(new QTimer(this))
{
    m_basePosition.latitude = 34.0522;
    m_basePosition.longitude = -118.2437;
    m_basePosition.altitude = 100.0;
    
    connect(m_updateTimer, &QTimer::timeout, this, &SensorSimulator::updateSimulation);
    connect(m_detectionTimer, &QTimer::timeout, this, &SensorSimulator::generateDetection);
}

SensorSimulator::~SensorSimulator() {
    stop();
}

void SensorSimulator::setTrackManager(TrackManager* manager) {
    m_trackManager = manager;
}

void SensorSimulator::setBasePosition(const GeoPosition& pos) {
    m_basePosition = pos;
}

void SensorSimulator::start() {
    if (m_running) return;
    
    m_running = true;
    m_updateTimer->start(1000 / m_updateRateHz);
    m_detectionTimer->start(500);  // Generate detections every 500ms
    
    Logger::instance().info("SensorSimulator", "Simulation started");
    emit simulationStarted();
}

void SensorSimulator::stop() {
    if (!m_running) return;
    
    m_running = false;
    m_updateTimer->stop();
    m_detectionTimer->stop();
    
    Logger::instance().info("SensorSimulator", "Simulation stopped");
    emit simulationStopped();
}

void SensorSimulator::setUpdateRate(int hz) {
    m_updateRateHz = qBound(1, hz, 100);
    if (m_running) {
        m_updateTimer->setInterval(1000 / m_updateRateHz);
    }
}

void SensorSimulator::registerRadar(RadarSensor* radar) {
    if (!radar) return;
    
    QString id = radar->sensorId();
    m_radars[id] = radar;
    
    RadarSimState state;
    state.active = true;
    m_radarStates[id] = state;
    
    Logger::instance().info("SensorSimulator", "Registered Radar: " + id);
}

void SensorSimulator::registerRFDetector(RFDetector* detector) {
    if (!detector) return;
    
    QString id = detector->sensorId();
    m_rfDetectors[id] = detector;
    
    RFDetectorSimState state;
    state.active = true;
    m_rfDetectorStates[id] = state;
    
    Logger::instance().info("SensorSimulator", "Registered RF Detector: " + id);
}

void SensorSimulator::registerCamera(CameraSystem* camera) {
    if (!camera) return;
    
    QString id = camera->sensorId();
    m_cameras[id] = camera;
    
    CameraSimState state;
    state.active = true;
    m_cameraStates[id] = state;
    
    Logger::instance().info("SensorSimulator", "Registered Camera: " + id);
}

void SensorSimulator::unregisterSensor(const QString& sensorId) {
    m_radars.remove(sensorId);
    m_radarStates.remove(sensorId);
    m_rfDetectors.remove(sensorId);
    m_rfDetectorStates.remove(sensorId);
    m_cameras.remove(sensorId);
    m_cameraStates.remove(sensorId);
}

void SensorSimulator::clearSensors() {
    m_radars.clear();
    m_radarStates.clear();
    m_rfDetectors.clear();
    m_rfDetectorStates.clear();
    m_cameras.clear();
    m_cameraStates.clear();
}

RadarSimState SensorSimulator::radarState(const QString& id) const {
    return m_radarStates.value(id);
}

RFDetectorSimState SensorSimulator::rfDetectorState(const QString& id) const {
    return m_rfDetectorStates.value(id);
}

CameraSimState SensorSimulator::cameraState(const QString& id) const {
    return m_cameraStates.value(id);
}

void SensorSimulator::injectRadarTarget(const SimulatedRadarTarget& target) {
    m_injectedRadarTargets.append(target);
}

void SensorSimulator::injectRFEmission(const SimulatedRFEmission& emission) {
    m_injectedRFEmissions.append(emission);
}

void SensorSimulator::injectVisualTarget(const SimulatedVisualTarget& target) {
    m_injectedVisualTargets.append(target);
}

void SensorSimulator::clearInjectedTargets() {
    m_injectedRadarTargets.clear();
    m_injectedRFEmissions.clear();
    m_injectedVisualTargets.clear();
}

void SensorSimulator::loadScenario(const QString& scenarioFile) {
    QFile file(scenarioFile);
    if (!file.open(QIODevice::ReadOnly)) {
        Logger::instance().error("SensorSimulator", "Failed to open scenario: " + scenarioFile);
        return;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();
    
    // Load radar targets
    QJsonArray radarTargets = root["radarTargets"].toArray();
    for (const QJsonValue& v : radarTargets) {
        QJsonObject obj = v.toObject();
        SimulatedRadarTarget target;
        target.id = obj["id"].toString();
        target.position.latitude = obj["latitude"].toDouble();
        target.position.longitude = obj["longitude"].toDouble();
        target.position.altitude = obj["altitude"].toDouble();
        target.velocity.north = obj["velocityNorth"].toDouble();
        target.velocity.east = obj["velocityEast"].toDouble();
        target.velocity.down = obj["velocityDown"].toDouble();
        target.rcs = obj["rcs"].toDouble(0.1);
        m_injectedRadarTargets.append(target);
    }
    
    // Load RF emissions
    QJsonArray rfEmissions = root["rfEmissions"].toArray();
    for (const QJsonValue& v : rfEmissions) {
        QJsonObject obj = v.toObject();
        SimulatedRFEmission emission;
        emission.sourceId = obj["sourceId"].toString();
        emission.position.latitude = obj["latitude"].toDouble();
        emission.position.longitude = obj["longitude"].toDouble();
        emission.position.altitude = obj["altitude"].toDouble();
        emission.frequencyMHz = obj["frequencyMHz"].toDouble(2400.0);
        emission.signalStrengthDbm = obj["signalDbm"].toDouble(-60.0);
        emission.protocol = obj["protocol"].toString("Unknown");
        m_injectedRFEmissions.append(emission);
    }
    
    Logger::instance().info("SensorSimulator", 
        QString("Loaded scenario: %1 radar targets, %2 RF emissions")
            .arg(m_injectedRadarTargets.size())
            .arg(m_injectedRFEmissions.size()));
}

void SensorSimulator::startScenario() {
    start();
}

void SensorSimulator::stopScenario() {
    stop();
}

void SensorSimulator::updateSimulation() {
    updateRadarSimulation();
    updateRFDetectorSimulation();
    updateCameraSimulation();
}

void SensorSimulator::updateRadarSimulation() {
    double dt = 1.0 / m_updateRateHz;
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    
    // Update injected targets
    for (auto& target : m_injectedRadarTargets) {
        target.position.latitude += (target.velocity.north * dt) / 111000.0;
        target.position.longitude += (target.velocity.east * dt) / 
            (111000.0 * std::cos(qDegreesToRadians(target.position.latitude)));
        target.position.altitude -= target.velocity.down * dt;
        target.lastUpdateTime = now;
    }
    
    // Update radar states
    for (auto it = m_radarStates.begin(); it != m_radarStates.end(); ++it) {
        const QString& id = it.key();
        RadarSimState& state = it.value();
        
        if (!state.active) continue;
        
        // Rotate scan angle (for scanning radar simulation)
        state.scanAngle += 30.0 * dt;  // 30 degrees per second
        if (state.scanAngle >= 360.0) state.scanAngle -= 360.0;
        
        state.lastScanTime = now;
        
        emit radarStateChanged(id, state);
    }
}

void SensorSimulator::updateRFDetectorSimulation() {
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    
    // Update injected emissions
    for (auto& emission : m_injectedRFEmissions) {
        // Add some frequency drift in realistic mode
        if (m_realisticMode) {
            emission.frequencyMHz += (QRandomGenerator::global()->generateDouble() - 0.5) * 0.1;
            emission.signalStrengthDbm += (QRandomGenerator::global()->generateDouble() - 0.5) * 2.0;
        }
        emission.lastUpdateTime = now;
    }
    
    for (auto it = m_rfDetectorStates.begin(); it != m_rfDetectorStates.end(); ++it) {
        const QString& id = it.key();
        RFDetectorSimState& state = it.value();
        
        if (!state.active) continue;
        
        // Update noise floor
        if (m_realisticMode) {
            state.noiseFloor = -90.0 + m_noiseLevel * 20.0 + 
                (QRandomGenerator::global()->generateDouble() - 0.5) * 5.0;
        }
        
        emit rfDetectorStateChanged(id, state);
    }
}

void SensorSimulator::updateCameraSimulation() {
    double dt = 1.0 / m_updateRateHz;
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    
    // Update injected visual targets
    for (auto& target : m_injectedVisualTargets) {
        target.position.latitude += (target.velocity.north * dt) / 111000.0;
        target.position.longitude += (target.velocity.east * dt) / 
            (111000.0 * std::cos(qDegreesToRadians(target.position.latitude)));
        target.position.altitude -= target.velocity.down * dt;
        target.lastDetectionTime = now;
    }
    
    for (auto it = m_cameraStates.begin(); it != m_cameraStates.end(); ++it) {
        const QString& id = it.key();
        CameraSimState& state = it.value();
        
        if (!state.active) continue;
        
        emit cameraStateChanged(id, state);
    }
}

void SensorSimulator::generateDetection() {
    if (!m_running || !m_trackManager) return;
    
    // Generate detections from each sensor type
    for (const QString& radarId : m_radars.keys()) {
        simulateRadarScan(radarId);
    }
    
    for (const QString& detectorId : m_rfDetectors.keys()) {
        simulateRFScan(detectorId);
    }
    
    for (const QString& cameraId : m_cameras.keys()) {
        simulateCameraFrame(cameraId);
    }
    
    // Occasionally spawn a new target if none exist
    if (m_injectedRadarTargets.isEmpty() && 
        QRandomGenerator::global()->generateDouble() < 0.1) {
        SimulatedRadarTarget target = createDroneTarget();
        m_injectedRadarTargets.append(target);
        
        // Also create associated RF emission
        SimulatedRFEmission emission = createDroneEmission(target.position);
        emission.sourceId = target.id;
        m_injectedRFEmissions.append(emission);
        
        // And visual target
        SimulatedVisualTarget visual;
        visual.id = target.id;
        visual.position = target.position;
        visual.velocity = target.velocity;
        visual.objectClass = "drone";
        m_injectedVisualTargets.append(visual);
        
        Logger::instance().info("SensorSimulator", 
            QString("Spawned new simulated target: %1").arg(target.id));
    }
    
    // Remove targets that are too far or have passed
    m_injectedRadarTargets.erase(
        std::remove_if(m_injectedRadarTargets.begin(), m_injectedRadarTargets.end(),
            [this](const SimulatedRadarTarget& t) {
                Track temp("temp");
                temp.setPosition(m_basePosition);
                double dist = temp.distanceTo(t.position);
                return dist > 5000.0 || dist < 50.0;
            }), m_injectedRadarTargets.end());
    
    // Clean up associated emissions and visual targets
    QSet<QString> activeIds;
    for (const auto& t : m_injectedRadarTargets) {
        activeIds.insert(t.id);
    }
    
    m_injectedRFEmissions.erase(
        std::remove_if(m_injectedRFEmissions.begin(), m_injectedRFEmissions.end(),
            [&activeIds](const SimulatedRFEmission& e) {
                return !activeIds.contains(e.sourceId);
            }), m_injectedRFEmissions.end());
    
    m_injectedVisualTargets.erase(
        std::remove_if(m_injectedVisualTargets.begin(), m_injectedVisualTargets.end(),
            [&activeIds](const SimulatedVisualTarget& v) {
                return !activeIds.contains(v.id);
            }), m_injectedVisualTargets.end());
}

void SensorSimulator::simulateRadarScan(const QString& radarId) {
    auto* radar = m_radars.value(radarId);
    if (!radar) return;
    
    RadarSimState& state = m_radarStates[radarId];
    auto* gen = QRandomGenerator::global();
    
    state.currentTargets.clear();
    state.detectedTargets = 0;
    
    GeoPosition radarPos = radar->position();
    double maxRange = radar->maxRange();
    
    // Process injected targets
    for (const auto& target : m_injectedRadarTargets) {
        Track temp("temp");
        temp.setPosition(radarPos);
        double range = temp.distanceTo(target.position);
        
        if (range > maxRange) continue;
        
        // Calculate detection probability
        double pd = calculateDetectionProbability(range, maxRange, target.rcs);
        
        if (m_realisticMode) {
            pd *= m_detectionProbability;
            pd *= (1.0 - m_noiseLevel * 0.5);
        }
        
        if (gen->generateDouble() < pd) {
            // Detection successful
            GeoPosition detectedPos = target.position;
            VelocityVector detectedVel = target.velocity;
            
            // Add measurement noise
            if (m_realisticMode) {
                double rangeError = range * 0.01 * (gen->generateDouble() - 0.5);  // 1% range error
                double azError = 0.5 * (gen->generateDouble() - 0.5);  // 0.5 deg azimuth error
                
                detectedPos.latitude += rangeError * 0.00001;
                detectedPos.longitude += azError * 0.00001;
                detectedPos.altitude += gen->generateDouble() * 10.0 - 5.0;
                
                detectedVel.north += (gen->generateDouble() - 0.5) * 2.0;
                detectedVel.east += (gen->generateDouble() - 0.5) * 2.0;
            }
            
            double quality = target.trackQuality;
            if (m_realisticMode) {
                quality *= (0.9 + gen->generateDouble() * 0.2);
            }
            
            state.currentTargets.append(target);
            state.detectedTargets++;
            
            // Report to track manager
            m_trackManager->processRadarDetection(detectedPos, detectedVel, quality,
                                                  QDateTime::currentMSecsSinceEpoch());
            
            emit radarDetection(radarId, detectedPos, detectedVel, quality);
            m_stats.radarDetections++;
            m_stats.totalDetections++;
        } else {
            m_stats.missedDetections++;
        }
    }
    
    // Generate clutter
    if (m_clutterLevel > 0.0) {
        generateClutter(radarId);
    }
    
    emit radarStateChanged(radarId, state);
}

void SensorSimulator::simulateRFScan(const QString& detectorId) {
    auto* detector = m_rfDetectors.value(detectorId);
    if (!detector) return;
    
    RFDetectorSimState& state = m_rfDetectorStates[detectorId];
    auto* gen = QRandomGenerator::global();
    
    state.currentEmissions.clear();
    state.detectedEmissions = 0;
    
    GeoPosition detectorPos = detector->position();
    double maxRange = detector->maxRange();
    
    // Process injected emissions
    for (const auto& emission : m_injectedRFEmissions) {
        if (!emission.isActive) continue;
        
        Track temp("temp");
        temp.setPosition(detectorPos);
        double range = temp.distanceTo(emission.position);
        
        if (range > maxRange) continue;
        
        // Calculate received signal strength
        double pathLoss = 32.44 + 20.0 * std::log10(emission.frequencyMHz) + 
                         20.0 * std::log10(range / 1000.0);
        double receivedPower = emission.signalStrengthDbm - pathLoss;
        
        // Check if above noise floor
        if (receivedPower > state.noiseFloor + 10.0) {
            state.currentEmissions.append(emission);
            state.detectedEmissions++;
            state.identifiedProtocols[emission.sourceId] = emission.protocol;
            
            // Estimate position (with error)
            GeoPosition estimatedPos = emission.position;
            if (m_realisticMode) {
                double posError = range * 0.1;  // 10% range error for RF
                estimatedPos.latitude += posError * 0.00001 * (gen->generateDouble() - 0.5);
                estimatedPos.longitude += posError * 0.00001 * (gen->generateDouble() - 0.5);
            }
            
            // Report to track manager - normalize signal strength to 0-1 range
            double normalizedSignal = (receivedPower + 100.0) / 100.0;
            m_trackManager->processRFDetection(estimatedPos, normalizedSignal,
                                              QDateTime::currentMSecsSinceEpoch());
            
            emit rfDetection(detectorId, estimatedPos, emission.frequencyMHz, 
                           receivedPower, emission.protocol);
            m_stats.rfDetections++;
            m_stats.totalDetections++;
        }
    }
    
    // Generate RF noise
    if (m_noiseLevel > 0.0) {
        generateRFNoise(detectorId);
    }
    
    emit rfDetectorStateChanged(detectorId, state);
}

void SensorSimulator::simulateCameraFrame(const QString& cameraId) {
    auto* camera = m_cameras.value(cameraId);
    if (!camera) return;
    
    CameraSimState& state = m_cameraStates[cameraId];
    auto* gen = QRandomGenerator::global();
    
    state.visibleTargets.clear();
    state.detectedObjects = 0;
    
    GeoPosition cameraPos = camera->position();
    double maxRange = camera->maxRange();
    double fov = camera->fieldOfView();
    double azimuth = camera->azimuth();
    
    // Process injected visual targets
    for (const auto& target : m_injectedVisualTargets) {
        Track temp("temp");
        temp.setPosition(cameraPos);
        double range = temp.distanceTo(target.position);
        
        if (range > maxRange) continue;
        
        // Check if target is in field of view
        double dLat = target.position.latitude - cameraPos.latitude;
        double dLon = target.position.longitude - cameraPos.longitude;
        double northOffset = dLat * 111000.0;
        double eastOffset = dLon * 111000.0 * std::cos(qDegreesToRadians(cameraPos.latitude));
        
        double targetAzimuth = qRadiansToDegrees(std::atan2(eastOffset, northOffset));
        double azDiff = targetAzimuth - azimuth;
        while (azDiff > 180.0) azDiff -= 360.0;
        while (azDiff < -180.0) azDiff += 360.0;
        
        if (std::abs(azDiff) > fov / 2.0) continue;
        
        // Calculate detection probability based on size and range
        double angularSize = target.size / range;
        double pd = target.detectability * std::min(1.0, angularSize * 1000.0);
        
        if (m_realisticMode) {
            pd *= m_detectionProbability;
            pd *= (1.0 - m_noiseLevel * 0.3);
        }
        
        if (gen->generateDouble() < pd) {
            // Calculate bounding box (normalized coordinates)
            double boxSize = std::min(0.3, angularSize * 500.0);
            double centerX = 0.5 + azDiff / fov;
            double centerY = 0.5;  // Simplified vertical positioning
            
            QRectF bbox(centerX - boxSize/2, centerY - boxSize/2, boxSize, boxSize);
            
            state.visibleTargets.append(target);
            state.detectedObjects++;
            
            // Report detection
            double confidence = pd * (0.8 + gen->generateDouble() * 0.2);
            
            m_trackManager->processCameraDetection(cameraId, BoundingBox(), target.position,
                                                   QDateTime::currentMSecsSinceEpoch());
            
            emit cameraDetection(cameraId, bbox, target.objectClass, confidence);
            m_stats.cameraDetections++;
            m_stats.totalDetections++;
        }
    }
    
    emit cameraStateChanged(cameraId, state);
}

GeoPosition SensorSimulator::generateRandomTargetPosition(double minRange, double maxRange) {
    auto* gen = QRandomGenerator::global();
    
    double range = minRange + gen->generateDouble() * (maxRange - minRange);
    double bearing = gen->generateDouble() * 360.0;
    double bearingRad = qDegreesToRadians(bearing);
    
    GeoPosition pos;
    pos.latitude = m_basePosition.latitude + (range * std::cos(bearingRad)) / 111000.0;
    pos.longitude = m_basePosition.longitude + 
                   (range * std::sin(bearingRad)) / 
                   (111000.0 * std::cos(qDegreesToRadians(m_basePosition.latitude)));
    pos.altitude = m_basePosition.altitude + 50.0 + gen->generateDouble() * 200.0;
    
    return pos;
}

VelocityVector SensorSimulator::generateTargetVelocity(const GeoPosition& pos, double speed) {
    auto* gen = QRandomGenerator::global();
    
    // Calculate bearing toward base
    double dLat = m_basePosition.latitude - pos.latitude;
    double dLon = m_basePosition.longitude - pos.longitude;
    double bearing = qRadiansToDegrees(std::atan2(dLon, dLat));
    
    // Add some randomness to heading
    bearing += (gen->generateDouble() * 40.0 - 20.0);
    double bearingRad = qDegreesToRadians(bearing);
    
    VelocityVector vel;
    vel.north = speed * std::cos(bearingRad);
    vel.east = speed * std::sin(bearingRad);
    vel.down = gen->generateDouble() * 2.0 - 1.0;
    
    return vel;
}

double SensorSimulator::calculateDetectionProbability(double range, double maxRange, double rcs) {
    // Blake's radar range equation simplified
    double rangeFactor = 1.0 - std::pow(range / maxRange, 2);
    double rcsFactor = std::sqrt(rcs / 0.1);  // Normalize to 0.1 m² reference
    
    return qBound(0.0, rangeFactor * rcsFactor, 1.0);
}

void SensorSimulator::generateClutter(const QString& radarId) {
    auto* gen = QRandomGenerator::global();
    int clutterCount = static_cast<int>(m_clutterLevel * 10 * gen->generateDouble());
    
    RadarSimState& state = m_radarStates[radarId];
    state.clutterReturns = clutterCount;
    
    for (int i = 0; i < clutterCount; i++) {
        // Generate clutter at random positions (but not report to track manager)
        GeoPosition clutterPos = generateRandomTargetPosition(100, 3000);
        
        SimulatedRadarTarget clutter;
        clutter.id = QString("CLUTTER-%1").arg(i);
        clutter.position = clutterPos;
        clutter.rcs = 0.01;  // Small RCS
        clutter.isClutter = true;
        
        // Clutter doesn't get reported to track manager
        m_stats.falseAlarms++;
    }
    
    emit clutterGenerated(radarId, clutterCount);
}

void SensorSimulator::generateRFNoise(const QString& detectorId) {
    auto* gen = QRandomGenerator::global();
    
    // Occasionally generate a false RF detection
    if (gen->generateDouble() < m_noiseLevel * 0.1) {
        m_stats.falseAlarms++;
    }
}

SimulatedRadarTarget SensorSimulator::createDroneTarget() {
    auto* gen = QRandomGenerator::global();
    
    SimulatedRadarTarget target;
    target.id = QString("TGT-%1").arg(m_nextTargetId++, 4, 10, QChar('0'));
    target.position = generateRandomTargetPosition(1500, 3000);
    
    double speed = 8.0 + gen->generateDouble() * 15.0;
    target.velocity = generateTargetVelocity(target.position, speed);
    
    target.rcs = 0.05 + gen->generateDouble() * 0.15;  // 0.05-0.2 m²
    target.trackQuality = 0.8 + gen->generateDouble() * 0.2;
    target.isClutter = false;
    target.lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    
    return target;
}

SimulatedRFEmission SensorSimulator::createDroneEmission(const GeoPosition& pos) {
    auto* gen = QRandomGenerator::global();
    
    SimulatedRFEmission emission;
    emission.sourceId = "";  // Will be set by caller
    emission.position = pos;
    
    // Common drone frequencies
    QList<double> frequencies = {2400.0, 2450.0, 5200.0, 5500.0, 5800.0};
    emission.frequencyMHz = frequencies[gen->bounded(frequencies.size())];
    
    emission.signalStrengthDbm = -50.0 - gen->generateDouble() * 20.0;
    emission.bandwidthMHz = 10.0 + gen->generateDouble() * 30.0;
    
    // Common protocols
    QList<QString> protocols = {"DJI_OcuSync", "DJI_Lightbridge", "FrSky", "Generic_2.4GHz"};
    emission.protocol = protocols[gen->bounded(protocols.size())];
    
    emission.isActive = true;
    emission.lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    
    return emission;
}

} // namespace CounterUAS
