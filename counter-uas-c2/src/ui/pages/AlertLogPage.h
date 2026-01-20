#ifndef ALERTLOGPAGE_H
#define ALERTLOGPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QList>

namespace CounterUAS {

class ThreatAssessor;
struct ThreatAlert;

/**
 * AlertLogItem - Widget displaying a single alert entry
 */
class AlertLogItem : public QFrame {
    Q_OBJECT
    
public:
    explicit AlertLogItem(QWidget* parent = nullptr);
    
    void setAlertId(const QString& id);
    void setTimestamp(const QDateTime& timestamp);
    void setSeverity(const QString& severity);  // INFO, WARNING, CRITICAL
    void setMessage(const QString& message);
    void setRelatedTarget(const QString& targetId);
    void setAcknowledged(bool acknowledged);
    
    QString alertId() const { return m_alertId; }
    bool isAcknowledged() const { return m_acknowledged; }
    
signals:
    void acknowledgeClicked(const QString& alertId);
    void alertClicked(const QString& alertId);
    
private slots:
    void onAckClicked();
    
private:
    void setupUI();
    void updateStyle();
    
    QString m_alertId;
    QString m_severity;
    bool m_acknowledged;
    
    QLabel* m_iconLabel;
    QLabel* m_severityBadge;
    QLabel* m_timestampLabel;
    QLabel* m_messageLabel;
    QLabel* m_targetLabel;
    QPushButton* m_ackButton;
    QLabel* m_ackLabel;
};

/**
 * AlertLogPage - System Alerts / Alert Log view
 * 
 * Shows audit log of all security events and system warnings with:
 * - Severity badge (INFO, CRITICAL, WARNING)
 * - Timestamp
 * - Message
 * - Related target ID
 * - Acknowledge button
 */
class AlertLogPage : public QWidget {
    Q_OBJECT
    
public:
    explicit AlertLogPage(QWidget* parent = nullptr);
    ~AlertLogPage() override = default;
    
    void setThreatAssessor(ThreatAssessor* assessor);
    
public slots:
    void refreshAlerts();
    void addAlert(const QString& id, const QDateTime& timestamp, 
                  const QString& severity, const QString& message,
                  const QString& targetId = QString());
    void clearAlerts();
    
signals:
    void alertAcknowledged(const QString& alertId);
    void alertSelected(const QString& alertId);
    
private slots:
    void onAlertAcknowledged(const QString& alertId);
    void onAlertClicked(const QString& alertId);
    
private:
    void setupUI();
    void addSampleAlerts();
    
    ThreatAssessor* m_threatAssessor;
    QVBoxLayout* m_alertsLayout;
    QScrollArea* m_scrollArea;
    QMap<QString, AlertLogItem*> m_alertItems;
};

} // namespace CounterUAS

#endif // ALERTLOGPAGE_H
