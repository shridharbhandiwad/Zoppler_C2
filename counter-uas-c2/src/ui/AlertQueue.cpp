#include "ui/AlertQueue.h"
#include "core/ThreatAssessor.h"
#include <QVBoxLayout>
#include <QLabel>

namespace CounterUAS {

AlertQueue::AlertQueue(ThreatAssessor* assessor, QWidget* parent)
    : QWidget(parent), m_assessor(assessor) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    m_listWidget = new QListWidget(this);
    layout->addWidget(m_listWidget);
    
    if (m_assessor) {
        connect(m_assessor, &ThreatAssessor::newAlert, this, &AlertQueue::onNewAlert);
    }
    
    connect(m_listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        emit alertClicked(item->data(Qt::UserRole).toString());
    });
}

void AlertQueue::onNewAlert(const ThreatAlert& alert) {
    QListWidgetItem* item = new QListWidgetItem(alert.message);
    item->setData(Qt::UserRole, alert.alertId);
    
    if (alert.threatLevel >= 4) {
        item->setBackground(QColor(255, 100, 100));
    } else if (alert.threatLevel >= 3) {
        item->setBackground(QColor(255, 200, 100));
    }
    
    m_listWidget->insertItem(0, item);
    
    while (m_listWidget->count() > 50) {
        delete m_listWidget->takeItem(m_listWidget->count() - 1);
    }
}

} // namespace CounterUAS
