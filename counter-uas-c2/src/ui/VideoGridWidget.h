#ifndef VIDEOGRIDWIDGET_H
#define VIDEOGRIDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QList>
#include <QHash>
#include <QImage>
#include "ui/VideoDisplayWidget.h"

namespace CounterUAS {

class VideoGridWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit VideoGridWidget(QWidget* parent = nullptr);
    
    void setGridSize(int rows, int cols);
    void addCamera(const QString& cameraId);
    void removeCamera(const QString& cameraId);
    void clearAllCameras();
    
    // Simplified Day/Night camera layout
    void setupDayNightLayout();
    
    // Get widget for a specific camera
    VideoDisplayWidget* widgetForCamera(const QString& cameraId) const;
    
    // Get Day/Night camera widgets directly
    VideoDisplayWidget* dayCameraWidget() const { return m_dayWidget; }
    VideoDisplayWidget* nightCameraWidget() const { return m_nightWidget; }
    
    // Get list of all camera IDs
    QStringList cameraIds() const;
    
public slots:
    // Update frame for a specific camera
    void updateFrame(const QString& cameraId, const QImage& frame);
    
signals:
    void cameraSelected(const QString& cameraId);
    
private:
    QGridLayout* m_layout;
    QList<VideoDisplayWidget*> m_widgets;
    QHash<QString, VideoDisplayWidget*> m_cameraWidgetMap;
    int m_rows = 1;
    int m_cols = 2;
    
    // Day/Night camera widgets
    VideoDisplayWidget* m_dayWidget = nullptr;
    VideoDisplayWidget* m_nightWidget = nullptr;
    QLabel* m_dayLabel = nullptr;
    QLabel* m_nightLabel = nullptr;
};

} // namespace CounterUAS

#endif // VIDEOGRIDWIDGET_H
