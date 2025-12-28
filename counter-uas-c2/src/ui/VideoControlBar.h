#ifndef VIDEOCONTROLBAR_H
#define VIDEOCONTROLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

namespace CounterUAS {

class VideoControlBar : public QWidget {
    Q_OBJECT
public:
    explicit VideoControlBar(QWidget* parent = nullptr);
signals:
    void recordToggled(bool recording);
    void snapshotRequested();
    void cameraSelected(const QString& cameraId);
private:
    QPushButton* m_recordBtn;
    QPushButton* m_snapshotBtn;
    QComboBox* m_cameraCombo;
};

} // namespace CounterUAS
#endif
