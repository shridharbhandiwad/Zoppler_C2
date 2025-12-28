#include "ui/VideoDisplayWidget.h"
#include "video/VideoStreamManager.h"
#include <QPainter>
#include <QMouseEvent>

namespace CounterUAS {

VideoDisplayWidget::VideoDisplayWidget(QWidget* parent)
    : QWidget(parent)
    , m_updateTimer(new QTimer(this))
{
    setMinimumSize(320, 240);
    setStyleSheet("background-color: black;");
    
    m_updateTimer->setInterval(33);  // ~30 fps
    connect(m_updateTimer, &QTimer::timeout, this, [this]() {
        update();
    });
}

void VideoDisplayWidget::setVideoManager(VideoStreamManager* manager) {
    m_videoManager = manager;
}

void VideoDisplayWidget::setSource(const QString& sourceId) {
    m_sourceId = sourceId;
    m_updateTimer->start();
}

void VideoDisplayWidget::updateFrame(const QImage& frame) {
    m_currentFrame = frame;
    update();
}

void VideoDisplayWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    if (!m_currentFrame.isNull()) {
        QImage scaled = m_currentFrame.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int x = (width() - scaled.width()) / 2;
        int y = (height() - scaled.height()) / 2;
        painter.drawImage(x, y, scaled);
    } else {
        painter.fillRect(rect(), Qt::black);
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, m_sourceId.isEmpty() ? "No Video Source" : m_sourceId);
    }
    
    // Draw overlay
    if (m_overlayEnabled) {
        painter.setPen(Qt::white);
        painter.drawText(10, 20, m_sourceId);
        painter.drawText(10, height() - 10, QDateTime::currentDateTime().toString("hh:mm:ss"));
    }
}

void VideoDisplayWidget::mouseDoubleClickEvent(QMouseEvent* event) {
    Q_UNUSED(event)
    emit doubleClicked();
}

void VideoDisplayWidget::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event)
    emit clicked();
}

} // namespace CounterUAS
