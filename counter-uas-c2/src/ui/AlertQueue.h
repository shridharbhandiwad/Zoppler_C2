#ifndef ALERTQUEUE_H
#define ALERTQUEUE_H

#include <QWidget>
#include <QListWidget>

namespace CounterUAS {
class ThreatAssessor;

class AlertQueue : public QWidget {
    Q_OBJECT
public:
    explicit AlertQueue(ThreatAssessor* assessor, QWidget* parent = nullptr);
    
signals:
    void alertClicked(const QString& alertId);
    
private slots:
    void onNewAlert(const struct ThreatAlert& alert);
    
private:
    ThreatAssessor* m_assessor;
    QListWidget* m_listWidget;
};

} // namespace CounterUAS
#endif // ALERTQUEUE_H
