#include "ui/HeaderBar.h"
#include "ui/SkyGuardTheme.h"
#include &lt;QHBoxLayout&gt;
#include &lt;QVBoxLayout&gt;
#include &lt;QDateTime&gt;
#include &lt;QFrame&gt;
#include &lt;QGraphicsDropShadowEffect&gt;
#include &lt;QIcon&gt;

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
    connect(m_timeTimer, &amp;QTimer::timeout, this, &amp;HeaderBar::updateTime);
    m_timeTimer-&gt;start(1000);
    updateTime();
}

void HeaderBar::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout-&gt;setContentsMargins(24, 0, 24, 0);
    mainLayout-&gt;setSpacing(0);
    
    // Apply subtle shadow to header
    QGraphicsDropShadowEffect* headerShadow = new QGraphicsDropShadowEffect(this);
    headerShadow-&gt;setBlurRadius(15);
    headerShadow-&gt;setOffset(0, 2);
    headerShadow-&gt;setColor(QColor(0, 0, 0, 80));
    setGraphicsEffect(headerShadow);
    
    // === System Status ===
    QWidget* systemStatusWidget = new QWidget(this);
    systemStatusWidget-&gt;setObjectName("headerStatWidget");
    systemStatusWidget-&gt;setStyleSheet(
        "border-right: 1px solid #1a3344; padding: 8px 32px 8px 0;"
    );
    QVBoxLayout* statusLayout = new QVBoxLayout(systemStatusWidget);
    statusLayout-&gt;setContentsMargins(0, 12, 0, 12);
    statusLayout-&gt;setSpacing(6);
    
    m_systemStatusLabel = new QLabel("SYSTEM STATUS", this);
    m_systemStatusLabel-&gt;setObjectName("headerStatLabel");
    m_systemStatusLabel-&gt;setStyleSheet(
        "font-size: 10px; font-weight: 700; color: #667788; letter-spacing: 1.5px;"
    );
    statusLayout-&gt;addWidget(m_systemStatusLabel);
    
    m_systemStatusValue = new QLabel("OPERATIONAL", this);
    m_systemStatusValue-&gt;setObjectName("headerStatValue");
    m_systemStatusValue-&gt;setStyleSheet(
        "font-size: 15px; font-weight: 700; color: #00ff88;"
    );
    statusLayout-&gt;addWidget(m_systemStatusValue);
    
    mainLayout-&gt;addWidget(systemStatusWidget);
    
    // === Active Sector ===
    QWidget* sectorWidget = new QWidget(this);
    sectorWidget-&gt;setObjectName("headerStatWidget");
    sectorWidget-&gt;setStyleSheet(
        "border-right: 1px solid #1a3344; padding: 8px 32px;"
    );
    QVBoxLayout* sectorLayout = new QVBoxLayout(sectorWidget);
    sectorLayout-&gt;setContentsMargins(0, 12, 0, 12);
    sectorLayout-&gt;setSpacing(6);
    
    m_activeSectorLabel = new QLabel("ACTIVE SECTOR", this);
    m_activeSectorLabel-&gt;setObjectName("headerStatLabel");
    m_activeSectorLabel-&gt;setStyleSheet(
        "font-size: 10px; font-weight: 700; color: #667788; letter-spacing: 1.5px;"
    );
    sectorLayout-&gt;addWidget(m_activeSectorLabel);
    
    m_activeSectorValue = new QLabel("ALPHA-9", this);
    m_activeSectorValue-&gt;setObjectName("headerStatValue");
    m_activeSectorValue-&gt;setStyleSheet(
        "font-size: 15px; font-weight: 700; color: #ffffff;"
    );
    sectorLayout-&gt;addWidget(m_activeSectorValue);
    
    mainLayout-&gt;addWidget(sectorWidget);
    
    // === Threat Level ===
    QWidget* threatWidget = new QWidget(this);
    threatWidget-&gt;setObjectName("headerStatWidget");
    threatWidget-&gt;setStyleSheet(
        "border-right: 1px solid #1a3344; padding: 8px 32px;"
    );
    QVBoxLayout* threatLayout = new QVBoxLayout(threatWidget);
    threatLayout-&gt;setContentsMargins(0, 12, 0, 12);
    threatLayout-&gt;setSpacing(6);
    
    m_threatLevelLabel = new QLabel("THREAT LEVEL", this);
    m_threatLevelLabel-&gt;setObjectName("headerStatLabel");
    m_threatLevelLabel-&gt;setStyleSheet(
        "font-size: 10px; font-weight: 700; color: #667788; letter-spacing: 1.5px;"
    );
    threatLayout-&gt;addWidget(m_threatLevelLabel);
    
    m_threatLevelValue = new QLabel("ELEVATED", this);
    m_threatLevelValue-&gt;setObjectName("headerStatValueWarning");
    m_threatLevelValue-&gt;setStyleSheet(
        "font-size: 15px; font-weight: 700; color: #ffaa00;"
    );
    threatLayout-&gt;addWidget(m_threatLevelValue);
    
    mainLayout-&gt;addWidget(threatWidget);
    
    // Spacer
    mainLayout-&gt;addStretch();
    
    // === Simulate Threat Button ===
    m_simulateButton = new QPushButton(this);
    m_simulateButton-&gt;setObjectName("simulateButton");
    m_simulateButton-&gt;setCursor(Qt::PointingHandCursor);
    
    // Button with icon
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout-&gt;setContentsMargins(0, 0, 0, 0);
    btnLayout-&gt;setSpacing(8);
    
    QLabel* plusIcon = new QLabel("+", m_simulateButton);
    plusIcon-&gt;setStyleSheet("font-size: 16px; font-weight: 700; color: #00d4ff; background: transparent;");
    
    QLabel* btnText = new QLabel("SIMULATE THREAT", m_simulateButton);
    btnText-&gt;setStyleSheet(
        "font-size: 12px; font-weight: 700; color: #00d4ff; "
        "letter-spacing: 1px; background: transparent;"
    );
    
    m_simulateButton-&gt;setText("+ SIMULATE THREAT");
    m_simulateButton-&gt;setStyleSheet(
        "QPushButton#simulateButton {"
        "   background: transparent;"
        "   border: 2px solid #00d4ff;"
        "   color: #00d4ff;"
        "   border-radius: 6px;"
        "   padding: 12px 24px;"
        "   font-weight: 700;"
        "   font-size: 12px;"
        "   letter-spacing: 1px;"
        "}"
        "QPushButton#simulateButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 rgba(0, 212, 255, 0.2), stop:1 rgba(0, 212, 255, 0.1));"
        "   border-color: #66e5ff;"
        "   color: #66e5ff;"
        "}"
        "QPushButton#simulateButton:pressed {"
        "   background: rgba(0, 212, 255, 0.3);"
        "}"
    );
    connect(m_simulateButton, &amp;QPushButton::clicked,
            this, &amp;HeaderBar::simulateThreatClicked);
    mainLayout-&gt;addWidget(m_simulateButton);
    
    mainLayout-&gt;addSpacing(32);
    
    // === Zulu Time ===
    QWidget* timeWidget = new QWidget(this);
    QVBoxLayout* timeLayout = new QVBoxLayout(timeWidget);
    timeLayout-&gt;setContentsMargins(0, 10, 0, 10);
    timeLayout-&gt;setSpacing(2);
    timeLayout-&gt;setAlignment(Qt::AlignRight);
    
    m_zuluTimeLabel = new QLabel("00:00", this);
    m_zuluTimeLabel-&gt;setObjectName("zuluTime");
    m_zuluTimeLabel-&gt;setStyleSheet(
        "font-size: 36px; "
        "font-weight: 700; "
        "color: #ffffff; "
        "font-family: 'JetBrains Mono', 'Cascadia Code', 'Consolas', monospace;"
    );
    m_zuluTimeLabel-&gt;setAlignment(Qt::AlignRight);
    timeLayout-&gt;addWidget(m_zuluTimeLabel);
    
    m_zuluTimeSuffix = new QLabel("ZULU TIME", this);
    m_zuluTimeSuffix-&gt;setObjectName("zuluTimeLabel");
    m_zuluTimeSuffix-&gt;setStyleSheet(
        "font-size: 10px; "
        "color: #667788; "
        "letter-spacing: 1.5px;"
    );
    m_zuluTimeSuffix-&gt;setAlignment(Qt::AlignRight);
    timeLayout-&gt;addWidget(m_zuluTimeSuffix);
    
    mainLayout-&gt;addWidget(timeWidget);
}

void HeaderBar::updateTime() {
    QDateTime utcTime = QDateTime::currentDateTimeUtc();
    m_zuluTimeLabel-&gt;setText(utcTime.toString("HH:mm"));
}

void HeaderBar::setSystemStatus(const QString&amp; status) {
    m_systemStatusValue-&gt;setText(status.toUpper());
    
    QString upperStatus = status.toUpper();
    if (upperStatus == "OPERATIONAL") {
        m_systemStatusValue-&gt;setStyleSheet(
            "font-size: 15px; font-weight: 700; color: #00ff88;"
        );
    } else if (upperStatus == "DEGRADED") {
        m_systemStatusValue-&gt;setStyleSheet(
            "font-size: 15px; font-weight: 700; color: #ffaa00;"
        );
    } else {
        m_systemStatusValue-&gt;setStyleSheet(
            "font-size: 15px; font-weight: 700; color: #ff3344;"
        );
    }
}

void HeaderBar::setActiveSector(const QString&amp; sector) {
    m_activeSectorValue-&gt;setText(sector.toUpper());
}

void HeaderBar::setThreatLevel(const QString&amp; level) {
    m_threatLevelValue-&gt;setText(level.toUpper());
    
    QString upperLevel = level.toUpper();
    if (upperLevel == "LOW" || upperLevel == "MINIMAL") {
        m_threatLevelValue-&gt;setStyleSheet(
            "font-size: 15px; font-weight: 700; color: #00ff88;"
        );
    } else if (upperLevel == "ELEVATED" || upperLevel == "GUARDED") {
        m_threatLevelValue-&gt;setStyleSheet(
            "font-size: 15px; font-weight: 700; color: #ffaa00;"
        );
    } else if (upperLevel == "HIGH" || upperLevel == "SEVERE" || upperLevel == "CRITICAL") {
        m_threatLevelValue-&gt;setStyleSheet(
            "font-size: 15px; font-weight: 700; color: #ff3344;"
        );
    } else {
        m_threatLevelValue-&gt;setStyleSheet(
            "font-size: 15px; font-weight: 700; color: #ffffff;"
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
