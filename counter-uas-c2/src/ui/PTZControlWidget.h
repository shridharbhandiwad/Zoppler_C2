#ifndef PTZCONTROLWIDGET_H
#define PTZCONTROLWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>

namespace CounterUAS {

class PTZControlWidget : public QWidget {
    Q_OBJECT
public:
    explicit PTZControlWidget(QWidget* parent = nullptr);
signals:
    void panLeft();
    void panRight();
    void tiltUp();
    void tiltDown();
    void zoomIn();
    void zoomOut();
    void goHome();
private:
    QPushButton* m_upBtn;
    QPushButton* m_downBtn;
    QPushButton* m_leftBtn;
    QPushButton* m_rightBtn;
    QSlider* m_zoomSlider;
};

} // namespace CounterUAS
#endif
