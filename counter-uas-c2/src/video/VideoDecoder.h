#ifndef VIDEODECODER_H
#define VIDEODECODER_H

#include <QObject>
#include <QImage>
#include <QThread>

namespace CounterUAS {

/**
 * @brief Video decoder for custom frame processing
 */
class VideoDecoder : public QObject {
    Q_OBJECT
    
public:
    explicit VideoDecoder(QObject* parent = nullptr);
    ~VideoDecoder() override;
    
    bool initialize(const QString& codecName);
    void shutdown();
    
    void setHardwareAcceleration(bool enable);
    bool hardwareAcceleration() const { return m_hwAccel; }
    
    QImage decode(const QByteArray& encodedData);
    
signals:
    void frameDecoded(const QImage& frame);
    void error(const QString& message);
    
private:
    bool m_initialized = false;
    bool m_hwAccel = false;
    QString m_codecName;
};

} // namespace CounterUAS

#endif // VIDEODECODER_H
