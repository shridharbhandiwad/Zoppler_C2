#ifndef VIDEOSIMULATOR_H
#define VIDEOSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QImage>

namespace CounterUAS {

class VideoSimulator : public QObject {
    Q_OBJECT
public:
    explicit VideoSimulator(QObject* parent = nullptr);
    void start();
    void stop();
    void setResolution(int width, int height);
    
signals:
    void frameReady(const QImage& frame, qint64 timestamp);
    
private slots:
    void generateFrame();
    
private:
    QTimer* m_timer;
    int m_width = 1280;
    int m_height = 720;
    qint64 m_frameCount = 0;
};

} // namespace CounterUAS
#endif
