#include "ui/HeaderBar.h"
#include "ui/SkyGuardTheme.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QFrame>

namespace CounterUAS {

HeaderBar::HeaderBar(QWidget* parent)
    : QWidget(parent)
    , m_timeTimer(new QTimer(this))
{
    setObjectName("headerBar");
    setFixedHeight(80);
    setStyleSheet(SkyGuardTheme::getHeaderStyleSheet());
    
    setupUI();
    
    // Start time update timer
    connect(m_timeTimer, &QTimer::timeout, this, &HeaderBar::updateTime);
    m_timeTimer->start(1000);
    updateTime();
}

void HeaderBar::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(24, 0, 24, 0);
    mainLayout->setSpacing(0);
    
    // === System Status ===
    QWidget* systemStatusWidget = new QWidget(this);
    systemStatusWidget->setObjectName("headerStatWidget");
    systemStatusWidget->setStyleSheet("border-right: 1px solid #1a3344; padding: 8px 32px 8px 0;");
    QVBoxLayout* statusLayout = new QVBoxLayout(systemStatusWidget);
    statusLayout->setContentsMargins(0, 12, 0, 12);
    statusLayout->setSpacing(4);
    
    m_systemStatusLabel = new QLabel("SYSTEM STATUS", this);
    m_systemStatusLabel->setObjectName("headerStatLabel");
    m_systemStatusLabel->setStyleSheet(
        "font-size: 10px; font-weight: bold; color: #667788; letter-spacing: 1px;"
    );
    statusLayout->addWidget(m_systemStatusLabel);
    
    m_systemStatusValue = new QLabel("OPERATIONAL", this);
    m_systemStatusValue->setObjectName("headerStatValue");
    m_systemStatusValue->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #00ff88;"
    );
    statusLayout->addWidget(m_systemStatusValue);
    
    mainLayout->addWidget(systemStatusWidget);
    
    // === Active Sector ===
    QWidget* sectorWidget = new QWidget(this);
    sectorWidget->setObjectName("headerStatWidget");
    sectorWidget->setStyleSheet("border-right: 1px solid #1a3344; padding: 8px 32px;");
    QVBoxLayout* sectorLayout = new QVBoxLayout(sectorWidget);
    sectorLayout->setContentsMargins(0, 12, 0, 12);
    sectorLayout->setSpacing(4);
    
    m_activeSectorLabel = new QLabel("ACTIVE SECTOR", this);
    m_activeSectorLabel->setObjectName("headerStatLabel");
    m_activeSectorLabel->setStyleSheet(
        "font-size: 10px; font-weight: bold; color: #667788; letter-spacing: 1px;"
    );
    sectorLayout->addWidget(m_activeSectorLabel);
    
    m_activeSectorValue = new QLabel("ALPHA-9", this);
    m_activeSectorValue->setObjectName("headerStatValue");
    m_activeSectorValue->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #ffffff;"
    );
    sectorLayout->addWidget(m_activeSectorValue);
    
    mainLayout->addWidget(sectorWidget);
    
    // === Threat Level ===
    QWidget* threatWidget = new QWidget(this);
    threatWidget->setObjectName("headerStatWidget");
    threatWidget->setStyleSheet("border-right: 1px solid #1a3344; padding: 8px 32px;");
    QVBoxLayout* threatLayout = new QVBoxLayout(threatWidget);
    threatLayout->setContentsMargins(0, 12, 0, 12);
    threatLayout->setSpacing(4);
    
    m_threatLevelLabel = new QLabel("THREAT LEVEL", this);
    m_threatLevelLabel->setObjectName("headerStatLabel");
    m_threatLevelLabel->setStyleSheet(
        "font-size: 10px; font-weight: bold; color: #667788; letter-spacing: 1px;"
    );
    threatLayout->addWidget(m_threatLevelLabel);
    
    m_threatLevelValue = new QLabel("ELEVATED", this);
    m_threatLevelValue->setObjectName("headerStatValueWarning");
    m_threatLevelValue->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #ffaa00;"
    );
    threatLayout->addWidget(m_threatLevelValue);
    
    mainLayout->addWidget(threatWidget);
    
    // Spacer
    mainLayout->addStretch();
    
    // === Simulate Threat Button ===
    m_simulateButton = new QPushButton("+ SIMULATE THREAT", this);
    m_simulateButton->setObjectName("simulateButton");
    m_simulateButton->setCursor(Qt::PointingHandCursor);
    m_simulateButton->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: 2px solid #00d4ff;"
        "   color: #00d4ff;"
        "   border-radius: 4px;"
        "   padding: 12px 24px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "   letter-spacing: 1px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(0, 212, 255, 0.15);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(0, 212, 255, 0.25);"
        "}"
    );
    connect(m_simulateButton, &QPushButton::clicked,
            this, &HeaderBar::simulateThreatClicked);
    mainLayout->addWidget(m_simulateButton);
    
    mainLayout->addSpacing(32);
    
    // === Zulu Time ===
    QWidget* timeWidget = new QWidget(this);
    QVBoxLayout* timeLayout = new QVBoxLayout(timeWidget);
    timeLayout->setContentsMargins(0, 12, 0, 12);
    timeLayout->setSpacing(0);
    timeLayout->setAlignment(Qt::AlignRight);
    
    m_zuluTimeLabel = new QLabel("00:00", this);
    m_zuluTimeLabel->setObjectName("zuluTime");
    m_zuluTimeLabel->setStyleSheet(
        "font-size: 36px; "
        "font-weight: bold; "
        "color: #ffffff; "
        "font-family: 'Consolas', 'Monaco', 'Courier New', monospace;"
    );
    m_zuluTimeLabel->setAlignment(Qt::AlignRight);
    timeLayout->addWidget(m_zuluTimeLabel);
    
    m_zuluTimeSuffix = new QLabel("ZULU TIME", this);
    m_zuluTimeSuffix->setObjectName("zuluTimeLabel");
    m_zuluTimeSuffix->setStyleSheet(
        "font-size: 10px; "
        "color: #667788; "
        "letter-spacing: 1px;"
    );
    m_zuluTimeSuffix->setAlignment(Qt::AlignRight);
    timeLayout->addWidget(m_zuluTimeSuffix);
    
    mainLayout->addWidget(timeWidget);
}

void HeaderBar::updateTime() {
    QDateTime utcTime = QDateTime::currentDateTimeUtc();
    m_zuluTimeLabel->setText(utcTime.toString("HH:mm"));
}

void HeaderBar::setSystemStatus(const QString& status) {
    m_systemStatusValue->setText(status.toUpper());
    
    if (status.toUpper() == "OPERATIONAL") {
        m_systemStatusValue->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #00ff88;"
        );
    } else if (status.toUpper() == "DEGRADED") {
        m_systemStatusValue->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #ffaa00;"
        );
    } else {
        m_systemStatusValue->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #ff3344;"
        );
    }
}

void HeaderBar::setActiveSector(const QString& sector) {
    m_activeSectorValue->setText(sector.toUpper());
}

void HeaderBar::setThreatLevel(const QString& level) {
    m_threatLevelValue->setText(level.toUpper());
    
    QString upperLevel = level.toUpper();
    if (upperLevel == "LOW" || upperLevel == "MINIMAL") {
        m_threatLevelValue->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #00ff88;"
        );
    } else if (upperLevel == "ELEVATED" || upperLevel == "GUARDED") {
        m_threatLevelValue->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #ffaa00;"
        );
    } else if (upperLevel == "HIGH" || upperLevel == "SEVERE" || upperLevel == "CRITICAL") {
        m_threatLevelValue->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #ff3344;"
        );
    } else {
        m_threatLevelValue->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #ffffff;"
        );
    }
}

void HeaderBar::setThreatCount(int active, int total) {
    Q_UNUSED(active)
    Q_UNUSED(total)
    // Can be extended to show threat count
}

void HeaderBar::setSensorCount(int active, int total) {
    Q_UNUSED(active)
    Q_UNUSED(total)
    // Can be extended to show sensor count
}

} // namespace CounterUAS
