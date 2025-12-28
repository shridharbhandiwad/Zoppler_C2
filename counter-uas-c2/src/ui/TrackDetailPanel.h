#ifndef TRACKDETAILPANEL_H
#define TRACKDETAILPANEL_H

#include <QWidget>
#include <QLabel>
#include "core/Track.h"

namespace CounterUAS {

class TrackDetailPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit TrackDetailPanel(QWidget* parent = nullptr);
    void setTrack(Track* track);
    void clear();
    
signals:
    void engageRequested(const QString& trackId);
    void slewCameraRequested(const QString& trackId);
    
private:
    void updateDisplay();
    
    Track* m_track = nullptr;
    QLabel* m_idLabel;
    QLabel* m_classLabel;
    QLabel* m_posLabel;
    QLabel* m_velLabel;
    QLabel* m_threatLabel;
    QLabel* m_stateLabel;
};

} // namespace CounterUAS

#endif // TRACKDETAILPANEL_H
