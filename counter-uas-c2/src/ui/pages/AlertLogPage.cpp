#include "ui/pages/AlertLogPage.h"
#include "ui/SkyGuardTheme.h"
#include "core/ThreatAssessor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

namespace CounterUAS {

// ==================== AlertLogItem Implementation ====================

AlertLogItem::AlertLogItem(QWidget* parent)
    : QFrame(parent)
    , m_severity("INFO")
    , m_acknowledged(false)
{
    setupUI();
}

void AlertLogItem::setupUI() {
    setObjectName("alertLogItem");
    setMinimumHeight(90);
    setCursor(Qt::PointingHandCursor);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(16, 12, 16, 12);
    mainLayout->setSpacing(16);
    
    // Icon
    m_iconLabel = new QLabel("\xE2\x9A\xA0", this);
    m_iconLabel->setStyleSheet(
        "font-size: 24px; color: #3388ff; "
        "background-color: rgba(51, 136, 255, 0.2); "
        "padding: 8px; border-radius: 8px;"
    );
    m_iconLabel->setFixedSize(44, 44);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_iconLabel);
    
    // Content section
    QVBoxLayout* contentLayout = new QVBoxLayout();
    contentLayout->setSpacing(4);
    
    // Header row: severity badge + timestamp
    QHBoxLayout* headerRow = new QHBoxLayout();
    headerRow->setSpacing(12);
    
    m_severityBadge = new QLabel("INFO", this);
    m_severityBadge->setStyleSheet(
        "background-color: #3388ff; color: #ffffff; font-size: 9px; "
        "font-weight: bold; padding: 3px 10px; border-radius: 8px;"
    );
    m_severityBadge->setFixedHeight(20);
    headerRow->addWidget(m_severityBadge);
    
    m_timestampLabel = new QLabel("1/20/2026, 12:33:44 AM", this);
    m_timestampLabel->setStyleSheet("font-size: 11px; color: #667788;");
    headerRow->addWidget(m_timestampLabel);
    headerRow->addStretch();
    
    contentLayout->addLayout(headerRow);
    
    // Message
    m_messageLabel = new QLabel("Alert message here", this);
    m_messageLabel->setStyleSheet("font-size: 14px; color: #ffffff;");
    m_messageLabel->setWordWrap(true);
    contentLayout->addWidget(m_messageLabel);
    
    // Target reference
    m_targetLabel = new QLabel("ID: RELATED TARGET: #1", this);
    m_targetLabel->setStyleSheet("font-size: 11px; color: #667788; font-family: 'Consolas', monospace;");
    contentLayout->addWidget(m_targetLabel);
    
    mainLayout->addLayout(contentLayout, 1);
    
    // Acknowledge button/label
    m_ackButton = new QPushButton("\xE2\x9C\x93 ACK", this);
    m_ackButton->setCursor(Qt::PointingHandCursor);
    m_ackButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #1a2d44;"
        "   color: #ffffff;"
        "   border: 1px solid #2a4455;"
        "   border-radius: 4px;"
        "   padding: 8px 16px;"
        "   font-size: 11px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2a3d55;"
        "   border-color: #00d4ff;"
        "}"
    );
    connect(m_ackButton, &QPushButton::clicked, this, &AlertLogItem::onAckClicked);
    mainLayout->addWidget(m_ackButton);
    
    m_ackLabel = new QLabel("ACKNOWLEDGED", this);
    m_ackLabel->setStyleSheet(
        "font-size: 10px; color: #667788; font-weight: bold;"
    );
    m_ackLabel->hide();
    mainLayout->addWidget(m_ackLabel);
    
    updateStyle();
}

void AlertLogItem::setAlertId(const QString& id) {
    m_alertId = id;
}

void AlertLogItem::setTimestamp(const QDateTime& timestamp) {
    m_timestampLabel->setText(timestamp.toString("M/d/yyyy, h:mm:ss AP"));
}

void AlertLogItem::setSeverity(const QString& severity) {
    m_severity = severity.toUpper();
    m_severityBadge->setText(m_severity);
    updateStyle();
}

void AlertLogItem::setMessage(const QString& message) {
    m_messageLabel->setText(message);
}

void AlertLogItem::setRelatedTarget(const QString& targetId) {
    if (targetId.isEmpty()) {
        m_targetLabel->hide();
    } else {
        m_targetLabel->setText(QString("ID: RELATED TARGET: %1").arg(targetId));
        m_targetLabel->show();
    }
}

void AlertLogItem::setAcknowledged(bool acknowledged) {
    m_acknowledged = acknowledged;
    
    if (acknowledged) {
        m_ackButton->hide();
        m_ackLabel->show();
    } else {
        m_ackButton->show();
        m_ackLabel->hide();
    }
}

void AlertLogItem::updateStyle() {
    QString borderColor = "#3388ff";
    QString badgeBg = "#3388ff";
    QString iconBg = "rgba(51, 136, 255, 0.2)";
    QString iconColor = "#3388ff";
    
    if (m_severity == "CRITICAL" || m_severity == "DANGER") {
        borderColor = "#ff3344";
        badgeBg = "#ff3344";
        iconBg = "rgba(255, 51, 68, 0.2)";
        iconColor = "#ff3344";
    } else if (m_severity == "WARNING") {
        borderColor = "#ffaa00";
        badgeBg = "#ffaa00";
        iconBg = "rgba(255, 170, 0, 0.2)";
        iconColor = "#ffaa00";
    }
    
    setStyleSheet(QString(
        "QFrame#alertLogItem {"
        "   background-color: #111d2e;"
        "   border: 1px solid #1a3344;"
        "   border-radius: 8px;"
        "   border-left: 4px solid %1;"
        "}"
        "QFrame#alertLogItem:hover {"
        "   background-color: #1a2d44;"
        "}"
    ).arg(borderColor));
    
    m_severityBadge->setStyleSheet(QString(
        "background-color: %1; color: #ffffff; font-size: 9px; "
        "font-weight: bold; padding: 3px 10px; border-radius: 8px;"
    ).arg(badgeBg));
    
    m_iconLabel->setStyleSheet(QString(
        "font-size: 24px; color: %1; "
        "background-color: %2; "
        "padding: 8px; border-radius: 8px;"
    ).arg(iconColor).arg(iconBg));
}

void AlertLogItem::onAckClicked() {
    setAcknowledged(true);
    emit acknowledgeClicked(m_alertId);
}

// ==================== AlertLogPage Implementation ====================

AlertLogPage::AlertLogPage(QWidget* parent)
    : QWidget(parent)
    , m_threatAssessor(nullptr)
{
    setupUI();
    addSampleAlerts();
}

void AlertLogPage::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 24);
    mainLayout->setSpacing(24);
    
    // Header section
    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(16);
    
    // Icon and title
    QLabel* iconLabel = new QLabel("\xE2\x9A\xA0", this);
    iconLabel->setStyleSheet(
        "font-size: 28px; color: #ffffff; background-color: #111d2e; "
        "padding: 12px; border-radius: 8px;"
    );
    headerLayout->addWidget(iconLabel);
    
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);
    
    QLabel* titleLabel = new QLabel("SYSTEM ALERTS", this);
    titleLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: #ffffff; letter-spacing: 2px;"
    );
    titleLayout->addWidget(titleLabel);
    
    QLabel* subtitleLabel = new QLabel("Audit log of all security events and system warnings", this);
    subtitleLabel->setStyleSheet(
        "font-size: 13px; color: #667788;"
    );
    titleLayout->addWidget(subtitleLabel);
    
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    
    mainLayout->addLayout(headerLayout);
    
    // Alerts scroll area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setStyleSheet(
        "QScrollArea { background-color: transparent; border: none; }"
    );
    
    QWidget* alertsContainer = new QWidget(this);
    alertsContainer->setStyleSheet("background-color: transparent;");
    m_alertsLayout = new QVBoxLayout(alertsContainer);
    m_alertsLayout->setContentsMargins(0, 0, 0, 0);
    m_alertsLayout->setSpacing(12);
    m_alertsLayout->addStretch();
    
    m_scrollArea->setWidget(alertsContainer);
    mainLayout->addWidget(m_scrollArea);
}

void AlertLogPage::setThreatAssessor(ThreatAssessor* assessor) {
    if (m_threatAssessor) {
        disconnect(m_threatAssessor, nullptr, this, nullptr);
    }
    
    m_threatAssessor = assessor;
    
    if (m_threatAssessor) {
        connect(m_threatAssessor, &ThreatAssessor::newAlert,
                this, [this](const ThreatAlert& alert) {
                    addAlert(alert.alertId, alert.timestamp, 
                             alert.threatLevel >= 4 ? "CRITICAL" : "INFO",
                             alert.message, alert.trackId);
                });
        
        refreshAlerts();
    }
}

void AlertLogPage::refreshAlerts() {
    clearAlerts();
    
    if (m_threatAssessor) {
        auto alerts = m_threatAssessor->alerts();
        for (const auto& alert : alerts) {
            QString severity = alert.threatLevel >= 4 ? "CRITICAL" : 
                              (alert.threatLevel >= 3 ? "WARNING" : "INFO");
            addAlert(alert.alertId, alert.timestamp, severity, 
                     alert.message, alert.trackId);
            
            if (alert.acknowledged && m_alertItems.contains(alert.alertId)) {
                m_alertItems[alert.alertId]->setAcknowledged(true);
            }
        }
    }
}

void AlertLogPage::addAlert(const QString& id, const QDateTime& timestamp,
                            const QString& severity, const QString& message,
                            const QString& targetId) {
    if (m_alertItems.contains(id)) return;
    
    AlertLogItem* item = new AlertLogItem(this);
    item->setAlertId(id);
    item->setTimestamp(timestamp);
    item->setSeverity(severity);
    item->setMessage(message);
    item->setRelatedTarget(targetId);
    
    connect(item, &AlertLogItem::acknowledgeClicked,
            this, &AlertLogPage::onAlertAcknowledged);
    connect(item, &AlertLogItem::alertClicked,
            this, &AlertLogPage::onAlertClicked);
    
    m_alertItems[id] = item;
    
    // Insert at the beginning (most recent first)
    m_alertsLayout->insertWidget(0, item);
}

void AlertLogPage::clearAlerts() {
    for (auto* item : m_alertItems) {
        m_alertsLayout->removeWidget(item);
        item->deleteLater();
    }
    m_alertItems.clear();
}

void AlertLogPage::addSampleAlerts() {
    // Add sample alerts matching the reference design
    QDateTime now = QDateTime::currentDateTime();
    
    addAlert("ALERT-001", now, "INFO", 
             "Friendly asset entering monitored airspace", "#2");
    m_alertItems["ALERT-001"]->setAcknowledged(true);
    
    addAlert("ALERT-002", now, "CRITICAL",
             "Unauthorized drone detected in Sector 4", "#1");
}

void AlertLogPage::onAlertAcknowledged(const QString& alertId) {
    if (m_threatAssessor) {
        m_threatAssessor->acknowledgeAlert(alertId, "OPERATOR-01");
    }
    emit alertAcknowledged(alertId);
}

void AlertLogPage::onAlertClicked(const QString& alertId) {
    emit alertSelected(alertId);
}

} // namespace CounterUAS
