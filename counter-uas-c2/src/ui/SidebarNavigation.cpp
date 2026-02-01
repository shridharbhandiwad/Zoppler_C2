#include "ui/SidebarNavigation.h"
#include "ui/SkyGuardTheme.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QVariant>
#include <QPainter>
#include <QIcon>

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
    
    // Define navigation items with SVG icon paths
    m_navItems = {
        {"tactical_map", "TACTICAL MAP", ":/icons/tactical-map.svg"},
        {"threat_list", "THREAT LIST", ":/icons/target.svg"},
        {"sensor_net", "SENSOR NET", ":/icons/radar.svg"},
        {"alert_log", "ALERT LOG", ":/icons/alert.svg"},
        {"system_health", "SYSTEM HEALTH", ":/icons/system-health.svg"},
        {"gallery", "GALLERY / REF", ":/icons/gallery.svg"}
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
    
    // Logo icon (using SVG)
    QLabel* iconLabel = new QLabel(this);
    QIcon logoIcon(":/icons/zoppler-logo.svg");
    iconLabel->setPixmap(logoIcon.pixmap(32, 32));
    iconLabel->setFixedSize(32, 32);
    logoRow->addWidget(iconLabel);
    
    // Logo text
    m_logoLabel = new QLabel("ZOPPLER", this);
    m_logoLabel->setObjectName("logoLabel");
    m_logoLabel->setStyleSheet(
        "font-size: 22px; "
        "font-weight: 700; "
        "color: #00d4ff; "
        "letter-spacing: 3px;"
    );
    logoRow->addWidget(m_logoLabel);
    logoRow->addStretch();
    
    logoLayout->addLayout(logoRow);
    
    // Subtitle
    m_subtitleLabel = new QLabel("C-UAS COMMAND SYSTEM", this);
    m_subtitleLabel->setObjectName("subtitleLabel");
    m_subtitleLabel->setStyleSheet(
        "font-size: 10px; "
        "color: #889aab; "
        "letter-spacing: 1.5px;"
    );
    logoLayout->addWidget(m_subtitleLabel);
    
    m_layout->addWidget(logoSection);
    
    // Divider line with gradient
    QFrame* divider = new QFrame(this);
    divider->setObjectName("sidebarDivider");
    divider->setFrameShape(QFrame::HLine);
    divider->setFixedHeight(1);
    divider->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 transparent, stop:0.5 #2a4455, stop:1 transparent);"
        "margin: 12px 16px;"
    );
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
    statusDivider->setFixedHeight(1);
    statusDivider->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 transparent, stop:0.5 #2a4455, stop:1 transparent);"
    );
    statusLayout->addWidget(statusDivider);
    statusLayout->addSpacing(12);
    
    // Status row
    QHBoxLayout* statusRow = new QHBoxLayout();
    statusRow->setSpacing(8);
    
    m_statusLabel = new QLabel("STATUS:", this);
    m_statusLabel->setStyleSheet(
        "font-size: 12px; "
        "color: #ffffff; "
        "font-weight: 600;"
    );
    statusRow->addWidget(m_statusLabel);
    
    m_statusValue = new QLabel("ONLINE", this);
    m_statusValue->setObjectName("statusIndicator");
    m_statusValue->setStyleSheet(
        "font-size: 12px; "
        "color: #00ff88; "
        "font-weight: 700;"
    );
    statusRow->addWidget(m_statusValue);
    statusRow->addStretch();
    
    statusLayout->addLayout(statusRow);
    
    // Version info
    m_versionLabel = new QLabel("V.2.4.1-ALPHA // SECURE", this);
    m_versionLabel->setObjectName("versionLabel");
    m_versionLabel->setStyleSheet(
        "font-size: 10px; "
        "color: #667788;"
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
    layout->setContentsMargins(12, 10, 12, 10);
    layout->setSpacing(14);
    
    // Icon label (using SVG)
    QLabel* iconLabel = new QLabel(content);
    QIcon icon(item.icon);
    iconLabel->setPixmap(icon.pixmap(20, 20));
    iconLabel->setFixedSize(24, 24);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("background: transparent;");
    layout->addWidget(iconLabel);
    
    // Text label
    QLabel* textLabel = new QLabel(item.label, content);
    textLabel->setStyleSheet(
        "font-size: 12px; "
        "font-weight: 600; "
        "color: inherit; "
        "letter-spacing: 1px; "
        "background: transparent;"
    );
    layout->addWidget(textLabel);
    layout->addStretch();
    
    btn->setLayout(new QVBoxLayout());
    btn->layout()->setContentsMargins(0, 0, 0, 0);
    btn->layout()->addWidget(content);
    
    btn->setMinimumHeight(48);
    btn->setStyleSheet(
        "QPushButton#navButton {"
        "   background: transparent;"
        "   color: #a8b8c8;"
        "   border: none;"
        "   border-left: 3px solid transparent;"
        "   border-radius: 0px;"
        "   text-align: left;"
        "}"
        "QPushButton#navButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 rgba(0, 212, 255, 0.15), stop:1 rgba(0, 212, 255, 0.05));"
        "   color: #00d4ff;"
        "}"
        "QPushButton#navButton:checked {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 rgba(0, 212, 255, 0.2), stop:1 rgba(0, 212, 255, 0.05));"
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
            "font-weight: 700;"
        );
    } else {
        m_statusValue->setText("OFFLINE");
        m_statusValue->setStyleSheet(
            "font-size: 12px; "
            "color: #ff3344; "
            "font-weight: 700;"
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
