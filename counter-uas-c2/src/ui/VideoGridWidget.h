#ifndef VIDEOGRIDWIDGET_H
#define VIDEOGRIDWIDGET_H

#include <QWidget>
#include <QGridLayout>
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
    
    // Get widget for a specific camera
    VideoDisplayWidget* widgetForCamera(const QString& cameraId) const;
    
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
    int m_rows = 2;
    int m_cols = 2;
};

} // namespace CounterUAS

#endif // VIDEOGRIDWIDGET_H
