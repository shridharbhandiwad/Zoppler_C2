#include "ui/AlertQueue.h"
#include "core/ThreatAssessor.h"
#include <QVBoxLayout>
#include <QLabel>

namespace CounterUAS {

AlertQueue::AlertQueue(ThreatAssessor* assessor, QWidget* parent)
    : QWidget(parent), m_assessor(assessor) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);
    
    // Panel title with consistent styling
    QLabel* titleLabel = new QLabel("ALERTS", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 11px; color: #00d4ff; "
                             "padding: 6px; background-color: #1a2a35; border-radius: 4px; "
                             "border: 1px solid #2a4a5a; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    m_listWidget = new QListWidget(this);
    m_listWidget->setStyleSheet(
        "QListWidget { "
        "   background-color: #0d1a20; "
        "   border: 1px solid #2a4a5a; "
        "   border-radius: 4px; "
        "   padding: 4px; "
        "   color: #ddd; "
        "} "
        "QListWidget::item { "
        "   padding: 8px; "
        "   margin: 2px 0px; "
        "   border-radius: 3px; "
        "   border-left: 3px solid transparent; "
        "} "
        "QListWidget::item:selected { "
        "   background-color: #1a3a4a; "
        "   border-left: 3px solid #00d4ff; "
        "} "
        "QListWidget::item:hover { "
        "   background-color: #15252f; "
        "} "
    );
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
    
    // Style based on threat level with clearer distinction
    if (alert.threatLevel >= 4) {
        item->setBackground(QColor(80, 20, 20));
        item->setForeground(QColor(255, 120, 120));
    } else if (alert.threatLevel >= 3) {
        item->setBackground(QColor(60, 40, 15));
        item->setForeground(QColor(255, 200, 100));
    } else if (alert.threatLevel >= 2) {
        item->setBackground(QColor(40, 40, 20));
        item->setForeground(QColor(255, 255, 130));
    } else {
        item->setBackground(QColor(20, 35, 40));
        item->setForeground(QColor(150, 200, 220));
    }
    
    m_listWidget->insertItem(0, item);
    
    while (m_listWidget->count() > 50) {
        delete m_listWidget->takeItem(m_listWidget->count() - 1);
    }
}

} // namespace CounterUAS
