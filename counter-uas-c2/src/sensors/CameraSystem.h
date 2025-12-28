#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include "sensors/SensorInterface.h"
#include <QImage>
#include <QRectF>

namespace CounterUAS {

/**
 * @brief Camera detection result from video analytics
 */
struct CameraDetection {
    QString cameraId;
    QRectF boundingBox;      // Normalized 0-1 coordinates
    double confidence;
    QString objectClass;      // e.g., "drone", "bird", "aircraft"
    qint64 frameNumber;
    qint64 timestamp;
    QImage thumbnail;
};

/**
 * @brief Camera PTZ state
 */
struct CameraPTZState {
    double pan = 0.0;        // degrees, 0 = north
    double tilt = 0.0;       // degrees, 0 = horizon
    double zoom = 1.0;       // 1.0 = no zoom
    double hfov = 60.0;      // horizontal field of view at current zoom
    double vfov = 45.0;      // vertical field of view at current zoom
};

/**
 * @brief Camera system configuration
 */
struct CameraSystemConfig {
    QString streamUrl;       // RTSP URL or device path
    QString cameraId;
    QString cameraName;
    GeoPosition mountPosition;
    CameraPTZState initialPTZ;
    
    bool hasPTZ = false;
    double panMin = -180.0;
    double panMax = 180.0;
    double tiltMin = -45.0;
    double tiltMax = 90.0;
    double zoomMin = 1.0;
    double zoomMax = 30.0;
    
    bool enableDetection = false;
    double detectionConfidenceThreshold = 0.5;
    int detectionFrameSkip = 5;  // Process every Nth frame
};

/**
 * @brief Camera System sensor for visual detection and tracking
 */
class CameraSystem : public SensorInterface {
    Q_OBJECT
    
public:
    explicit CameraSystem(const QString& sensorId, QObject* parent = nullptr);
    ~CameraSystem() override;
    
    // SensorInterface implementation
    QString sensorType() const override { return "CAMERA"; }
    DetectionSource detectionSource() const override { return DetectionSource::Camera; }
    
    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;
    
    double maxRange() const override { return 2000.0; }  // Visual detection range
    double fieldOfView() const override { return m_ptzState.hfov; }
    double azimuth() const override { return m_ptzState.pan; }
    
    // Configuration
    void setConfig(const CameraSystemConfig& config);
    CameraSystemConfig config() const { return m_config; }
    
    // PTZ Control
    CameraPTZState ptzState() const { return m_ptzState; }
    void setPan(double degrees);
    void setTilt(double degrees);
    void setZoom(double level);
    void setPTZ(double pan, double tilt, double zoom);
    void goToPreset(int presetNumber);
    void savePreset(int presetNumber);
    
    // Slew to position
    void slewToPosition(const GeoPosition& target);
    void slewToTrack(const QString& trackId);
    bool isSlewingComplete() const { return m_slewComplete; }
    
    // Frame access
    QImage currentFrame() const { return m_currentFrame; }
    qint64 currentFrameNumber() const { return m_frameNumber; }
    
    // Detection callback (for external detector integration)
    void reportDetection(const CameraDetection& detection);
    
    // Video recording notification
    bool isRecording() const { return m_recording; }
    void setRecording(bool recording);
    
signals:
    void frameReady(const QImage& frame, qint64 frameNumber);
    void cameraDetection(const CameraDetection& detection);
    void ptzChanged(const CameraPTZState& state);
    void slewComplete(const GeoPosition& target);
    void recordingChanged(bool recording);
    void streamError(const QString& error);
    
protected slots:
    void processData() override;
    
private slots:
    void onFrameReceived(const QImage& frame);
    void updateSlewProgress();
    
private:
    GeoPosition calculateTargetPosition(const QRectF& bbox);
    QPair<double, double> calculatePanTilt(const GeoPosition& target);
    void executePTZCommand();
    
    CameraSystemConfig m_config;
    CameraPTZState m_ptzState;
    CameraPTZState m_targetPTZ;
    
    QImage m_currentFrame;
    qint64 m_frameNumber = 0;
    bool m_connected = false;
    bool m_recording = false;
    
    bool m_slewInProgress = false;
    bool m_slewComplete = true;
    GeoPosition m_slewTarget;
    QTimer* m_slewTimer;
    
    QHash<int, CameraPTZState> m_presets;
};

} // namespace CounterUAS

#endif // CAMERASYSTEM_H
