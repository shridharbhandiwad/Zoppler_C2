#include "ui/VideoGridWidget.h"
#include <QLabel>

namespace CounterUAS {

VideoGridWidget::VideoGridWidget(QWidget* parent) : QWidget(parent) {
    m_layout = new QGridLayout(this);
    m_layout->setSpacing(4);
    m_layout->setContentsMargins(2, 2, 2, 2);
    
    // Create simplified 2-camera layout (Day and Night)
    setupDayNightLayout();
}

void VideoGridWidget::setupDayNightLayout() {
    // Clear existing layout
    for (auto* w : m_widgets) {
        m_layout->removeWidget(w);
        delete w;
    }
    m_widgets.clear();
    m_cameraWidgetMap.clear();
    
    // Delete old labels if they exist
    if (m_dayLabel) { delete m_dayLabel; m_dayLabel = nullptr; }
    if (m_nightLabel) { delete m_nightLabel; m_nightLabel = nullptr; }
    
    m_rows = 1;
    m_cols = 2;
    
    // Style for camera labels
    QString labelStyle = "QLabel { color: white; background-color: rgba(0, 0, 0, 180); "
                        "padding: 4px 8px; font-weight: bold; font-size: 11px; "
                        "border-radius: 3px; }";
    
    // Create Day Camera container (left side)
    QWidget* dayContainer = new QWidget(this);
    dayContainer->setStyleSheet("background-color: #1a1a1a; border: 1px solid #333;");
    QVBoxLayout* dayLayout = new QVBoxLayout(dayContainer);
    dayLayout->setContentsMargins(2, 2, 2, 2);
    dayLayout->setSpacing(2);
    
    m_dayLabel = new QLabel("DAY CAMERA", dayContainer);
    m_dayLabel->setStyleSheet(labelStyle + "background-color: rgba(30, 100, 180, 200);");
    m_dayLabel->setAlignment(Qt::AlignCenter);
    dayLayout->addWidget(m_dayLabel);
    
    m_dayWidget = new VideoDisplayWidget(dayContainer);
    m_dayWidget->setMinimumSize(240, 180);
    m_dayWidget->setSource("SIM-DAY-001");
    connect(m_dayWidget, &VideoDisplayWidget::clicked, this, [this]() {
        emit cameraSelected(m_dayWidget->currentSource());
    });
    dayLayout->addWidget(m_dayWidget, 1);
    
    // Create Night Camera container (right side)
    QWidget* nightContainer = new QWidget(this);
    nightContainer->setStyleSheet("background-color: #1a1a1a; border: 1px solid #333;");
    QVBoxLayout* nightLayout = new QVBoxLayout(nightContainer);
    nightLayout->setContentsMargins(2, 2, 2, 2);
    nightLayout->setSpacing(2);
    
    m_nightLabel = new QLabel("NIGHT CAMERA", nightContainer);
    m_nightLabel->setStyleSheet(labelStyle + "background-color: rgba(100, 30, 100, 200);");
    m_nightLabel->setAlignment(Qt::AlignCenter);
    nightLayout->addWidget(m_nightLabel);
    
    m_nightWidget = new VideoDisplayWidget(nightContainer);
    m_nightWidget->setMinimumSize(240, 180);
    m_nightWidget->setSource("SIM-NIGHT-001");
    connect(m_nightWidget, &VideoDisplayWidget::clicked, this, [this]() {
        emit cameraSelected(m_nightWidget->currentSource());
    });
    nightLayout->addWidget(m_nightWidget, 1);
    
    // Add containers to main grid
    m_layout->addWidget(dayContainer, 0, 0);
    m_layout->addWidget(nightContainer, 0, 1);
    m_layout->setColumnStretch(0, 1);
    m_layout->setColumnStretch(1, 1);
    
    // Add to widget list for tracking
    m_widgets.append(m_dayWidget);
    m_widgets.append(m_nightWidget);
    
    // Pre-register the camera mappings
    m_cameraWidgetMap["SIM-DAY-001"] = m_dayWidget;
    m_cameraWidgetMap["SIM-NIGHT-001"] = m_nightWidget;
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
