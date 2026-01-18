#ifndef VIDEOSIMULATOR_H
#define VIDEOSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QImage>
#include <QHash>
#include <QVector>

namespace CounterUAS {

class SimulationVideoSource;
class VideoStreamManager;

/**
 * @brief Simulates multiple video camera feeds for Counter-UAS simulation
 * 
 * This class manages multiple simulated video sources representing:
 * - Electro-Optical (EO) tracking cameras
 * - Thermal/IR cameras
 * - Radar display
 * 
 * When simulation starts, it creates virtual camera streams and feeds
 * them into the VideoStreamManager for display.
 */
class VideoSimulator : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief Simulation camera definition
     */
    struct SimulatedCamera {
        QString cameraId;
        QString name;
        int scenarioType;  // 0=EO, 1=Thermal, 2=Radar
        bool enabled = true;
    };
    
    explicit VideoSimulator(QObject* parent = nullptr);
    ~VideoSimulator() override;
    
    void setVideoManager(VideoStreamManager* manager);
    VideoStreamManager* videoManager() const { return m_videoManager; }
    
    void start();
    void stop();
    bool isRunning() const { return m_running; }
    
    void setResolution(int width, int height);
    int width() const { return m_width; }
    int height() const { return m_height; }
    
    // Camera configuration
    void addSimulatedCamera(const SimulatedCamera& camera);
    void removeSimulatedCamera(const QString& cameraId);
    void clearSimulatedCameras();
    QList<SimulatedCamera> simulatedCameras() const;
    
    // Default setup
    void setupDefaultCameras();
    
    // Reset tracked target state
    void clearTrackedTargets();
    
    // Legacy frame generation (for direct display without VideoStreamManager)
    void enableLegacyMode(bool enable) { m_legacyMode = enable; }
    bool legacyMode() const { return m_legacyMode; }
    
signals:
    void frameReady(const QImage& frame, qint64 timestamp);
    void cameraFrameReady(const QString& cameraId, const QImage& frame, qint64 timestamp);
    void simulationStarted();
    void simulationStopped();
    
private slots:
    void generateFrame();
    void onSourceFrameReady(const QImage& frame, qint64 timestamp);
    
private:
    void createSimulationSources();
    void destroySimulationSources();
    
    VideoStreamManager* m_videoManager = nullptr;
    QTimer* m_timer;
    
    int m_width = 1280;
    int m_height = 720;
    qint64 m_frameCount = 0;
    bool m_running = false;
    bool m_legacyMode = false;  // When true, uses simple frame generation
    
    QVector<SimulatedCamera> m_cameraConfigs;
    QHash<QString, SimulationVideoSource*> m_sources;
};

} // namespace CounterUAS
#endif
