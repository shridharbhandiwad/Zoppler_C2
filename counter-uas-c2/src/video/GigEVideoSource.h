#ifndef GIGEVIDEOSOURCE_H
#define GIGEVIDEOSOURCE_H

#include "video/VideoSource.h"
#include <QThread>

namespace CounterUAS {

/**
 * @brief GigE Vision camera configuration
 */
struct GigEConfig {
    QString deviceId;
    int packetSize = 8192;
    int frameBufferCount = 3;
    double exposureMs = 10.0;
    double gainDb = 0.0;
    bool autoExposure = true;
    bool autoGain = true;
    int pixelFormat = 0;  // 0 = mono8, 1 = rgb8, etc.
};

/**
 * @brief GigE Vision camera video source
 * Note: Full implementation requires GigE Vision SDK (e.g., Aravis, Vimba)
 */
class GigEVideoSource : public VideoSource {
    Q_OBJECT
    
public:
    explicit GigEVideoSource(const QString& sourceId, QObject* parent = nullptr);
    ~GigEVideoSource() override;
    
    QString sourceType() const override { return "GigE"; }
    
    // VideoSource implementation
    bool open(const QUrl& url) override;
    void close() override;
    bool isOpen() const override;
    
    // GigE-specific configuration
    void setConfig(const GigEConfig& config);
    GigEConfig config() const { return m_config; }
    
    // Camera control
    void setExposure(double ms);
    void setGain(double db);
    void setAutoExposure(bool enable);
    void setAutoGain(bool enable);
    
    // Device enumeration
    static QStringList availableDevices();
    
protected slots:
    void processFrame() override;
    
private:
    void initializeCamera();
    void captureLoop();
    
    GigEConfig m_config;
    bool m_isOpen = false;
    
    // Placeholder for actual GigE SDK objects
    void* m_cameraHandle = nullptr;
    QThread* m_captureThread = nullptr;
    bool m_stopCapture = false;
};

} // namespace CounterUAS

#endif // GIGEVIDEOSOURCE_H
