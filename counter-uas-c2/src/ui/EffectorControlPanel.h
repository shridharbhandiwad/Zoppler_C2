#ifndef EFFECTORCONTROLPANEL_H
#define EFFECTORCONTROLPANEL_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

namespace CounterUAS {
class EngagementManager;

class EffectorControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit EffectorControlPanel(EngagementManager* manager, QWidget* parent = nullptr);
    
signals:
    void engageRequested(const QString& trackId);
    
private:
    EngagementManager* m_manager;
    QListWidget* m_effectorList;
    QPushButton* m_engageBtn;
};

} // namespace CounterUAS
#endif // EFFECTORCONTROLPANEL_H
