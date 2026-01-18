#ifndef EFFECTORCONTROLPANEL_H
#define EFFECTORCONTROLPANEL_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QProgressBar>
#include "effectors/EffectorInterface.h"

namespace CounterUAS {
class EngagementManager;

class EffectorControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit EffectorControlPanel(EngagementManager* manager, QWidget* parent = nullptr);
    
    void refreshEffectorList();
    
signals:
    void engageRequested(const QString& trackId);
    void effectorSelected(const QString& effectorId);
    
private slots:
    void onEffectorItemClicked(QListWidgetItem* item);
    void onEngageClicked();
    void onDisengageClicked();
    void updateEffectorStatus();
    
private:
    void setupUI();
    void updateEffectorItem(QListWidgetItem* item, EffectorInterface* effector);
    QColor statusToColor(EffectorStatus status);
    QString statusToString(EffectorStatus status);
    
    EngagementManager* m_manager;
    QListWidget* m_effectorList;
    QPushButton* m_engageBtn;
    QPushButton* m_disengageBtn;
    
    // Status display
    QLabel* m_selectedEffectorLabel;
    QLabel* m_statusLabel;
    QProgressBar* m_readinessBar;
    QLabel* m_roundsLabel;
    
    QTimer* m_updateTimer;
    QString m_selectedEffectorId;
};

} // namespace CounterUAS
#endif // EFFECTORCONTROLPANEL_H
