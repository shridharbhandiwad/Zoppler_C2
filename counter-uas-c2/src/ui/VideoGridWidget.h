#ifndef VIDEOGRIDWIDGET_H
#define VIDEOGRIDWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QList>
#include "ui/VideoDisplayWidget.h"

namespace CounterUAS {

class VideoGridWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit VideoGridWidget(QWidget* parent = nullptr);
    
    void setGridSize(int rows, int cols);
    void addCamera(const QString& cameraId);
    void removeCamera(const QString& cameraId);
    
signals:
    void cameraSelected(const QString& cameraId);
    
private:
    QGridLayout* m_layout;
    QList<VideoDisplayWidget*> m_widgets;
    int m_rows = 2;
    int m_cols = 2;
};

} // namespace CounterUAS

#endif // VIDEOGRIDWIDGET_H
