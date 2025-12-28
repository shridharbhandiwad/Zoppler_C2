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
    for (auto* w : m_widgets) {
        if (w->currentSource().isEmpty()) {
            w->setSource(cameraId);
            return;
        }
    }
}

void VideoGridWidget::removeCamera(const QString& cameraId) {
    for (auto* w : m_widgets) {
        if (w->currentSource() == cameraId) {
            w->setSource(QString());
            return;
        }
    }
}

} // namespace CounterUAS
