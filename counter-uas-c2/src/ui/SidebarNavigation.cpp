#include "ui/SidebarNavigation.h"
#include "ui/SkyGuardTheme.h"
#include &lt;QVBoxLayout&gt;
#include &lt;QHBoxLayout&gt;
#include &lt;QSpacerItem&gt;
#include &lt;QFrame&gt;
#include &lt;QVariant&gt;
#include &lt;QSvgWidget&gt;
#include &lt;QPainter&gt;
#include &lt;QIcon&gt;

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
    connect(m_buttonGroup, &amp;QButtonGroup::idClicked,
            this, &amp;SidebarNavigation::onNavButtonClicked);
}

void SidebarNavigation::setupUI() {
    m_layout-&gt;setContentsMargins(0, 0, 0, 0);
    m_layout-&gt;setSpacing(0);
    
    // === Logo Section ===
    QWidget* logoSection = new QWidget(this);
    logoSection-&gt;setStyleSheet("background-color: transparent;");
    QVBoxLayout* logoLayout = new QVBoxLayout(logoSection);
    logoLayout-&gt;setContentsMargins(20, 24, 20, 8);
    logoLayout-&gt;setSpacing(4);
    
    // Logo with icon
    QHBoxLayout* logoRow = new QHBoxLayout();
    logoRow-&gt;setSpacing(12);
    
    // Logo icon (using SVG)
    QLabel* iconLabel = new QLabel(this);
    QIcon logoIcon(":/icons/zoppler-logo.svg");
    iconLabel-&gt;setPixmap(logoIcon.pixmap(32, 32));
    iconLabel-&gt;setFixedSize(32, 32);
    logoRow-&gt;addWidget(iconLabel);
    
    // Logo text
    m_logoLabel = new QLabel("ZOPPLER", this);
    m_logoLabel-&gt;setObjectName("logoLabel");
    m_logoLabel-&gt;setStyleSheet(
        "font-size: 22px; "
        "font-weight: 700; "
        "color: #00d4ff; "
        "letter-spacing: 3px;"
    );
    logoRow-&gt;addWidget(m_logoLabel);
    logoRow-&gt;addStretch();
    
    logoLayout-&gt;addLayout(logoRow);
    
    // Subtitle
    m_subtitleLabel = new QLabel("C-UAS COMMAND SYSTEM", this);
    m_subtitleLabel-&gt;setObjectName("subtitleLabel");
    m_subtitleLabel-&gt;setStyleSheet(
        "font-size: 10px; "
        "color: #889aab; "
        "letter-spacing: 1.5px;"
    );
    logoLayout-&gt;addWidget(m_subtitleLabel);
    
    m_layout-&gt;addWidget(logoSection);
    
    // Divider line with gradient
    QFrame* divider = new QFrame(this);
    divider-&gt;setObjectName("sidebarDivider");
    divider-&gt;setFrameShape(QFrame::HLine);
    divider-&gt;setFixedHeight(1);
    divider-&gt;setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 transparent, stop:0.5 #2a4455, stop:1 transparent);"
        "margin: 12px 16px;"
    );
    m_layout-&gt;addWidget(divider);
    
    // === Navigation Buttons ===
    QWidget* navSection = new QWidget(this);
    QVBoxLayout* navLayout = new QVBoxLayout(navSection);
    navLayout-&gt;setContentsMargins(8, 8, 8, 8);
    navLayout-&gt;setSpacing(4);
    
    int buttonId = 0;
    for (const NavItem&amp; item : m_navItems) {
        QPushButton* btn = createNavButton(item);
        m_buttonGroup-&gt;addButton(btn, buttonId++);
        navLayout-&gt;addWidget(btn);
    }
    
    m_layout-&gt;addWidget(navSection);
    
    // Spacer to push status to bottom
    m_layout-&gt;addStretch();
    
    // === Bottom Status Section ===
    QWidget* statusSection = new QWidget(this);
    statusSection-&gt;setStyleSheet("background-color: transparent;");
    QVBoxLayout* statusLayout = new QVBoxLayout(statusSection);
    statusLayout-&gt;setContentsMargins(20, 16, 20, 20);
    statusLayout-&gt;setSpacing(4);
    
    // Divider
    QFrame* statusDivider = new QFrame(this);
    statusDivider-&gt;setFrameShape(QFrame::HLine);
    statusDivider-&gt;setFixedHeight(1);
    statusDivider-&gt;setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 transparent, stop:0.5 #2a4455, stop:1 transparent);"
    );
    statusLayout-&gt;addWidget(statusDivider);
    statusLayout-&gt;addSpacing(12);
    
    // Status row
    QHBoxLayout* statusRow = new QHBoxLayout();
    statusRow-&gt;setSpacing(8);
    
    m_statusLabel = new QLabel("STATUS:", this);
    m_statusLabel-&gt;setStyleSheet(
        "font-size: 12px; "
        "color: #ffffff; "
        "font-weight: 600;"
    );
    statusRow-&gt;addWidget(m_statusLabel);
    
    m_statusValue = new QLabel("ONLINE", this);
    m_statusValue-&gt;setObjectName("statusIndicator");
    m_statusValue-&gt;setStyleSheet(
        "font-size: 12px; "
        "color: #00ff88; "
        "font-weight: 700;"
    );
    statusRow-&gt;addWidget(m_statusValue);
    statusRow-&gt;addStretch();
    
    statusLayout-&gt;addLayout(statusRow);
    
    // Version info
    m_versionLabel = new QLabel("V.2.4.1-ALPHA // SECURE", this);
    m_versionLabel-&gt;setObjectName("versionLabel");
    m_versionLabel-&gt;setStyleSheet(
        "font-size: 10px; "
        "color: #667788;"
    );
    statusLayout-&gt;addWidget(m_versionLabel);
    
    m_layout-&gt;addWidget(statusSection);
}

QPushButton* SidebarNavigation::createNavButton(const NavItem&amp; item) {
    QPushButton* btn = new QPushButton(this);
    btn-&gt;setObjectName("navButton");
    btn-&gt;setCheckable(true);
    btn-&gt;setCursor(Qt::PointingHandCursor);
    
    // Create icon + text layout
    QWidget* content = new QWidget(btn);
    QHBoxLayout* layout = new QHBoxLayout(content);
    layout-&gt;setContentsMargins(12, 10, 12, 10);
    layout-&gt;setSpacing(14);
    
    // Icon label (using SVG)
    QLabel* iconLabel = new QLabel(content);
    QIcon icon(item.icon);
    iconLabel-&gt;setPixmap(icon.pixmap(20, 20));
    iconLabel-&gt;setFixedSize(24, 24);
    iconLabel-&gt;setAlignment(Qt::AlignCenter);
    iconLabel-&gt;setStyleSheet("background: transparent;");
    layout-&gt;addWidget(iconLabel);
    
    // Text label
    QLabel* textLabel = new QLabel(item.label, content);
    textLabel-&gt;setStyleSheet(
        "font-size: 12px; "
        "font-weight: 600; "
        "color: inherit; "
        "letter-spacing: 1px; "
        "background: transparent;"
    );
    layout-&gt;addWidget(textLabel);
    layout-&gt;addStretch();
    
    btn-&gt;setLayout(new QVBoxLayout());
    btn-&gt;layout()-&gt;setContentsMargins(0, 0, 0, 0);
    btn-&gt;layout()-&gt;addWidget(content);
    
    btn-&gt;setMinimumHeight(48);
    btn-&gt;setStyleSheet(
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
    
    btn-&gt;setProperty("navId", item.id);
    
    return btn;
}

void SidebarNavigation::setCurrentPage(const QString&amp; pageId) {
    m_currentPage = pageId;
    
    // Update button states
    for (int i = 0; i &lt; m_navItems.size(); ++i) {
        if (m_navItems[i].id == pageId) {
            QAbstractButton* btn = m_buttonGroup-&gt;button(i);
            if (btn) {
                btn-&gt;setChecked(true);
            }
            break;
        }
    }
}

void SidebarNavigation::setOnlineStatus(bool online) {
    if (online) {
        m_statusValue-&gt;setText("ONLINE");
        m_statusValue-&gt;setStyleSheet(
            "font-size: 12px; "
            "color: #00ff88; "
            "font-weight: 700;"
        );
    } else {
        m_statusValue-&gt;setText("OFFLINE");
        m_statusValue-&gt;setStyleSheet(
            "font-size: 12px; "
            "color: #ff3344; "
            "font-weight: 700;"
        );
    }
}

void SidebarNavigation::onNavButtonClicked(int index) {
    if (index &gt;= 0 &amp;&amp; index &lt; m_navItems.size()) {
        m_currentPage = m_navItems[index].id;
        emit pageSelected(m_currentPage);
    }
}

} // namespace CounterUAS
