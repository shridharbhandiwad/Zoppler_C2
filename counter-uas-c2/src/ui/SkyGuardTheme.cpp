#include "ui/SkyGuardTheme.h"

namespace CounterUAS {

// Color definitions
const QColor SkyGuardTheme::BackgroundDark(10, 22, 40);        // #0a1628
const QColor SkyGuardTheme::BackgroundPanel(13, 26, 45);       // #0d1a2d
const QColor SkyGuardTheme::BackgroundCard(17, 29, 46);        // #111d2e
const QColor SkyGuardTheme::BackgroundHover(26, 45, 68);       // #1a2d44

const QColor SkyGuardTheme::AccentCyan(0, 212, 255);           // #00d4ff
const QColor SkyGuardTheme::AccentCyanDark(0, 168, 204);       // #00a8cc
const QColor SkyGuardTheme::AccentCyanGlow(0, 212, 255, 64);   // #00d4ff40

const QColor SkyGuardTheme::StatusOnline(0, 255, 136);         // #00ff88
const QColor SkyGuardTheme::StatusHostile(255, 51, 68);        // #ff3344
const QColor SkyGuardTheme::StatusFriendly(0, 204, 102);       // #00cc66
const QColor SkyGuardTheme::StatusWarning(255, 170, 0);        // #ffaa00
const QColor SkyGuardTheme::StatusInfo(51, 136, 255);          // #3388ff
const QColor SkyGuardTheme::StatusProcessing(255, 136, 0);     // #ff8800

const QColor SkyGuardTheme::TextPrimary(255, 255, 255);        // #ffffff
const QColor SkyGuardTheme::TextSecondary(136, 153, 170);      // #8899aa
const QColor SkyGuardTheme::TextMuted(85, 102, 119);           // #556677
const QColor SkyGuardTheme::TextLabel(102, 119, 136);          // #667788

const QColor SkyGuardTheme::BorderPrimary(26, 51, 68);         // #1a3344
const QColor SkyGuardTheme::BorderHighlight(0, 212, 255);      // #00d4ff
const QColor SkyGuardTheme::BorderActive(0, 212, 255);         // #00d4ff

QString SkyGuardTheme::getStyleSheet() {
    return QString(R"(
        /* ===== GLOBAL STYLES ===== */
        QMainWindow {
            background-color: #0a1628;
        }
        
        QWidget {
            background-color: transparent;
            color: #ffffff;
            font-family: 'Segoe UI', 'Roboto', 'Arial', sans-serif;
            font-size: 13px;
        }
        
        QWidget#centralWidget {
            background-color: #0a1628;
        }
        
        /* ===== SCROLL BARS ===== */
        QScrollBar:vertical {
            background-color: #0d1a2d;
            width: 10px;
            border: none;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background-color: #1a3344;
            border-radius: 5px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #2a4455;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
        
        QScrollBar:horizontal {
            background-color: #0d1a2d;
            height: 10px;
            border: none;
            border-radius: 5px;
        }
        QScrollBar::handle:horizontal {
            background-color: #1a3344;
            border-radius: 5px;
            min-width: 30px;
        }
        QScrollBar::handle:horizontal:hover {
            background-color: #2a4455;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }
        
        /* ===== LABELS ===== */
        QLabel {
            color: #ffffff;
            background: transparent;
        }
        
        QLabel#headerLabel {
            font-size: 12px;
            font-weight: bold;
            color: #8899aa;
            letter-spacing: 2px;
            text-transform: uppercase;
        }
        
        QLabel#valueLabel {
            font-size: 24px;
            font-weight: bold;
            color: #00d4ff;
        }
        
        QLabel#statusLabel {
            font-size: 11px;
            color: #667788;
        }
        
        /* ===== BUTTONS ===== */
        QPushButton {
            background-color: #1a2d44;
            color: #ffffff;
            border: 1px solid #2a4455;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: #2a3d55;
            border-color: #00d4ff;
        }
        QPushButton:pressed {
            background-color: #0a1628;
        }
        QPushButton:disabled {
            background-color: #111d2e;
            color: #556677;
            border-color: #1a3344;
        }
        
        QPushButton#primaryButton {
            background-color: transparent;
            border: 2px solid #00d4ff;
            color: #00d4ff;
        }
        QPushButton#primaryButton:hover {
            background-color: rgba(0, 212, 255, 0.1);
        }
        
        QPushButton#dangerButton {
            background-color: transparent;
            border: 2px solid #ff3344;
            color: #ff3344;
        }
        QPushButton#dangerButton:hover {
            background-color: rgba(255, 51, 68, 0.1);
        }
        
        /* ===== TABLES ===== */
        QTableWidget, QTableView {
            background-color: #0d1a2d;
            alternate-background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
            gridline-color: #1a3344;
            selection-background-color: rgba(0, 212, 255, 0.2);
            selection-color: #ffffff;
        }
        
        QTableWidget::item, QTableView::item {
            padding: 12px 8px;
            border: none;
        }
        
        QTableWidget::item:selected, QTableView::item:selected {
            background-color: rgba(0, 212, 255, 0.2);
        }
        
        QHeaderView::section {
            background-color: #0d1a2d;
            color: #00d4ff;
            font-weight: bold;
            font-size: 12px;
            padding: 12px 8px;
            border: none;
            border-bottom: 2px solid #1a3344;
            text-transform: uppercase;
        }
        
        /* ===== LINE EDITS ===== */
        QLineEdit {
            background-color: #111d2e;
            color: #ffffff;
            border: 1px solid #1a3344;
            border-radius: 4px;
            padding: 8px 12px;
        }
        QLineEdit:focus {
            border-color: #00d4ff;
        }
        
        /* ===== COMBO BOXES ===== */
        QComboBox {
            background-color: #111d2e;
            color: #ffffff;
            border: 1px solid #1a3344;
            border-radius: 4px;
            padding: 8px 12px;
            min-width: 100px;
        }
        QComboBox:hover {
            border-color: #00d4ff;
        }
        QComboBox::drop-down {
            border: none;
            padding-right: 12px;
        }
        QComboBox QAbstractItemView {
            background-color: #0d1a2d;
            border: 1px solid #1a3344;
            selection-background-color: rgba(0, 212, 255, 0.2);
        }
        
        /* ===== GROUP BOXES ===== */
        QGroupBox {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 24px;
        }
        QGroupBox::title {
            color: #00d4ff;
            font-weight: bold;
            font-size: 12px;
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 4px 12px;
            background-color: #111d2e;
        }
        
        /* ===== MENUS ===== */
        QMenuBar {
            background-color: #0a1628;
            color: #8899aa;
            border-bottom: 1px solid #1a3344;
            padding: 4px;
        }
        QMenuBar::item {
            padding: 6px 12px;
            border-radius: 4px;
        }
        QMenuBar::item:selected {
            background-color: #1a2d44;
            color: #ffffff;
        }
        
        QMenu {
            background-color: #0d1a2d;
            border: 1px solid #1a3344;
            border-radius: 4px;
            padding: 4px;
        }
        QMenu::item {
            padding: 8px 24px;
            border-radius: 4px;
        }
        QMenu::item:selected {
            background-color: rgba(0, 212, 255, 0.2);
        }
        QMenu::separator {
            height: 1px;
            background-color: #1a3344;
            margin: 4px 8px;
        }
        
        /* ===== TOOL TIPS ===== */
        QToolTip {
            background-color: #1a2d44;
            color: #ffffff;
            border: 1px solid #00d4ff;
            border-radius: 4px;
            padding: 8px;
        }
        
        /* ===== PROGRESS BARS ===== */
        QProgressBar {
            background-color: #111d2e;
            border: none;
            border-radius: 4px;
            height: 8px;
            text-align: center;
        }
        QProgressBar::chunk {
            background-color: #ff3344;
            border-radius: 4px;
        }
        
        /* ===== DOCK WIDGETS ===== */
        QDockWidget {
            background-color: #0d1a2d;
            border: 1px solid #1a3344;
            titlebar-close-icon: url(close.png);
        }
        QDockWidget::title {
            background-color: #111d2e;
            color: #00d4ff;
            font-weight: bold;
            padding: 8px;
            border-bottom: 1px solid #1a3344;
        }
        
        /* ===== SPLITTERS ===== */
        QSplitter::handle {
            background-color: #1a3344;
        }
        QSplitter::handle:hover {
            background-color: #00d4ff;
        }
    )");
}

QString SkyGuardTheme::getSidebarStyleSheet() {
    return QString(R"(
        QWidget#sidebar {
            background-color: #0d1a2d;
            border-right: 1px solid #1a3344;
        }
        
        QPushButton#navButton {
            background-color: transparent;
            color: #8899aa;
            border: none;
            border-left: 3px solid transparent;
            border-radius: 0px;
            padding: 16px 20px;
            text-align: left;
            font-size: 13px;
            font-weight: 500;
        }
        QPushButton#navButton:hover {
            background-color: #1a2d44;
            color: #ffffff;
        }
        QPushButton#navButton:checked {
            background-color: rgba(0, 212, 255, 0.1);
            color: #00d4ff;
            border-left: 3px solid #00d4ff;
        }
        
        QLabel#logoLabel {
            font-size: 20px;
            font-weight: bold;
            color: #00d4ff;
            padding: 20px;
        }
        
        QLabel#subtitleLabel {
            font-size: 11px;
            color: #667788;
            padding: 0 20px 20px 20px;
            letter-spacing: 1px;
        }
        
        QLabel#statusIndicator {
            color: #00ff88;
            font-weight: bold;
        }
        
        QLabel#versionLabel {
            font-size: 10px;
            color: #556677;
        }
    )");
}

QString SkyGuardTheme::getHeaderStyleSheet() {
    return QString(R"(
        QWidget#headerBar {
            background-color: #0d1a2d;
            border-bottom: 1px solid #1a3344;
        }
        
        QWidget#headerStatWidget {
            background-color: transparent;
            border-right: 1px solid #1a3344;
            padding: 8px 24px;
        }
        
        QLabel#headerStatLabel {
            font-size: 10px;
            font-weight: bold;
            color: #667788;
            letter-spacing: 1px;
            text-transform: uppercase;
        }
        
        QLabel#headerStatValue {
            font-size: 14px;
            font-weight: bold;
            color: #00ff88;
        }
        
        QLabel#headerStatValueWarning {
            font-size: 14px;
            font-weight: bold;
            color: #ffaa00;
        }
        
        QLabel#headerStatValueDanger {
            font-size: 14px;
            font-weight: bold;
            color: #ff3344;
        }
        
        QLabel#zuluTime {
            font-size: 32px;
            font-weight: bold;
            color: #ffffff;
            font-family: 'Consolas', 'Monaco', monospace;
        }
        
        QLabel#zuluTimeLabel {
            font-size: 10px;
            color: #667788;
        }
        
        QPushButton#simulateButton {
            background-color: transparent;
            border: 2px solid #00d4ff;
            color: #00d4ff;
            border-radius: 4px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 12px;
        }
        QPushButton#simulateButton:hover {
            background-color: rgba(0, 212, 255, 0.15);
        }
    )");
}

QString SkyGuardTheme::getCardStyleSheet() {
    return QString(R"(
        QFrame#threatCard {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
            border-left: 4px solid #ff3344;
        }
        
        QFrame#friendlyCard {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
            border-left: 4px solid #00cc66;
        }
        
        QFrame#sensorCard {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
        }
        
        QFrame#alertCard {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
        }
        
        QFrame#alertCardCritical {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
            border-left: 4px solid #ff3344;
        }
        
        QFrame#alertCardInfo {
            background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
            border-left: 4px solid #3388ff;
        }
        
        QFrame#statCard {
            background-color: #111d2e;
            border: 1px solid #00d4ff;
            border-radius: 8px;
            padding: 16px;
        }
        
        QLabel#cardTitle {
            font-size: 14px;
            font-weight: bold;
            color: #ffffff;
        }
        
        QLabel#cardSubtitle {
            font-size: 11px;
            color: #667788;
        }
        
        QLabel#cardValue {
            font-size: 24px;
            font-weight: bold;
            color: #00d4ff;
        }
        
        QLabel#hostileBadge {
            background-color: #ff3344;
            color: #ffffff;
            font-size: 10px;
            font-weight: bold;
            padding: 4px 12px;
            border-radius: 10px;
        }
        
        QLabel#friendlyBadge {
            background-color: #00cc66;
            color: #ffffff;
            font-size: 10px;
            font-weight: bold;
            padding: 4px 12px;
            border-radius: 10px;
        }
        
        QLabel#onlineBadge {
            background-color: #00cc66;
            color: #0d1a2d;
            font-size: 10px;
            font-weight: bold;
            padding: 4px 12px;
            border-radius: 10px;
        }
        
        QLabel#criticalBadge {
            background-color: #ff3344;
            color: #ffffff;
            font-size: 10px;
            font-weight: bold;
            padding: 4px 12px;
            border-radius: 10px;
        }
        
        QLabel#infoBadge {
            background-color: #3388ff;
            color: #ffffff;
            font-size: 10px;
            font-weight: bold;
            padding: 4px 12px;
            border-radius: 10px;
        }
    )");
}

QString SkyGuardTheme::getTableStyleSheet() {
    return QString(R"(
        QTableWidget#threatTable {
            background-color: #0d1a2d;
            alternate-background-color: #111d2e;
            border: none;
            border-radius: 0px;
            gridline-color: transparent;
            selection-background-color: rgba(0, 212, 255, 0.15);
        }
        
        QTableWidget#threatTable::item {
            padding: 16px 12px;
            border-bottom: 1px solid #1a3344;
        }
        
        QTableWidget#threatTable QHeaderView::section {
            background-color: #0d1a2d;
            color: #00d4ff;
            font-weight: bold;
            font-size: 11px;
            padding: 16px 12px;
            border: none;
            border-bottom: 2px solid #1a3344;
        }
    )");
}

QString SkyGuardTheme::getButtonStyleSheet() {
    return QString(R"(
        QPushButton#ackButton {
            background-color: #1a2d44;
            color: #ffffff;
            border: 1px solid #2a4455;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 11px;
        }
        QPushButton#ackButton:hover {
            background-color: #2a3d55;
            border-color: #00d4ff;
        }
        
        QPushButton#addButton {
            background-color: transparent;
            border: 2px solid #00d4ff;
            color: #00d4ff;
            border-radius: 4px;
            padding: 8px 20px;
            font-weight: bold;
            font-size: 12px;
        }
        QPushButton#addButton:hover {
            background-color: rgba(0, 212, 255, 0.15);
        }
    )");
}

QString SkyGuardTheme::getStatusBarStyleSheet() {
    return QString(R"(
        QWidget#statusBar {
            background-color: #0d1a2d;
            border-top: 1px solid #1a3344;
        }
        
        QLabel#statusText {
            color: #667788;
            font-size: 12px;
            padding: 8px 16px;
        }
        
        QLabel#onlineStatus {
            color: #00ff88;
            font-weight: bold;
            font-size: 12px;
        }
        
        QLabel#versionText {
            color: #556677;
            font-size: 11px;
        }
    )");
}

} // namespace CounterUAS
