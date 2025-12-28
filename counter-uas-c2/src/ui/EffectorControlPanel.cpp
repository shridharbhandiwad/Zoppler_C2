#include "ui/EffectorControlPanel.h"
#include "core/EngagementManager.h"
#include <QVBoxLayout>
#include <QLabel>

namespace CounterUAS {

EffectorControlPanel::EffectorControlPanel(EngagementManager* manager, QWidget* parent)
    : QWidget(parent), m_manager(manager) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    layout->addWidget(new QLabel("Effectors:"));
    
    m_effectorList = new QListWidget(this);
    layout->addWidget(m_effectorList);
    
    m_engageBtn = new QPushButton("ENGAGE", this);
    m_engageBtn->setStyleSheet("background-color: red; color: white; font-weight: bold;");
    m_engageBtn->setMinimumHeight(40);
    layout->addWidget(m_engageBtn);
    
    connect(m_engageBtn, &QPushButton::clicked, this, [this]() {
        if (m_manager && !m_manager->selectedTrackId().isEmpty()) {
            m_manager->requestAuthorization();
        }
    });
}

} // namespace CounterUAS
