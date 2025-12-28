#include "ui/SensorStatusPanel.h"
#include <QVBoxLayout>
#include <QHeaderView>

namespace CounterUAS {

SensorStatusPanel::SensorStatusPanel(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    m_table = new QTableWidget(this);
    m_table->setColumnCount(3);
    m_table->setHorizontalHeaderLabels({"Sensor", "Type", "Status"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->hide();
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addWidget(m_table);
}

void SensorStatusPanel::addSensor(const QString& id, const QString& name, const QString& type) {
    int row = m_table->rowCount();
    m_table->insertRow(row);
    m_table->setItem(row, 0, new QTableWidgetItem(name));
    m_table->setItem(row, 1, new QTableWidgetItem(type));
    m_table->setItem(row, 2, new QTableWidgetItem("OFFLINE"));
    m_table->item(row, 0)->setData(Qt::UserRole, id);
}

void SensorStatusPanel::updateSensorStatus(const QString& id, const QString& status) {
    for (int i = 0; i < m_table->rowCount(); ++i) {
        if (m_table->item(i, 0)->data(Qt::UserRole).toString() == id) {
            m_table->item(i, 2)->setText(status);
            return;
        }
    }
}

} // namespace CounterUAS
