#ifndef CAMERASTATUSPANEL_H
#define CAMERASTATUSPANEL_H

#include <QWidget>
#include <QTableWidget>

namespace CounterUAS {
class VideoStreamManager;

class CameraStatusPanel : public QWidget {
    Q_OBJECT
public:
    explicit CameraStatusPanel(VideoStreamManager* manager, QWidget* parent = nullptr);
private:
    VideoStreamManager* m_manager;
    QTableWidget* m_table;
};

} // namespace CounterUAS
#endif // CAMERASTATUSPANEL_H
