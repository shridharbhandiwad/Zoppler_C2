#include "ui/SidebarNavigation.h"
#include "ui/SkyGuardTheme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFrame>

namespace CounterUAS {

SidebarNavigation::SidebarNavigation(QWidget* parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_buttonGroup(new QButtonGroup(this))
    , m_currentPage("tactical_map")
{
    setObjectName("sidebar");
    setFixedWidth(260);
    setStyleSheet(SkyGuardTheme::getSidebarStyleSheet());
    
    // Define navigation items with Unicode icons
    m_navItems = {
        {"tactical_map", "TACTICAL MAP", "\xF0\x9F\x93\x8A"},      // Chart icon
        {"threat_list", "THREAT LIST", "\xF0\x9F\x8E\xAF"},        // Target icon
        {"sensor_net", "SENSOR NET", "\xF0\x9F\x93\xA1"},          // Antenna icon
        {"alert_log", "ALERT LOG", "\xE2\x9A\xA0"},                // Warning icon
        {"system_health", "SYSTEM HEALTH", "\xF0\x9F\x93\x88"},    // Chart icon
        {"gallery", "GALLERY / REF", "\xF0\x9F\x96\xBC"}           // Picture icon
    };
    
    setupUI();
    
    // Set initial page
    setCurrentPage("tactical_map");
    
    // Connect button group
    connect(m_buttonGroup, &QButtonGroup::idClicked,
            this, &SidebarNavigation::onNavButtonClicked);
}

void SidebarNavigation::setupUI() {
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    
    // === Logo Section ===
    QWidget* logoSection = new QWidget(this);
    logoSection->setStyleSheet("background-color: transparent;");
    QVBoxLayout* logoLayout = new QVBoxLayout(logoSection);
    logoLayout->setContentsMargins(20, 24, 20, 8);
    logoLayout->setSpacing(4);
    
    // Logo with icon
    QHBoxLayout* logoRow = new QHBoxLayout();
    logoRow->setSpacing(12);
    
    // Radar/target icon (using styled label)
    QLabel* iconLabel = new QLabel(this);
    iconLabel->setText("\xE2\x97\x89");  // Circle target
    iconLabel->setStyleSheet(
        "font-size: 28px; "
        "color: #00d4ff; "
        "font-weight: bold;"
    );
    logoRow->addWidget(iconLabel);
    
    // Logo text
    m_logoLabel = new QLabel("SKYGUARD", this);
    m_logoLabel->setObjectName("logoLabel");
    m_logoLabel->setStyleSheet(
        "font-size: 22px; "
        "font-weight: bold; "
        "color: #00d4ff; "
        "letter-spacing: 2px;"
    );
    logoRow->addWidget(m_logoLabel);
    logoRow->addStretch();
    
    logoLayout->addLayout(logoRow);
    
    // Subtitle
    m_subtitleLabel = new QLabel("C-UAS COMMAND SYSTEM", this);
    m_subtitleLabel->setObjectName("subtitleLabel");
    m_subtitleLabel->setStyleSheet(
        "font-size: 10px; "
        "color: #667788; "
        "letter-spacing: 1px;"
    );
    logoLayout->addWidget(m_subtitleLabel);
    
    m_layout->addWidget(logoSection);
    
    // Divider line
    QFrame* divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("background-color: #1a3344; max-height: 1px; margin: 12px 16px;");
    m_layout->addWidget(divider);
    
    // === Navigation Buttons ===
    QWidget* navSection = new QWidget(this);
    QVBoxLayout* navLayout = new QVBoxLayout(navSection);
    navLayout->setContentsMargins(8, 8, 8, 8);
    navLayout->setSpacing(4);
    
    int buttonId = 0;
    for (const NavItem& item : m_navItems) {
        QPushButton* btn = createNavButton(item);
        m_buttonGroup->addButton(btn, buttonId++);
        navLayout->addWidget(btn);
    }
    
    m_layout->addWidget(navSection);
    
    // Spacer to push status to bottom
    m_layout->addStretch();
    
    // === Bottom Status Section ===
    QWidget* statusSection = new QWidget(this);
    statusSection->setStyleSheet("background-color: transparent;");
    QVBoxLayout* statusLayout = new QVBoxLayout(statusSection);
    statusLayout->setContentsMargins(20, 16, 20, 20);
    statusLayout->setSpacing(4);
    
    // Divider
    QFrame* statusDivider = new QFrame(this);
    statusDivider->setFrameShape(QFrame::HLine);
    statusDivider->setStyleSheet("background-color: #1a3344; max-height: 1px;");
    statusLayout->addWidget(statusDivider);
    statusLayout->addSpacing(12);
    
    // Status row
    QHBoxLayout* statusRow = new QHBoxLayout();
    statusRow->setSpacing(8);
    
    m_statusLabel = new QLabel("STATUS:", this);
    m_statusLabel->setStyleSheet(
        "font-size: 12px; "
        "color: #667788; "
        "font-weight: bold;"
    );
    statusRow->addWidget(m_statusLabel);
    
    m_statusValue = new QLabel("ONLINE", this);
    m_statusValue->setObjectName("statusIndicator");
    m_statusValue->setStyleSheet(
        "font-size: 12px; "
        "color: #00ff88; "
        "font-weight: bold;"
    );
    statusRow->addWidget(m_statusValue);
    statusRow->addStretch();
    
    statusLayout->addLayout(statusRow);
    
    // Version info
    m_versionLabel = new QLabel("V.2.4.1-ALPHA // SECURE", this);
    m_versionLabel->setObjectName("versionLabel");
    m_versionLabel->setStyleSheet(
        "font-size: 10px; "
        "color: #556677;"
    );
    statusLayout->addWidget(m_versionLabel);
    
    m_layout->addWidget(statusSection);
}

QPushButton* SidebarNavigation::createNavButton(const NavItem& item) {
    QPushButton* btn = new QPushButton(this);
    btn->setObjectName("navButton");
    btn->setCheckable(true);
    btn->setCursor(Qt::PointingHandCursor);
    
    // Create icon + text layout
    QWidget* content = new QWidget(btn);
    QHBoxLayout* layout = new QHBoxLayout(content);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(16);
    
    // Icon label (using simple text representations)
    QLabel* iconLabel = new QLabel(content);
    QString iconChar;
    
    // Map nav items to appropriate icons
    if (item.id == "tactical_map") {
        iconChar = "\xE2\x8C\xA0";  // Place of interest symbol
    } else if (item.id == "threat_list") {
        iconChar = "\xE2\x97\x89";  // Circled dot
    } else if (item.id == "sensor_net") {
        iconChar = "\xC2\xB7";      // Middle dot (signal)
    } else if (item.id == "alert_log") {
        iconChar = "\xE2\x9A\xA0";  // Warning
    } else if (item.id == "system_health") {
        iconChar = "\xE2\x86\x97";  // Arrow
    } else if (item.id == "gallery") {
        iconChar = "\xE2\x96\xA1";  // Square
    }
    
    iconLabel->setText(iconChar);
    iconLabel->setStyleSheet(
        "font-size: 18px; "
        "color: inherit; "
        "min-width: 24px; "
        "qproperty-alignment: AlignCenter;"
    );
    layout->addWidget(iconLabel);
    
    // Text label
    QLabel* textLabel = new QLabel(item.label, content);
    textLabel->setStyleSheet(
        "font-size: 13px; "
        "font-weight: 500; "
        "color: inherit; "
        "letter-spacing: 1px;"
    );
    layout->addWidget(textLabel);
    layout->addStretch();
    
    btn->setLayout(new QVBoxLayout());
    btn->layout()->setContentsMargins(0, 0, 0, 0);
    btn->layout()->addWidget(content);
    
    btn->setMinimumHeight(52);
    btn->setStyleSheet(
        "QPushButton#navButton {"
        "   background-color: transparent;"
        "   color: #8899aa;"
        "   border: none;"
        "   border-left: 3px solid transparent;"
        "   border-radius: 0px;"
        "   text-align: left;"
        "}"
        "QPushButton#navButton:hover {"
        "   background-color: #1a2d44;"
        "   color: #ffffff;"
        "}"
        "QPushButton#navButton:checked {"
        "   background-color: rgba(0, 212, 255, 0.1);"
        "   color: #00d4ff;"
        "   border-left: 3px solid #00d4ff;"
        "}"
    );
    
    btn->setProperty("navId", item.id);
    
    return btn;
}

void SidebarNavigation::setCurrentPage(const QString& pageId) {
    m_currentPage = pageId;
    
    // Update button states
    for (int i = 0; i < m_navItems.size(); ++i) {
        if (m_navItems[i].id == pageId) {
            QAbstractButton* btn = m_buttonGroup->button(i);
            if (btn) {
                btn->setChecked(true);
            }
            break;
        }
    }
}

void SidebarNavigation::setOnlineStatus(bool online) {
    if (online) {
        m_statusValue->setText("ONLINE");
        m_statusValue->setStyleSheet(
            "font-size: 12px; "
            "color: #00ff88; "
            "font-weight: bold;"
        );
    } else {
        m_statusValue->setText("OFFLINE");
        m_statusValue->setStyleSheet(
            "font-size: 12px; "
            "color: #ff3344; "
            "font-weight: bold;"
        );
    }
}

void SidebarNavigation::onNavButtonClicked(int index) {
    if (index >= 0 && index < m_navItems.size()) {
        m_currentPage = m_navItems[index].id;
        emit pageSelected(m_currentPage);
    }
}

} // namespace CounterUAS
