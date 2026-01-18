/**
 * @file EffectorStatusDialog.h
 * @brief Dialog for viewing and controlling effectors
 */

#ifndef EFFECTORSTATUSDIALOG_H
#define EFFECTORSTATUSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "effectors/EffectorInterface.h"

namespace CounterUAS {

class EngagementManager;

/**
 * @brief Dialog for viewing effector status and control
 */
class EffectorStatusDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit EffectorStatusDialog(EngagementManager* manager, QWidget* parent = nullptr);
    
signals:
    void effectorInitializeRequested(const QString& effectorId);
    void effectorShutdownRequested(const QString& effectorId);
    void effectorResetRequested(const QString& effectorId);
    
private slots:
    void refreshStatus();
    void onEffectorSelected(int row);
    void onInitialize();
    void onShutdown();
    void onReset();
    void onTestEngage();
    
private:
    void setupUI();
    QString statusToString(EffectorStatus status);
    QColor statusToColor(EffectorStatus status);
    
    EngagementManager* m_manager;
    
    QTableWidget* m_effectorTable;
    
    // Detail panel
    QLabel* m_detailId;
    QLabel* m_detailType;
    QLabel* m_detailStatus;
    QLabel* m_detailReadiness;
    QProgressBar* m_readinessBar;
    QLabel* m_detailEngagements;
    QLabel* m_detailLastEngagement;
    QLabel* m_detailRoundsRemaining;
    
    // Control buttons
    QPushButton* m_initBtn;
    QPushButton* m_shutdownBtn;
    QPushButton* m_resetBtn;
    QPushButton* m_testBtn;
    
    QTimer* m_refreshTimer;
    QString m_selectedEffectorId;
};

} // namespace CounterUAS

#endif // EFFECTORSTATUSDIALOG_H
