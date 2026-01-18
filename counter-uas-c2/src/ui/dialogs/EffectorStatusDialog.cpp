/**
 * @file EffectorStatusDialog.cpp
 * @brief Implementation of effector status dialog
 */

#include "ui/dialogs/EffectorStatusDialog.h"
#include "core/EngagementManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>

namespace CounterUAS {

EffectorStatusDialog::EffectorStatusDialog(EngagementManager* manager, QWidget* parent)
    : QDialog(parent)
    , m_manager(manager)
    , m_refreshTimer(new QTimer(this))
{
    setWindowTitle("Effector Status");
    setMinimumSize(800, 500);
    
    setupUI();
    
    connect(m_refreshTimer, &QTimer::timeout, this, &EffectorStatusDialog::refreshStatus);
    m_refreshTimer->start(500);  // Refresh every 500ms
    
    refreshStatus();
}

void EffectorStatusDialog::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    
    // Left side - effector list
    QGroupBox* listGroup = new QGroupBox("Effectors", this);
    QVBoxLayout* listLayout = new QVBoxLayout(listGroup);
    
    m_effectorTable = new QTableWidget(this);
    m_effectorTable->setColumnCount(4);
    m_effectorTable->setHorizontalHeaderLabels({"ID", "Type", "Status", "Readiness"});
    m_effectorTable->horizontalHeader()->setStretchLastSection(true);
    m_effectorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_effectorTable->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_effectorTable, &QTableWidget::currentCellChanged,
            this, [this](int row, int, int, int) { onEffectorSelected(row); });
    listLayout->addWidget(m_effectorTable);
    
    mainLayout->addWidget(listGroup, 1);
    
    // Right side - detail and control
    QVBoxLayout* rightLayout = new QVBoxLayout();
    
    // Detail panel
    QGroupBox* detailGroup = new QGroupBox("Effector Details", this);
    QFormLayout* detailLayout = new QFormLayout(detailGroup);
    
    m_detailId = new QLabel("-", this);
    detailLayout->addRow("ID:", m_detailId);
    
    m_detailType = new QLabel("-", this);
    detailLayout->addRow("Type:", m_detailType);
    
    m_detailStatus = new QLabel("-", this);
    detailLayout->addRow("Status:", m_detailStatus);
    
    m_readinessBar = new QProgressBar(this);
    m_readinessBar->setRange(0, 100);
    detailLayout->addRow("Readiness:", m_readinessBar);
    
    m_detailEngagements = new QLabel("-", this);
    detailLayout->addRow("Total Engagements:", m_detailEngagements);
    
    m_detailLastEngagement = new QLabel("-", this);
    detailLayout->addRow("Last Engagement:", m_detailLastEngagement);
    
    m_detailRoundsRemaining = new QLabel("-", this);
    detailLayout->addRow("Rounds Remaining:", m_detailRoundsRemaining);
    
    rightLayout->addWidget(detailGroup);
    
    // Control panel
    QGroupBox* controlGroup = new QGroupBox("Control", this);
    QVBoxLayout* controlLayout = new QVBoxLayout(controlGroup);
    
    m_initBtn = new QPushButton("Initialize", this);
    m_initBtn->setEnabled(false);
    connect(m_initBtn, &QPushButton::clicked, this, &EffectorStatusDialog::onInitialize);
    controlLayout->addWidget(m_initBtn);
    
    m_shutdownBtn = new QPushButton("Shutdown", this);
    m_shutdownBtn->setEnabled(false);
    connect(m_shutdownBtn, &QPushButton::clicked, this, &EffectorStatusDialog::onShutdown);
    controlLayout->addWidget(m_shutdownBtn);
    
    m_resetBtn = new QPushButton("Reset", this);
    m_resetBtn->setEnabled(false);
    connect(m_resetBtn, &QPushButton::clicked, this, &EffectorStatusDialog::onReset);
    controlLayout->addWidget(m_resetBtn);
    
    m_testBtn = new QPushButton("Test Engage", this);
    m_testBtn->setEnabled(false);
    m_testBtn->setStyleSheet("background-color: orange;");
    connect(m_testBtn, &QPushButton::clicked, this, &EffectorStatusDialog::onTestEngage);
    controlLayout->addWidget(m_testBtn);
    
    controlLayout->addStretch();
    
    rightLayout->addWidget(controlGroup);
    
    // Close button
    QPushButton* closeBtn = new QPushButton("Close", this);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    rightLayout->addWidget(closeBtn);
    
    mainLayout->addLayout(rightLayout);
}

void EffectorStatusDialog::refreshStatus() {
    if (!m_manager) return;
    
    auto effectors = m_manager->effectors();
    
    // Update table
    m_effectorTable->setRowCount(effectors.size());
    
    for (int i = 0; i < effectors.size(); i++) {
        auto* eff = effectors[i];
        
        m_effectorTable->setItem(i, 0, new QTableWidgetItem(eff->effectorId()));
        m_effectorTable->setItem(i, 1, new QTableWidgetItem(eff->effectorType()));
        
        QString statusStr = statusToString(eff->status());
        QTableWidgetItem* statusItem = new QTableWidgetItem(statusStr);
        statusItem->setForeground(statusToColor(eff->status()));
        m_effectorTable->setItem(i, 2, statusItem);
        
        QString readiness = QString("%1%").arg(eff->health().readiness * 100, 0, 'f', 0);
        m_effectorTable->setItem(i, 3, new QTableWidgetItem(readiness));
    }
    
    // Update detail panel if effector selected
    if (!m_selectedEffectorId.isEmpty()) {
        auto* eff = m_manager->effector(m_selectedEffectorId);
        if (eff) {
            m_detailId->setText(eff->effectorId());
            m_detailType->setText(eff->effectorType());
            m_detailStatus->setText(statusToString(eff->status()));
            m_detailStatus->setStyleSheet(QString("color: %1;").arg(statusToColor(eff->status()).name()));
            m_readinessBar->setValue(static_cast<int>(eff->health().readiness * 100));
            m_detailEngagements->setText(QString::number(eff->health().totalEngagements));
            
            if (eff->health().lastEngagementTime.isValid()) {
                m_detailLastEngagement->setText(eff->health().lastEngagementTime.toString("yyyy-MM-dd hh:mm:ss"));
            } else {
                m_detailLastEngagement->setText("Never");
            }
            
            if (eff->health().remainingShots >= 0) {
                m_detailRoundsRemaining->setText(QString::number(eff->health().remainingShots));
            } else {
                m_detailRoundsRemaining->setText("N/A");
            }
        }
    }
}

void EffectorStatusDialog::onEffectorSelected(int row) {
    if (row < 0 || !m_manager) {
        m_selectedEffectorId.clear();
        m_initBtn->setEnabled(false);
        m_shutdownBtn->setEnabled(false);
        m_resetBtn->setEnabled(false);
        m_testBtn->setEnabled(false);
        return;
    }
    
    m_selectedEffectorId = m_effectorTable->item(row, 0)->text();
    
    auto* eff = m_manager->effector(m_selectedEffectorId);
    if (eff) {
        bool isOffline = eff->status() == EffectorStatus::Offline;
        bool isReady = eff->status() == EffectorStatus::Ready;
        bool isFault = eff->status() == EffectorStatus::Fault;
        
        m_initBtn->setEnabled(isOffline);
        m_shutdownBtn->setEnabled(!isOffline);
        m_resetBtn->setEnabled(isFault);
        m_testBtn->setEnabled(isReady);
    }
}

void EffectorStatusDialog::onInitialize() {
    if (m_selectedEffectorId.isEmpty() || !m_manager) return;
    
    auto* eff = m_manager->effector(m_selectedEffectorId);
    if (eff) {
        eff->initialize();
        emit effectorInitializeRequested(m_selectedEffectorId);
    }
}

void EffectorStatusDialog::onShutdown() {
    if (m_selectedEffectorId.isEmpty() || !m_manager) return;
    
    auto* eff = m_manager->effector(m_selectedEffectorId);
    if (eff) {
        eff->shutdown();
        emit effectorShutdownRequested(m_selectedEffectorId);
    }
}

void EffectorStatusDialog::onReset() {
    if (m_selectedEffectorId.isEmpty() || !m_manager) return;
    
    auto* eff = m_manager->effector(m_selectedEffectorId);
    if (eff) {
        eff->reset();
        emit effectorResetRequested(m_selectedEffectorId);
    }
}

void EffectorStatusDialog::onTestEngage() {
    if (m_selectedEffectorId.isEmpty() || !m_manager) return;
    
    QMessageBox::information(this, "Test Engagement",
        QString("Test engagement initiated for %1.\n"
                "This would engage a simulated test target.")
            .arg(m_selectedEffectorId));
}

QString EffectorStatusDialog::statusToString(EffectorStatus status) {
    switch (status) {
        case EffectorStatus::Unknown: return "Unknown";
        case EffectorStatus::Offline: return "Offline";
        case EffectorStatus::Initializing: return "Initializing";
        case EffectorStatus::Ready: return "Ready";
        case EffectorStatus::Engaged: return "Engaged";
        case EffectorStatus::Reloading: return "Reloading";
        case EffectorStatus::Cooldown: return "Cooldown";
        case EffectorStatus::Maintenance: return "Maintenance";
        case EffectorStatus::Fault: return "Fault";
        default: return "Unknown";
    }
}

QColor EffectorStatusDialog::statusToColor(EffectorStatus status) {
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

} // namespace CounterUAS
