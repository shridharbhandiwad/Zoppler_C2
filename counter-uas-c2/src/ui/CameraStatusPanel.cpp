#include "ui/CameraStatusPanel.h"
#include "video/VideoStreamManager.h"
#include <QVBoxLayout>
#include <QHeaderView>

namespace CounterUAS {

CameraStatusPanel::CameraStatusPanel(VideoStreamManager* manager, QWidget* parent)
    : QWidget(parent), m_manager(manager) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    m_table = new QTableWidget(this);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"Camera", "Status", "FPS", "Recording"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->hide();
    
    layout->addWidget(m_table);
}

} // namespace CounterUAS
