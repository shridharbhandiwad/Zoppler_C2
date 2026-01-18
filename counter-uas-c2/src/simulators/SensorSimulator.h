/**
 * @file SensorSimulator.h
 * @brief Complete sensor simulation for Counter-UAS C2 system
 * 
 * Provides realistic simulation of:
 * - Radar sensors with track reports, clutter, and multi-target tracking
 * - RF Detection sensors with signal analysis and protocol identification
 * - Camera sensors with detection bounding boxes and visual tracking
 */

#ifndef SENSORSIMULATOR_H
#define SENSORSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QHash>
#include <QList>
#include "core/Track.h"
#include "sensors/SensorInterface.h"

namespace CounterUAS {

class TrackManager;
class RadarSensor;
class RFDetector;
class CameraSystem;

/**
 * @brief Simulated radar target for radar sensor
 */
struct SimulatedRadarTarget {
    QString id;
    GeoPosition position;
    VelocityVector velocity;
    double rcs = 0.1;           // Radar cross section (m²)
    double trackQuality = 0.9;  // 0-1
    bool isClutter = false;
    qint64 lastUpdateTime = 0;
};

/**
 * @brief Simulated RF emission for RF detector
 */
struct SimulatedRFEmission {
    QString sourceId;
    GeoPosition position;
    double frequencyMHz = 2400.0;
    double signalStrengthDbm = -60.0;
    double bandwidthMHz = 20.0;
    QString protocol;           // "DJI", "FrSky", etc.
    bool isActive = true;
    qint64 lastUpdateTime = 0;
};

/**
 * @brief Simulated visual target for camera
 */
struct SimulatedVisualTarget {
    QString id;
    GeoPosition position;
    VelocityVector velocity;
    double size = 0.5;          // Approximate size in meters
    QString objectClass = "drone";
    double detectability = 0.8; // 0-1
    qint64 lastDetectionTime = 0;
};

/**
 * @brief Radar simulation state
 */
struct RadarSimState {
    bool active = false;
    int detectedTargets = 0;
    int clutterReturns = 0;
    double signalQuality = 1.0;
    QList<SimulatedRadarTarget> currentTargets;
    qint64 lastScanTime = 0;
    double scanAngle = 0.0;     // Current scan angle for rotating radar
};

/**
 * @brief RF Detector simulation state
 */
struct RFDetectorSimState {
    bool active = false;
    int detectedEmissions = 0;
    double noiseFloor = -90.0;  // dBm
    QList<SimulatedRFEmission> currentEmissions;
    QHash<QString, QString> identifiedProtocols;
};

/**
 * @brief Camera simulation state
 */
struct CameraSimState {
    bool active = false;
    int detectedObjects = 0;
    double pan = 0.0;
    double tilt = 0.0;
    double zoom = 1.0;
    QList<SimulatedVisualTarget> visibleTargets;
    QString trackingTargetId;
    bool autoTrackEnabled = false;
};

/**
 * @brief Comprehensive sensor simulator for all sensor types
 */
class SensorSimulator : public QObject {
    Q_OBJECT
    
public:
    explicit SensorSimulator(TrackManager* manager, QObject* parent = nullptr);
    ~SensorSimulator() override;
    
    void setTrackManager(TrackManager* manager);
    TrackManager* trackManager() const { return m_trackManager; }
    
    // Simulation control
    void start();
    void stop();
    bool isRunning() const { return m_running; }
    
    void setUpdateRate(int hz);
    int updateRate() const { return m_updateRateHz; }
    
    // Base position
    void setBasePosition(const GeoPosition& pos);
    GeoPosition basePosition() const { return m_basePosition; }
    
    // Simulation parameters
    void setRealisticMode(bool enable) { m_realisticMode = enable; }
    bool realisticMode() const { return m_realisticMode; }
    
    void setClutterLevel(double level) { m_clutterLevel = qBound(0.0, level, 1.0); }
    double clutterLevel() const { return m_clutterLevel; }
    
    void setNoiseLevel(double level) { m_noiseLevel = qBound(0.0, level, 1.0); }
    double noiseLevel() const { return m_noiseLevel; }
    
    void setDetectionProbability(double prob) { m_detectionProbability = qBound(0.0, prob, 1.0); }
    double detectionProbability() const { return m_detectionProbability; }
    
    // Sensor registration
    void registerRadar(RadarSensor* radar);
    void registerRFDetector(RFDetector* detector);
    void registerCamera(CameraSystem* camera);
    
    void unregisterSensor(const QString& sensorId);
    void clearSensors();
    
    // State access
    RadarSimState radarState(const QString& id) const;
    RFDetectorSimState rfDetectorState(const QString& id) const;
    CameraSimState cameraState(const QString& id) const;
    
    // Target injection (for testing)
    void injectRadarTarget(const SimulatedRadarTarget& target);
    void injectRFEmission(const SimulatedRFEmission& emission);
    void injectVisualTarget(const SimulatedVisualTarget& target);
    
    void clearInjectedTargets();
    
    // Scenario control
    void loadScenario(const QString& scenarioFile);
    void startScenario();
    void stopScenario();
    
    // Statistics
    struct Statistics {
        int totalDetections = 0;
        int radarDetections = 0;
        int rfDetections = 0;
        int cameraDetections = 0;
        int falseAlarms = 0;
        int missedDetections = 0;
    };
    Statistics statistics() const { return m_stats; }
    
signals:
    void radarStateChanged(const QString& id, const RadarSimState& state);
    void rfDetectorStateChanged(const QString& id, const RFDetectorSimState& state);
    void cameraStateChanged(const QString& id, const CameraSimState& state);
    
    void radarDetection(const QString& sensorId, const GeoPosition& pos, 
                       const VelocityVector& vel, double quality);
    void rfDetection(const QString& sensorId, const GeoPosition& pos,
                    double frequencyMHz, double signalDbm, const QString& protocol);
    void cameraDetection(const QString& sensorId, const QRectF& boundingBox,
                        const QString& objectClass, double confidence);
    
    void clutterGenerated(const QString& sensorId, int count);
    void targetLost(const QString& sensorId, const QString& targetId);
    void newTargetDetected(const QString& sensorId, const QString& targetId);
    
    void simulationStarted();
    void simulationStopped();
    
private slots:
    void updateSimulation();
    void generateDetection();
    
private:
    void updateRadarSimulation();
    void updateRFDetectorSimulation();
    void updateCameraSimulation();
    
    void simulateRadarScan(const QString& radarId);
    void simulateRFScan(const QString& detectorId);
    void simulateCameraFrame(const QString& cameraId);
    
    GeoPosition generateRandomTargetPosition(double minRange, double maxRange);
    VelocityVector generateTargetVelocity(const GeoPosition& pos, double speed);
    double calculateDetectionProbability(double range, double maxRange, double rcs);
    
    void generateClutter(const QString& radarId);
    void generateRFNoise(const QString& detectorId);
    
    SimulatedRadarTarget createDroneTarget();
    SimulatedRFEmission createDroneEmission(const GeoPosition& pos);
    
    TrackManager* m_trackManager = nullptr;
    
    QTimer* m_updateTimer;
    QTimer* m_detectionTimer;
    int m_updateRateHz = 10;
    bool m_running = false;
    
    GeoPosition m_basePosition;
    
    // Simulation parameters
    bool m_realisticMode = true;
    double m_clutterLevel = 0.1;    // 0-1 clutter intensity
    double m_noiseLevel = 0.05;     // 0-1 noise level
    double m_detectionProbability = 0.9;
    
    // Registered sensors
    QHash<QString, RadarSensor*> m_radars;
    QHash<QString, RFDetector*> m_rfDetectors;
    QHash<QString, CameraSystem*> m_cameras;
    
    // Simulation states
    QHash<QString, RadarSimState> m_radarStates;
    QHash<QString, RFDetectorSimState> m_rfDetectorStates;
    QHash<QString, CameraSimState> m_cameraStates;
    
    // Injected targets
    QList<SimulatedRadarTarget> m_injectedRadarTargets;
    QList<SimulatedRFEmission> m_injectedRFEmissions;
    QList<SimulatedVisualTarget> m_injectedVisualTargets;
    
    Statistics m_stats;
    int m_nextTargetId = 1;
};

} // namespace CounterUAS

#endif // SENSORSIMULATOR_H
