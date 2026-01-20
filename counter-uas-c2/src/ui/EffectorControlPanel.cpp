#include "ui/EffectorControlPanel.h"
#include "core/EngagementManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QLabel>

namespace CounterUAS {

EffectorControlPanel::EffectorControlPanel(EngagementManager* manager, QWidget* parent)
    : QWidget(parent)
    , m_manager(manager)
    , m_updateTimer(new QTimer(this))
{
    setupUI();
    
    connect(m_updateTimer, &QTimer::timeout, this, &EffectorControlPanel::updateEffectorStatus);
    m_updateTimer->start(500);  // Update every 500ms
    
    // Initial population
    refreshEffectorList();
}

void EffectorControlPanel::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(8);
    
    // Panel title with consistent styling
    QLabel* titleLabel = new QLabel("EFFECTOR CONTROL", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 11px; color: #00d4ff; "
                             "padding: 6px; background-color: #1a2a35; border-radius: 4px; "
                             "border: 1px solid #2a4a5a; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);
    
    // Effector list
    QGroupBox* listGroup = new QGroupBox("Available Effectors", this);
    listGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #2a4a5a; border-radius: 6px; "
        "background-color: #0d1a20; margin-top: 10px; padding: 12px 8px 8px 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 2px 8px; "
        "color: #00d4ff; background-color: #0d1a20; }");
    QVBoxLayout* listLayout = new QVBoxLayout(listGroup);
    listLayout->setContentsMargins(10, 14, 10, 10);
    
    m_effectorList = new QListWidget(this);
    m_effectorList->setMinimumHeight(150);
    m_effectorList->setStyleSheet(
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
        "} "
        "QListWidget::item:selected { "
        "   background-color: #1a3a4a; "
        "   border-left: 3px solid #00d4ff; "
        "} "
        "QListWidget::item:hover { "
        "   background-color: #15252f; "
        "} "
    );
    connect(m_effectorList, &QListWidget::itemClicked, 
            this, &EffectorControlPanel::onEffectorItemClicked);
    listLayout->addWidget(m_effectorList);
    
    layout->addWidget(listGroup);
    
    // Status display
    QGroupBox* statusGroup = new QGroupBox("Selected Effector", this);
    statusGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #2a4a5a; border-radius: 6px; "
        "background-color: #0d1a20; margin-top: 10px; padding: 12px 8px 8px 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 2px 8px; "
        "color: #00d4ff; background-color: #0d1a20; }");
    QVBoxLayout* statusLayout = new QVBoxLayout(statusGroup);
    statusLayout->setContentsMargins(10, 14, 10, 10);
    
    m_selectedEffectorLabel = new QLabel("None selected", this);
    m_selectedEffectorLabel->setStyleSheet("font-weight: bold;");
    statusLayout->addWidget(m_selectedEffectorLabel);
    
    m_statusLabel = new QLabel("Status: -", this);
    statusLayout->addWidget(m_statusLabel);
    
    QHBoxLayout* readinessLayout = new QHBoxLayout();
    readinessLayout->addWidget(new QLabel("Readiness:", this));
    m_readinessBar = new QProgressBar(this);
    m_readinessBar->setRange(0, 100);
    m_readinessBar->setValue(0);
    readinessLayout->addWidget(m_readinessBar);
    statusLayout->addLayout(readinessLayout);
    
    m_roundsLabel = new QLabel("Rounds: N/A", this);
    statusLayout->addWidget(m_roundsLabel);
    
    layout->addWidget(statusGroup);
    
    // Control buttons
    QGroupBox* controlGroup = new QGroupBox("Engagement Control", this);
    controlGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #4a2a2a; border-radius: 6px; "
        "background-color: #0d1a20; margin-top: 10px; padding: 12px 8px 8px 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 2px 8px; "
        "color: #ff6666; background-color: #0d1a20; }");
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);
    controlLayout->setContentsMargins(10, 14, 10, 10);
    controlLayout->setSpacing(8);
    
    m_engageBtn = new QPushButton("ENGAGE TARGET", this);
    m_engageBtn->setStyleSheet(
        "QPushButton { background-color: #8a2222; color: white; font-weight: bold; font-size: 14px; "
        "padding: 12px; border-radius: 6px; border: 1px solid #aa3333; }"
        "QPushButton:disabled { background-color: #333; color: #666; border: 1px solid #444; }"
        "QPushButton:hover:enabled { background-color: #aa3333; }");
    m_engageBtn->setMinimumHeight(50);
    m_engageBtn->setEnabled(false);
    connect(m_engageBtn, &QPushButton::clicked, this, &EffectorControlPanel::onEngageClicked);
    controlLayout->addWidget(m_engageBtn);
    
    m_disengageBtn = new QPushButton("DISENGAGE", this);
    m_disengageBtn->setStyleSheet(
        "QPushButton { background-color: #6a5a2a; color: white; font-weight: bold; "
        "padding: 8px; border-radius: 4px; border: 1px solid #8a7a3a; }"
        "QPushButton:disabled { background-color: #333; color: #666; border: 1px solid #444; }"
        "QPushButton:hover:enabled { background-color: #8a7a3a; }");
    m_disengageBtn->setMinimumHeight(35);
    m_disengageBtn->setEnabled(false);
    connect(m_disengageBtn, &QPushButton::clicked, this, &EffectorControlPanel::onDisengageClicked);
    controlLayout->addWidget(m_disengageBtn);
    
    layout->addWidget(controlGroup);
    
    layout->addStretch();
}

void EffectorControlPanel::refreshEffectorList() {
    m_effectorList->clear();
    
    if (!m_manager) return;
    
    for (auto* effector : m_manager->effectors()) {
        QListWidgetItem* item = new QListWidgetItem(m_effectorList);
        item->setData(Qt::UserRole, effector->effectorId());
        updateEffectorItem(item, effector);
        m_effectorList->addItem(item);
    }
}

void EffectorControlPanel::updateEffectorItem(QListWidgetItem* item, EffectorInterface* effector) {
    if (!item || !effector) return;
    
    QString text = QString("%1 [%2]\n%3")
        .arg(effector->displayName())
        .arg(effector->effectorType())
        .arg(statusToString(effector->status()));
    
    item->setText(text);
    item->setForeground(statusToColor(effector->status()));
    
    // Add icon based on type
    if (effector->effectorType() == "RF_JAMMER") {
        item->setIcon(QIcon());  // Would set jammer icon
    } else if (effector->effectorType() == "KINETIC") {
        item->setIcon(QIcon());  // Would set missile icon
    } else if (effector->effectorType() == "DIRECTED_ENERGY") {
        item->setIcon(QIcon());  // Would set laser icon
    }
}

void EffectorControlPanel::onEffectorItemClicked(QListWidgetItem* item) {
    if (!item || !m_manager) return;
    
    m_selectedEffectorId = item->data(Qt::UserRole).toString();
    
    auto* effector = m_manager->effector(m_selectedEffectorId);
    if (effector) {
        m_selectedEffectorLabel->setText(QString("%1 (%2)")
            .arg(effector->displayName())
            .arg(effector->effectorType()));
        
        m_manager->selectEffector(m_selectedEffectorId);
        emit effectorSelected(m_selectedEffectorId);
        
        updateEffectorStatus();
    }
}

void EffectorControlPanel::onEngageClicked() {
    if (!m_manager) return;
    
    QString trackId = m_manager->selectedTrackId();
    if (trackId.isEmpty()) {
        QMessageBox::warning(this, "Engage", "No track selected for engagement.");
        return;
    }
    
    if (m_selectedEffectorId.isEmpty()) {
        QMessageBox::warning(this, "Engage", "No effector selected.");
        return;
    }
    
    // Request authorization (this starts the engagement workflow)
    m_manager->requestAuthorization();
    
    emit engageRequested(trackId);
}

void EffectorControlPanel::onDisengageClicked() {
    if (!m_manager) return;
    
    if (!m_selectedEffectorId.isEmpty()) {
        auto* effector = m_manager->effector(m_selectedEffectorId);
        if (effector && effector->isEngaged()) {
            effector->disengage();
        }
    }
}

void EffectorControlPanel::updateEffectorStatus() {
    if (!m_manager) return;
    
    // Update list items
    for (int i = 0; i < m_effectorList->count(); i++) {
        QListWidgetItem* item = m_effectorList->item(i);
        QString effectorId = item->data(Qt::UserRole).toString();
        auto* effector = m_manager->effector(effectorId);
        if (effector) {
            updateEffectorItem(item, effector);
        }
    }
    
    // Update selected effector detail
    if (!m_selectedEffectorId.isEmpty()) {
        auto* effector = m_manager->effector(m_selectedEffectorId);
        if (effector) {
            m_statusLabel->setText(QString("Status: %1").arg(statusToString(effector->status())));
            m_statusLabel->setStyleSheet(QString("color: %1;").arg(statusToColor(effector->status()).name()));
            
            int readiness = static_cast<int>(effector->health().readiness * 100);
            m_readinessBar->setValue(readiness);
            
            if (effector->health().remainingShots >= 0) {
                m_roundsLabel->setText(QString("Rounds: %1").arg(effector->health().remainingShots));
            } else {
                m_roundsLabel->setText("Rounds: Unlimited");
            }
            
            // Enable/disable buttons based on state
            bool canEngage = effector->isReady() && !m_manager->selectedTrackId().isEmpty();
            bool canDisengage = effector->isEngaged();
            
            m_engageBtn->setEnabled(canEngage);
            m_disengageBtn->setEnabled(canDisengage);
        }
    } else {
        m_engageBtn->setEnabled(false);
        m_disengageBtn->setEnabled(false);
    }
}

QColor EffectorControlPanel::statusToColor(EffectorStatus status) {
    switch (status) {
        case EffectorStatus::Ready: return QColor(0, 200, 0);
        case EffectorStatus::Engaged: return QColor(255, 165, 0);
        case EffectorStatus::Offline: return QColor(128, 128, 128);
        case EffectorStatus::Initializing:
        case EffectorStatus::Reloading:
        case EffectorStatus::Cooldown: return QColor(255, 255, 0);
        case EffectorStatus::Fault: return QColor(255, 0, 0);
        case EffectorStatus::Maintenance: return QColor(100, 100, 255);
        default: return QColor(255, 255, 255);
    }
}

QString EffectorControlPanel::statusToString(EffectorStatus status) {
    switch (status) {
        case EffectorStatus::Unknown: return "Unknown";
        case EffectorStatus::Offline: return "Offline";
        case EffectorStatus::Initializing: return "Initializing...";
        case EffectorStatus::Ready: return "READY";
        case EffectorStatus::Engaged: return "ENGAGED";
        case EffectorStatus::Reloading: return "Reloading...";
        case EffectorStatus::Cooldown: return "Cooldown";
        case EffectorStatus::Maintenance: return "Maintenance";
        case EffectorStatus::Fault: return "FAULT";
        default: return "Unknown";
    }
}

} // namespace CounterUAS
