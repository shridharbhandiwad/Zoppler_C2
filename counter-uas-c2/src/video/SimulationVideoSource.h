#ifndef SIMULATIONVIDEOSOURCE_H
#define SIMULATIONVIDEOSOURCE_H

#include "video/VideoSource.h"
#include <QDir>
#include <QStringList>

namespace CounterUAS {

/**
 * @brief Video source for simulation that can play image sequences or generate frames
 * 
 * This source supports:
 * - Loading and playing image sequences from a directory
 * - Loading images from Qt resources
 * - Procedurally generated frames for UAS tracking simulation
 * - Customizable overlays and telemetry display
 */
class SimulationVideoSource : public VideoSource {
    Q_OBJECT
    
public:
    enum class SimulationMode {
        Generated,      // Procedurally generate frames
        ImageSequence,  // Play images from directory
        Resources       // Play images from Qt resources
    };
    
    explicit SimulationVideoSource(const QString& sourceId, QObject* parent = nullptr);
    ~SimulationVideoSource() override;
    
    QString sourceType() const override { return "SIMULATION"; }
    
    // VideoSource implementation
    bool open(const QUrl& url) override;
    void close() override;
    bool isOpen() const override;
    
    // Simulation configuration
    void setSimulationMode(SimulationMode mode) { m_mode = mode; }
    SimulationMode simulationMode() const { return m_mode; }
    
    // Image sequence configuration
    bool loadImageSequence(const QString& directoryPath);
    bool loadFromResources(const QString& resourcePrefix);
    void setLooping(bool loop) { m_looping = loop; }
    bool isLooping() const { return m_looping; }
    
    // Generated frame configuration
    void setScenarioType(int type) { m_scenarioType = type; }
    int scenarioType() const { return m_scenarioType; }
    
    void setCameraName(const QString& name) { m_cameraName = name; }
    QString cameraName() const { return m_cameraName; }
    
    void setShowOverlay(bool show) { m_showOverlay = show; }
    bool showOverlay() const { return m_showOverlay; }
    
    // Target simulation
    void setTargetPosition(const QPointF& pos) { m_targetPos = pos; }
    QPointF targetPosition() const { return m_targetPos; }
    
    void setTargetVisible(bool visible) { m_targetVisible = visible; }
    bool targetVisible() const { return m_targetVisible; }
    
protected slots:
    void processFrame() override;
    
private:
    QImage generateFrame();
    QImage generateUASTrackingFrame();
    QImage generateThermalFrame();
    QImage generateRadarFrame();
    void drawOverlay(QImage& frame);
    void drawTarget(QPainter& painter, int x, int y);
    void drawCrosshairs(QPainter& painter, int cx, int cy);
    void drawTelemetry(QPainter& painter, const QSize& size);
    
    SimulationMode m_mode = SimulationMode::Generated;
    bool m_isOpen = false;
    bool m_looping = true;
    
    // Image sequence
    QStringList m_imageFiles;
    int m_currentImageIndex = 0;
    QString m_imagePath;
    
    // Generated frame state
    int m_scenarioType = 0;  // 0=EO, 1=Thermal, 2=Radar
    QString m_cameraName = "SIM-CAM-001";
    bool m_showOverlay = true;
    qint64 m_frameCount = 0;
    
    // Target simulation
    QPointF m_targetPos{0.5, 0.5};  // Normalized position
    bool m_targetVisible = true;
    double m_targetPhase = 0.0;
};

} // namespace CounterUAS

#endif // SIMULATIONVIDEOSOURCE_H
