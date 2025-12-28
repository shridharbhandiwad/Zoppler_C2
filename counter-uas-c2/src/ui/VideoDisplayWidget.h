#ifndef VIDEODISPLAYWIDGET_H
#define VIDEODISPLAYWIDGET_H

#include <QWidget>
#include <QImage>
#include <QTimer>

namespace CounterUAS {

class VideoStreamManager;

class VideoDisplayWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit VideoDisplayWidget(QWidget* parent = nullptr);
    
    void setVideoManager(VideoStreamManager* manager);
    void setSource(const QString& sourceId);
    QString currentSource() const { return m_sourceId; }
    
    void setOverlayEnabled(bool enabled) { m_overlayEnabled = enabled; }
    bool overlayEnabled() const { return m_overlayEnabled; }
    
public slots:
    void updateFrame(const QImage& frame);
    
signals:
    void clicked();
    void doubleClicked();
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    
private:
    VideoStreamManager* m_videoManager = nullptr;
    QString m_sourceId;
    QImage m_currentFrame;
    bool m_overlayEnabled = true;
    QTimer* m_updateTimer;
};

} // namespace CounterUAS

#endif // VIDEODISPLAYWIDGET_H
