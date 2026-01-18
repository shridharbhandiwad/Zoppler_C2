#include "ui/VideoGridWidget.h"

namespace CounterUAS {

VideoGridWidget::VideoGridWidget(QWidget* parent) : QWidget(parent) {
    m_layout = new QGridLayout(this);
    m_layout->setSpacing(2);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setGridSize(2, 2);
}

void VideoGridWidget::setGridSize(int rows, int cols) {
    m_rows = rows;
    m_cols = cols;
    
    // Clear existing
    for (auto* w : m_widgets) {
        m_layout->removeWidget(w);
        delete w;
    }
    m_widgets.clear();
    m_cameraWidgetMap.clear();
    
    // Create new grid
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            VideoDisplayWidget* widget = new VideoDisplayWidget(this);
            widget->setMinimumSize(160, 120);
            connect(widget, &VideoDisplayWidget::clicked, this, [this, widget]() {
                emit cameraSelected(widget->currentSource());
            });
            m_layout->addWidget(widget, r, c);
            m_widgets.append(widget);
        }
    }
}

void VideoGridWidget::addCamera(const QString& cameraId) {
    // Check if camera already assigned
    if (m_cameraWidgetMap.contains(cameraId)) {
        return;
    }
    
    // Find first available widget
    for (auto* w : m_widgets) {
        if (w->currentSource().isEmpty()) {
            w->setSource(cameraId);
            m_cameraWidgetMap[cameraId] = w;
            return;
        }
    }
}

void VideoGridWidget::removeCamera(const QString& cameraId) {
    if (m_cameraWidgetMap.contains(cameraId)) {
        VideoDisplayWidget* w = m_cameraWidgetMap.take(cameraId);
        if (w) {
            w->setSource(QString());
        }
    }
}

void VideoGridWidget::clearAllCameras() {
    m_cameraWidgetMap.clear();
    for (auto* w : m_widgets) {
        w->setSource(QString());
    }
}

VideoDisplayWidget* VideoGridWidget::widgetForCamera(const QString& cameraId) const {
    return m_cameraWidgetMap.value(cameraId, nullptr);
}

QStringList VideoGridWidget::cameraIds() const {
    return m_cameraWidgetMap.keys();
}

void VideoGridWidget::updateFrame(const QString& cameraId, const QImage& frame) {
    // If camera is not yet assigned, try to add it
    if (!m_cameraWidgetMap.contains(cameraId)) {
        // Find first available widget
        for (auto* w : m_widgets) {
            if (w->currentSource().isEmpty()) {
                w->setSource(cameraId);
                m_cameraWidgetMap[cameraId] = w;
                break;
            }
        }
    }
    
    // Update the frame
    VideoDisplayWidget* widget = m_cameraWidgetMap.value(cameraId, nullptr);
    if (widget) {
        widget->updateFrame(frame);
    }
}

} // namespace CounterUAS
