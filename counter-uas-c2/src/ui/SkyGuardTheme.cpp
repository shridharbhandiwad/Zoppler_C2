#include "ui/SkyGuardTheme.h"
#include &lt;QPalette&gt;
#include &lt;QStyleFactory&gt;

namespace CounterUAS {

// ===== BACKGROUND COLORS =====
const QColor SkyGuardTheme::BackgroundDarkest(8, 14, 26);          // #080e1a
const QColor SkyGuardTheme::BackgroundDark(10, 22, 40);            // #0a1628
const QColor SkyGuardTheme::BackgroundPanel(13, 26, 45);           // #0d1a2d
const QColor SkyGuardTheme::BackgroundCard(17, 29, 46);            // #111d2e
const QColor SkyGuardTheme::BackgroundHover(26, 45, 68);           // #1a2d44
const QColor SkyGuardTheme::BackgroundActive(36, 58, 82);          // #243a52
const QColor SkyGuardTheme::BackgroundElevated(30, 49, 72);        // #1e3148

// ===== PRIMARY ACCENT =====
const QColor SkyGuardTheme::AccentPrimary(0, 212, 255);            // #00d4ff
const QColor SkyGuardTheme::AccentPrimaryDark(0, 168, 204);        // #00a8cc
const QColor SkyGuardTheme::AccentPrimaryLight(102, 229, 255);     // #66e5ff
const QColor SkyGuardTheme::AccentPrimaryGlow(0, 212, 255, 64);    // #00d4ff40
const QColor SkyGuardTheme::AccentPrimaryMuted(0, 136, 170);       // #0088aa

// ===== SECONDARY ACCENT =====
const QColor SkyGuardTheme::AccentSecondary(124, 77, 255);         // #7c4dff
const QColor SkyGuardTheme::AccentSecondaryDark(90, 61, 204);      // #5a3dcc

// ===== STATUS COLORS =====
const QColor SkyGuardTheme::StatusOnline(0, 255, 136);             // #00ff88
const QColor SkyGuardTheme::StatusOnlineDark(0, 204, 106);         // #00cc6a
const QColor SkyGuardTheme::StatusHostile(255, 51, 68);            // #ff3344
const QColor SkyGuardTheme::StatusHostileDark(204, 41, 55);        // #cc2937
const QColor SkyGuardTheme::StatusFriendly(0, 204, 102);           // #00cc66
const QColor SkyGuardTheme::StatusWarning(255, 170, 0);            // #ffaa00
const QColor SkyGuardTheme::StatusWarningDark(204, 136, 0);        // #cc8800
const QColor SkyGuardTheme::StatusInfo(51, 136, 255);              // #3388ff
const QColor SkyGuardTheme::StatusInfoDark(40, 102, 204);          // #2866cc
const QColor SkyGuardTheme::StatusProcessing(255, 136, 0);         // #ff8800
const QColor SkyGuardTheme::StatusNeutral(102, 136, 170);          // #6688aa

// ===== TEXT COLORS =====
const QColor SkyGuardTheme::TextPrimary(255, 255, 255);            // #ffffff
const QColor SkyGuardTheme::TextSecondary(168, 184, 200);          // #a8b8c8
const QColor SkyGuardTheme::TextMuted(102, 119, 136);              // #667788
const QColor SkyGuardTheme::TextLabel(136, 154, 171);              // #889aab
const QColor SkyGuardTheme::TextDark(13, 26, 45);                  // #0d1a2d

// ===== BORDER COLORS =====
const QColor SkyGuardTheme::BorderPrimary(26, 51, 68);             // #1a3344
const QColor SkyGuardTheme::BorderSecondary(42, 68, 85);           // #2a4455
const QColor SkyGuardTheme::BorderHighlight(0, 212, 255);          // #00d4ff
const QColor SkyGuardTheme::BorderActive(0, 212, 255);             // #00d4ff
const QColor SkyGuardTheme::BorderSubtle(21, 37, 53);              // #152535

// ===== GRADIENT COLORS =====
const QColor SkyGuardTheme::GradientStart(13, 26, 45);             // #0d1a2d
const QColor SkyGuardTheme::GradientEnd(10, 20, 37);               // #0a1425

// ===== SHADOW COLORS =====
const QColor SkyGuardTheme::ShadowDark(0, 0, 0, 102);              // rgba(0,0,0,0.4)
const QColor SkyGuardTheme::ShadowMedium(0, 0, 0, 64);             // rgba(0,0,0,0.25)
const QColor SkyGuardTheme::ShadowLight(0, 0, 0, 38);              // rgba(0,0,0,0.15)
const QColor SkyGuardTheme::ShadowGlow(0, 212, 255, 51);           // rgba(0,212,255,0.2)

// ===== FONT METHODS =====
QString SkyGuardTheme::primaryFontFamily() {
    return "'Segoe UI', 'SF Pro Display', 'Roboto', 'Helvetica Neue', sans-serif";
}

QString SkyGuardTheme::monospaceFontFamily() {
    return "'JetBrains Mono', 'Cascadia Code', 'Consolas', 'Monaco', monospace";
}

int SkyGuardTheme::fontSizeXS()    { return 10; }
int SkyGuardTheme::fontSizeS()     { return 11; }
int SkyGuardTheme::fontSizeM()     { return 13; }
int SkyGuardTheme::fontSizeL()     { return 14; }
int SkyGuardTheme::fontSizeXL()    { return 16; }
int SkyGuardTheme::fontSizeXXL()   { return 20; }
int SkyGuardTheme::fontSizeTitle() { return 28; }
int SkyGuardTheme::fontSizeHuge()  { return 36; }

// ===== UTILITY METHODS =====
QString SkyGuardTheme::toHex(const QColor&amp; color) {
    return color.name(QColor::HexRgb);
}

QString SkyGuardTheme::toRgba(const QColor&amp; color) {
    return QString("rgba(%1, %2, %3, %4)")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue())
        .arg(color.alpha() / 255.0, 0, 'f', 2);
}

QString SkyGuardTheme::iconPath(const QString&amp; iconName) {
    return QString(":/icons/%1.svg").arg(iconName);
}

void SkyGuardTheme::applyTheme(QApplication* app) {
    if (!app) return;
    
    // Use Fusion style as base
    app-&gt;setStyle(QStyleFactory::create("Fusion"));
    
    // Apply dark palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, BackgroundDark);
    darkPalette.setColor(QPalette::WindowText, TextPrimary);
    darkPalette.setColor(QPalette::Base, BackgroundPanel);
    darkPalette.setColor(QPalette::AlternateBase, BackgroundCard);
    darkPalette.setColor(QPalette::ToolTipBase, BackgroundHover);
    darkPalette.setColor(QPalette::ToolTipText, TextPrimary);
    darkPalette.setColor(QPalette::Text, TextPrimary);
    darkPalette.setColor(QPalette::Button, BackgroundHover);
    darkPalette.setColor(QPalette::ButtonText, TextPrimary);
    darkPalette.setColor(QPalette::BrightText, AccentPrimary);
    darkPalette.setColor(QPalette::Link, AccentPrimary);
    darkPalette.setColor(QPalette::Highlight, AccentPrimary);
    darkPalette.setColor(QPalette::HighlightedText, BackgroundDark);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, TextMuted);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, TextMuted);
    app-&gt;setPalette(darkPalette);
}

QString SkyGuardTheme::getStyleSheet() {
    return QString(R"(
        /* ================================================================
           SKYGUARD THEME - Global Application Stylesheet
           Modern Defense-Grade UI with Cyan/Teal Accents
           ================================================================ */
        
        /* ===== GLOBAL RESET &amp; BASE STYLES ===== */
        * {
            outline: none;
        }
        
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0d1a2d, stop:1 #080e1a);
        }
        
        QWidget {
            background-color: transparent;
            color: #ffffff;
            font-family: 'Segoe UI', 'SF Pro Display', 'Roboto', sans-serif;
            font-size: 13px;
            selection-background-color: rgba(0, 212, 255, 0.3);
            selection-color: #ffffff;
        }
        
        QWidget#centralWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a1628, stop:1 #080e1a);
        }
        
        /* ===== SCROLLBARS ===== */
        QScrollBar:vertical {
            background-color: #0d1a2d;
            width: 12px;
            border: none;
            border-radius: 6px;
            margin: 4px 2px;
        }
        
        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #2a4455, stop:1 #1a3344);
            border-radius: 5px;
            min-height: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3a5566, stop:1 #2a4455);
        }
        
        QScrollBar::handle:vertical:pressed {
            background: #00d4ff;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: transparent;
        }
        
        QScrollBar:horizontal {
            background-color: #0d1a2d;
            height: 12px;
            border: none;
            border-radius: 6px;
            margin: 2px 4px;
        }
        
        QScrollBar::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2a4455, stop:1 #1a3344);
            border-radius: 5px;
            min-width: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3a5566, stop:1 #2a4455);
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }
        
        /* ===== LABELS ===== */
        QLabel {
            color: #ffffff;
            background: transparent;
            padding: 0px;
        }
        
        QLabel#headerLabel {
            font-size: 11px;
            font-weight: 600;
            color: #889aab;
            letter-spacing: 1.5px;
        }
        
        QLabel#valueLabel {
            font-size: 24px;
            font-weight: 700;
            color: #00d4ff;
        }
        
        QLabel#statusLabel {
            font-size: 11px;
            color: #667788;
        }
        
        QLabel#titleLabel {
            font-size: 28px;
            font-weight: 700;
            color: #ffffff;
            letter-spacing: 2px;
        }
        
        QLabel#subtitleLabel {
            font-size: 13px;
            color: #667788;
        }
        
        /* ===== BUTTONS ===== */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e3148, stop:1 #1a2d44);
            color: #ffffff;
            border: 1px solid #2a4455;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: 600;
            font-size: 12px;
            min-height: 18px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2a3d55, stop:1 #1e3148);
            border-color: #00d4ff;
            color: #00d4ff;
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a1628, stop:1 #0d1a2d);
            border-color: #00a8cc;
        }
        
        QPushButton:disabled {
            background: #111d2e;
            color: #556677;
            border-color: #1a3344;
        }
        
        QPushButton:focus {
            border-color: #00d4ff;
            outline: none;
        }
        
        QPushButton#primaryButton {
            background: transparent;
            border: 2px solid #00d4ff;
            color: #00d4ff;
            font-weight: 700;
        }
        
        QPushButton#primaryButton:hover {
            background: rgba(0, 212, 255, 0.15);
            border-color: #66e5ff;
            color: #66e5ff;
        }
        
        QPushButton#primaryButton:pressed {
            background: rgba(0, 212, 255, 0.25);
        }
        
        QPushButton#accentButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00d4ff, stop:1 #00a8cc);
            border: none;
            color: #0d1a2d;
            font-weight: 700;
        }
        
        QPushButton#accentButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #66e5ff, stop:1 #00d4ff);
        }
        
        QPushButton#accentButton:pressed {
            background: #0088aa;
        }
        
        QPushButton#dangerButton {
            background: transparent;
            border: 2px solid #ff3344;
            color: #ff3344;
        }
        
        QPushButton#dangerButton:hover {
            background: rgba(255, 51, 68, 0.15);
            border-color: #ff6677;
            color: #ff6677;
        }
        
        QPushButton#successButton {
            background: transparent;
            border: 2px solid #00ff88;
            color: #00ff88;
        }
        
        QPushButton#successButton:hover {
            background: rgba(0, 255, 136, 0.15);
        }
        
        QPushButton#flatButton {
            background: transparent;
            border: none;
            color: #a8b8c8;
            padding: 8px 12px;
        }
        
        QPushButton#flatButton:hover {
            color: #00d4ff;
            background: rgba(0, 212, 255, 0.1);
        }
        
        QPushButton#iconButton {
            background: transparent;
            border: none;
            padding: 8px;
            border-radius: 6px;
        }
        
        QPushButton#iconButton:hover {
            background: rgba(0, 212, 255, 0.15);
        }
        
        /* ===== TABLES ===== */
        QTableWidget, QTableView, QTreeView, QListView {
            background-color: #0d1a2d;
            alternate-background-color: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 8px;
            gridline-color: #152535;
            selection-background-color: rgba(0, 212, 255, 0.2);
            selection-color: #ffffff;
            outline: none;
        }
        
        QTableWidget::item, QTableView::item, QTreeView::item, QListView::item {
            padding: 12px 10px;
            border: none;
            border-bottom: 1px solid #152535;
        }
        
        QTableWidget::item:selected, QTableView::item:selected,
        QTreeView::item:selected, QListView::item:selected {
            background-color: rgba(0, 212, 255, 0.2);
            border-left: 3px solid #00d4ff;
        }
        
        QTableWidget::item:hover, QTableView::item:hover {
            background-color: rgba(0, 212, 255, 0.1);
        }
        
        QHeaderView {
            background-color: transparent;
        }
        
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            color: #00d4ff;
            font-weight: 700;
            font-size: 11px;
            padding: 14px 10px;
            border: none;
            border-bottom: 2px solid #1a3344;
            border-right: 1px solid #152535;
            letter-spacing: 1px;
        }
        
        QHeaderView::section:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #111d2e);
        }
        
        QHeaderView::section:last {
            border-right: none;
        }
        
        /* ===== LINE EDITS ===== */
        QLineEdit {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a1628, stop:1 #0d1a2d);
            color: #ffffff;
            border: 1px solid #1a3344;
            border-radius: 6px;
            padding: 10px 14px;
            font-size: 13px;
            selection-background-color: rgba(0, 212, 255, 0.3);
        }
        
        QLineEdit:hover {
            border-color: #2a4455;
        }
        
        QLineEdit:focus {
            border-color: #00d4ff;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0d1a2d, stop:1 #111d2e);
        }
        
        QLineEdit:disabled {
            background: #0a1628;
            color: #556677;
            border-color: #152535;
        }
        
        QLineEdit[readOnly="true"] {
            background: #0a1628;
            border-color: #152535;
        }
        
        /* ===== TEXT EDIT ===== */
        QTextEdit, QPlainTextEdit {
            background-color: #0d1a2d;
            color: #ffffff;
            border: 1px solid #1a3344;
            border-radius: 6px;
            padding: 10px;
            selection-background-color: rgba(0, 212, 255, 0.3);
        }
        
        QTextEdit:focus, QPlainTextEdit:focus {
            border-color: #00d4ff;
        }
        
        /* ===== COMBO BOXES ===== */
        QComboBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #111d2e);
            color: #ffffff;
            border: 1px solid #1a3344;
            border-radius: 6px;
            padding: 10px 14px;
            padding-right: 32px;
            min-width: 120px;
            font-size: 13px;
        }
        
        QComboBox:hover {
            border-color: #2a4455;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e3148, stop:1 #1a2d44);
        }
        
        QComboBox:focus {
            border-color: #00d4ff;
        }
        
        QComboBox:on {
            border-color: #00d4ff;
            border-bottom-left-radius: 0;
            border-bottom-right-radius: 0;
        }
        
        QComboBox::drop-down {
            border: none;
            width: 28px;
            padding-right: 8px;
        }
        
        QComboBox::down-arrow {
            image: url(:/icons/chevron-down.svg);
            width: 12px;
            height: 12px;
        }
        
        QComboBox QAbstractItemView {
            background-color: #111d2e;
            border: 1px solid #00d4ff;
            border-top: none;
            border-radius: 0 0 6px 6px;
            selection-background-color: rgba(0, 212, 255, 0.25);
            outline: none;
            padding: 4px;
        }
        
        QComboBox QAbstractItemView::item {
            padding: 10px 14px;
            border-radius: 4px;
            margin: 2px;
        }
        
        QComboBox QAbstractItemView::item:hover {
            background-color: rgba(0, 212, 255, 0.15);
        }
        
        /* ===== SPIN BOXES ===== */
        QSpinBox, QDoubleSpinBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a1628, stop:1 #0d1a2d);
            color: #ffffff;
            border: 1px solid #1a3344;
            border-radius: 6px;
            padding: 8px 12px;
            padding-right: 24px;
            font-size: 13px;
        }
        
        QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #2a4455;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #00d4ff;
        }
        
        QSpinBox::up-button, QDoubleSpinBox::up-button {
            subcontrol-origin: border;
            subcontrol-position: top right;
            width: 22px;
            border: none;
            border-left: 1px solid #1a3344;
            border-top-right-radius: 5px;
            background: transparent;
        }
        
        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: rgba(0, 212, 255, 0.15);
        }
        
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            subcontrol-origin: border;
            subcontrol-position: bottom right;
            width: 22px;
            border: none;
            border-left: 1px solid #1a3344;
            border-bottom-right-radius: 5px;
            background: transparent;
        }
        
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: rgba(0, 212, 255, 0.15);
        }
        
        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            image: url(:/icons/chevron-up.svg);
            width: 10px;
            height: 10px;
        }
        
        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: url(:/icons/chevron-down.svg);
            width: 10px;
            height: 10px;
        }
        
        /* ===== CHECKBOXES ===== */
        QCheckBox {
            color: #ffffff;
            spacing: 10px;
            font-size: 13px;
        }
        
        QCheckBox::indicator {
            width: 20px;
            height: 20px;
            border-radius: 4px;
            border: 2px solid #2a4455;
            background: #0d1a2d;
        }
        
        QCheckBox::indicator:hover {
            border-color: #00d4ff;
            background: #111d2e;
        }
        
        QCheckBox::indicator:checked {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00d4ff, stop:1 #00a8cc);
            border-color: #00d4ff;
            image: url(:/icons/check.svg);
        }
        
        QCheckBox::indicator:checked:hover {
            background: #66e5ff;
        }
        
        QCheckBox:disabled {
            color: #556677;
        }
        
        QCheckBox::indicator:disabled {
            border-color: #1a3344;
            background: #0a1628;
        }
        
        /* ===== RADIO BUTTONS ===== */
        QRadioButton {
            color: #ffffff;
            spacing: 10px;
            font-size: 13px;
        }
        
        QRadioButton::indicator {
            width: 20px;
            height: 20px;
            border-radius: 10px;
            border: 2px solid #2a4455;
            background: #0d1a2d;
        }
        
        QRadioButton::indicator:hover {
            border-color: #00d4ff;
            background: #111d2e;
        }
        
        QRadioButton::indicator:checked {
            background: #0d1a2d;
            border-color: #00d4ff;
        }
        
        QRadioButton::indicator:checked::before {
            content: "";
            display: block;
            width: 10px;
            height: 10px;
            border-radius: 5px;
            background: #00d4ff;
        }
        
        /* ===== GROUP BOXES ===== */
        QGroupBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            border: 1px solid #1a3344;
            border-radius: 8px;
            margin-top: 20px;
            padding: 20px 16px 16px 16px;
            font-weight: 600;
        }
        
        QGroupBox::title {
            color: #00d4ff;
            font-weight: 700;
            font-size: 12px;
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 6px 14px;
            margin-left: 12px;
            background: #111d2e;
            border: 1px solid #1a3344;
            border-radius: 4px;
            letter-spacing: 1px;
        }
        
        /* ===== MENUS ===== */
        QMenuBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e3148, stop:1 #1a2d44);
            color: #ffffff;
            border-bottom: 1px solid #2a4455;
            padding: 4px 8px;
            font-size: 13px;
        }
        
        QMenuBar::item {
            padding: 8px 14px;
            border-radius: 4px;
            color: #ffffff;
            margin: 2px;
        }
        
        QMenuBar::item:selected {
            background: rgba(0, 212, 255, 0.2);
            color: #00d4ff;
        }
        
        QMenuBar::item:pressed {
            background: rgba(0, 212, 255, 0.3);
        }
        
        QMenu {
            background-color: #1a2d44;
            color: #ffffff;
            border: 1px solid #2a4455;
            border-radius: 8px;
            padding: 8px;
        }
        
        QMenu::item {
            padding: 10px 32px 10px 16px;
            border-radius: 4px;
            color: #ffffff;
            margin: 2px;
        }
        
        QMenu::item:selected {
            background: rgba(0, 212, 255, 0.25);
            color: #00d4ff;
        }
        
        QMenu::item:disabled {
            color: #556677;
        }
        
        QMenu::separator {
            height: 1px;
            background: #2a4455;
            margin: 8px 12px;
        }
        
        QMenu::indicator {
            width: 18px;
            height: 18px;
            margin-left: 8px;
        }
        
        /* ===== TOOL TIPS ===== */
        QToolTip {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e3148, stop:1 #1a2d44);
            color: #ffffff;
            border: 1px solid #00d4ff;
            border-radius: 6px;
            padding: 10px 14px;
            font-size: 12px;
        }
        
        /* ===== PROGRESS BARS ===== */
        QProgressBar {
            background-color: #0d1a2d;
            border: 1px solid #1a3344;
            border-radius: 6px;
            height: 12px;
            text-align: center;
            font-size: 10px;
            color: #ffffff;
        }
        
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00d4ff, stop:1 #00a8cc);
            border-radius: 5px;
            margin: 1px;
        }
        
        QProgressBar[threat="true"]::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ff3344, stop:1 #cc2937);
        }
        
        QProgressBar[success="true"]::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00ff88, stop:1 #00cc6a);
        }
        
        QProgressBar[warning="true"]::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ffaa00, stop:1 #cc8800);
        }
        
        /* ===== SLIDERS ===== */
        QSlider::groove:horizontal {
            border: none;
            height: 6px;
            background: #1a3344;
            border-radius: 3px;
        }
        
        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00d4ff, stop:1 #00a8cc);
            border: none;
            width: 18px;
            height: 18px;
            margin: -6px 0;
            border-radius: 9px;
        }
        
        QSlider::handle:horizontal:hover {
            background: #66e5ff;
        }
        
        QSlider::sub-page:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00d4ff, stop:1 #0088aa);
            border-radius: 3px;
        }
        
        QSlider::groove:vertical {
            border: none;
            width: 6px;
            background: #1a3344;
            border-radius: 3px;
        }
        
        QSlider::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00d4ff, stop:1 #00a8cc);
            border: none;
            width: 18px;
            height: 18px;
            margin: 0 -6px;
            border-radius: 9px;
        }
        
        /* ===== TAB WIDGETS ===== */
        QTabWidget::pane {
            background: #0d1a2d;
            border: 1px solid #1a3344;
            border-radius: 0 0 8px 8px;
            border-top: none;
            padding: 16px;
        }
        
        QTabBar::tab {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            color: #889aab;
            border: 1px solid #1a3344;
            border-bottom: none;
            padding: 12px 24px;
            margin-right: 2px;
            border-radius: 6px 6px 0 0;
            font-weight: 600;
            font-size: 12px;
        }
        
        QTabBar::tab:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #111d2e);
            color: #ffffff;
        }
        
        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #0d1a2d);
            color: #00d4ff;
            border-color: #00d4ff;
            border-bottom: 2px solid #0d1a2d;
        }
        
        /* ===== DOCK WIDGETS ===== */
        QDockWidget {
            background-color: #0d1a2d;
            border: 1px solid #1a3344;
            titlebar-close-icon: url(:/icons/close.svg);
            titlebar-normal-icon: url(:/icons/float.svg);
        }
        
        QDockWidget::title {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #111d2e);
            color: #00d4ff;
            font-weight: 700;
            font-size: 12px;
            padding: 10px 16px;
            border-bottom: 1px solid #1a3344;
            letter-spacing: 1px;
        }
        
        QDockWidget::close-button, QDockWidget::float-button {
            border: none;
            background: transparent;
            padding: 4px;
            icon-size: 14px;
        }
        
        QDockWidget::close-button:hover, QDockWidget::float-button:hover {
            background: rgba(0, 212, 255, 0.2);
            border-radius: 4px;
        }
        
        /* ===== SPLITTERS ===== */
        QSplitter::handle {
            background: #1a3344;
        }
        
        QSplitter::handle:horizontal {
            width: 2px;
        }
        
        QSplitter::handle:vertical {
            height: 2px;
        }
        
        QSplitter::handle:hover {
            background: #00d4ff;
        }
        
        /* ===== FRAMES ===== */
        QFrame#cardFrame {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            border: 1px solid #1a3344;
            border-radius: 8px;
        }
        
        QFrame#cardFrame:hover {
            border-color: #2a4455;
        }
        
        QFrame#panelFrame {
            background-color: #0d1a2d;
            border: 1px solid #1a3344;
            border-radius: 8px;
        }
        
        QFrame#divider {
            background-color: #1a3344;
            max-height: 1px;
        }
        
        /* ===== DIALOGS ===== */
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0d1a2d, stop:1 #0a1628);
            border: 1px solid #1a3344;
            border-radius: 12px;
        }
        
        QDialogButtonBox {
            button-layout: 3;
        }
        
        QDialogButtonBox QPushButton {
            min-width: 90px;
        }
        
        /* ===== MESSAGE BOXES ===== */
        QMessageBox {
            background-color: #0d1a2d;
        }
        
        QMessageBox QLabel {
            color: #ffffff;
            font-size: 13px;
        }
        
        /* ===== TOOL BARS ===== */
        QToolBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #111d2e);
            border: none;
            border-bottom: 1px solid #1a3344;
            spacing: 8px;
            padding: 6px 12px;
        }
        
        QToolBar::separator {
            background: #2a4455;
            width: 1px;
            margin: 4px 8px;
        }
        
        QToolButton {
            background: transparent;
            border: none;
            border-radius: 6px;
            padding: 8px;
            color: #ffffff;
        }
        
        QToolButton:hover {
            background: rgba(0, 212, 255, 0.15);
            color: #00d4ff;
        }
        
        QToolButton:pressed {
            background: rgba(0, 212, 255, 0.25);
        }
        
        QToolButton:checked {
            background: rgba(0, 212, 255, 0.2);
            color: #00d4ff;
        }
        
        /* ===== STATUS BAR ===== */
        QStatusBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            border-top: 1px solid #1a3344;
            color: #889aab;
            font-size: 12px;
        }
        
        QStatusBar::item {
            border: none;
        }
        
        /* ===== CALENDAR ===== */
        QCalendarWidget {
            background-color: #0d1a2d;
        }
        
        QCalendarWidget QToolButton {
            color: #ffffff;
            background: transparent;
            border: none;
            border-radius: 4px;
            padding: 6px;
        }
        
        QCalendarWidget QToolButton:hover {
            background: rgba(0, 212, 255, 0.15);
        }
        
        QCalendarWidget QMenu {
            background-color: #1a2d44;
        }
        
        QCalendarWidget QSpinBox {
            background: transparent;
            border: none;
            color: #ffffff;
        }
        
        QCalendarWidget QAbstractItemView:enabled {
            background-color: #0d1a2d;
            color: #ffffff;
            selection-background-color: #00d4ff;
            selection-color: #0d1a2d;
        }
    )");
}

QString SkyGuardTheme::getSidebarStyleSheet() {
    return QString(R"(
        QWidget#sidebar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #1a2d44, stop:1 #162538);
            border-right: 1px solid #2a4455;
        }
        
        QPushButton#navButton {
            background-color: transparent;
            color: #a8b8c8;
            border: none;
            border-left: 3px solid transparent;
            border-radius: 0px;
            padding: 16px 20px;
            text-align: left;
            font-size: 13px;
            font-weight: 500;
        }
        
        QPushButton#navButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 212, 255, 0.15), stop:1 rgba(0, 212, 255, 0.05));
            color: #00d4ff;
        }
        
        QPushButton#navButton:checked {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(0, 212, 255, 0.2), stop:1 rgba(0, 212, 255, 0.05));
            color: #00d4ff;
            border-left: 3px solid #00d4ff;
        }
        
        QLabel#logoLabel {
            font-size: 22px;
            font-weight: 700;
            color: #00d4ff;
            letter-spacing: 3px;
        }
        
        QLabel#subtitleLabel {
            font-size: 10px;
            color: #889aab;
            padding: 0 20px 20px 20px;
            letter-spacing: 1.5px;
        }
        
        QLabel#statusIndicator {
            color: #00ff88;
            font-weight: 700;
            font-size: 12px;
        }
        
        QLabel#versionLabel {
            font-size: 10px;
            color: #667788;
        }
        
        QFrame#sidebarDivider {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 transparent, stop:0.5 #2a4455, stop:1 transparent);
            max-height: 1px;
            margin: 12px 16px;
        }
    )");
}

QString SkyGuardTheme::getHeaderStyleSheet() {
    return QString(R"(
        QWidget#headerBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            border-bottom: 1px solid #1a3344;
        }
        
        QWidget#headerStatWidget {
            background-color: transparent;
            border-right: 1px solid #1a3344;
            padding: 8px 32px;
        }
        
        QLabel#headerStatLabel {
            font-size: 10px;
            font-weight: 700;
            color: #667788;
            letter-spacing: 1.5px;
        }
        
        QLabel#headerStatValue {
            font-size: 15px;
            font-weight: 700;
            color: #00ff88;
        }
        
        QLabel#headerStatValueWarning {
            font-size: 15px;
            font-weight: 700;
            color: #ffaa00;
        }
        
        QLabel#headerStatValueDanger {
            font-size: 15px;
            font-weight: 700;
            color: #ff3344;
        }
        
        QLabel#zuluTime {
            font-size: 36px;
            font-weight: 700;
            color: #ffffff;
            font-family: 'JetBrains Mono', 'Cascadia Code', 'Consolas', monospace;
        }
        
        QLabel#zuluTimeLabel {
            font-size: 10px;
            color: #667788;
            letter-spacing: 1.5px;
        }
        
        QPushButton#simulateButton {
            background: transparent;
            border: 2px solid #00d4ff;
            color: #00d4ff;
            border-radius: 6px;
            padding: 12px 24px;
            font-weight: 700;
            font-size: 12px;
            letter-spacing: 1px;
        }
        
        QPushButton#simulateButton:hover {
            background: rgba(0, 212, 255, 0.15);
            border-color: #66e5ff;
            color: #66e5ff;
        }
        
        QPushButton#simulateButton:pressed {
            background: rgba(0, 212, 255, 0.25);
        }
    )");
}

QString SkyGuardTheme::getCardStyleSheet() {
    return QString(R"(
        QFrame#threatCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #141f32, stop:1 #111d2e);
            border: 1px solid #1a3344;
            border-radius: 10px;
            border-left: 4px solid #ff3344;
        }
        
        QFrame#threatCard:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #162538);
            border-color: #2a4455;
        }
        
        QFrame#friendlyCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #141f32, stop:1 #111d2e);
            border: 1px solid #1a3344;
            border-radius: 10px;
            border-left: 4px solid #00cc66;
        }
        
        QFrame#sensorCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #141f32, stop:1 #111d2e);
            border: 1px solid #1a3344;
            border-radius: 10px;
        }
        
        QFrame#sensorCard:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #162538);
            border-color: #00d4ff;
        }
        
        QFrame#alertCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #141f32, stop:1 #111d2e);
            border: 1px solid #1a3344;
            border-radius: 8px;
        }
        
        QFrame#alertCardCritical {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1520, stop:1 #111d2e);
            border: 1px solid #1a3344;
            border-radius: 8px;
            border-left: 4px solid #ff3344;
        }
        
        QFrame#alertCardInfo {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #141f32, stop:1 #111d2e);
            border: 1px solid #1a3344;
            border-radius: 8px;
            border-left: 4px solid #3388ff;
        }
        
        QFrame#statCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #141f32, stop:1 #111d2e);
            border: 1px solid #00d4ff;
            border-radius: 10px;
            padding: 16px;
        }
        
        QLabel#cardTitle {
            font-size: 14px;
            font-weight: 700;
            color: #ffffff;
        }
        
        QLabel#cardSubtitle {
            font-size: 11px;
            color: #667788;
        }
        
        QLabel#cardValue {
            font-size: 28px;
            font-weight: 700;
            color: #00d4ff;
        }
        
        QLabel#hostileBadge {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff4455, stop:1 #ff3344);
            color: #ffffff;
            font-size: 10px;
            font-weight: 700;
            padding: 5px 14px;
            border-radius: 12px;
        }
        
        QLabel#friendlyBadge {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00dd77, stop:1 #00cc66);
            color: #ffffff;
            font-size: 10px;
            font-weight: 700;
            padding: 5px 14px;
            border-radius: 12px;
        }
        
        QLabel#onlineBadge {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #00dd77, stop:1 #00cc66);
            color: #0d1a2d;
            font-size: 10px;
            font-weight: 700;
            padding: 5px 14px;
            border-radius: 12px;
        }
        
        QLabel#criticalBadge {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff4455, stop:1 #ff3344);
            color: #ffffff;
            font-size: 10px;
            font-weight: 700;
            padding: 5px 14px;
            border-radius: 12px;
        }
        
        QLabel#infoBadge {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #4499ff, stop:1 #3388ff);
            color: #ffffff;
            font-size: 10px;
            font-weight: 700;
            padding: 5px 14px;
            border-radius: 12px;
        }
        
        QLabel#warningBadge {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffbb11, stop:1 #ffaa00);
            color: #0d1a2d;
            font-size: 10px;
            font-weight: 700;
            padding: 5px 14px;
            border-radius: 12px;
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
            padding: 16px 14px;
            border-bottom: 1px solid #152535;
        }
        
        QTableWidget#threatTable::item:hover {
            background-color: rgba(0, 212, 255, 0.08);
        }
        
        QTableWidget#threatTable::item:selected {
            background-color: rgba(0, 212, 255, 0.2);
            border-left: 3px solid #00d4ff;
        }
        
        QTableWidget#threatTable QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            color: #00d4ff;
            font-weight: 700;
            font-size: 11px;
            padding: 16px 14px;
            border: none;
            border-bottom: 2px solid #1a3344;
            letter-spacing: 1px;
        }
    )");
}

QString SkyGuardTheme::getButtonStyleSheet() {
    return QString(R"(
        QPushButton#ackButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e3148, stop:1 #1a2d44);
            color: #ffffff;
            border: 1px solid #2a4455;
            border-radius: 6px;
            padding: 10px 18px;
            font-size: 11px;
            font-weight: 600;
        }
        
        QPushButton#ackButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2a3d55, stop:1 #1e3148);
            border-color: #00d4ff;
            color: #00d4ff;
        }
        
        QPushButton#addButton {
            background: transparent;
            border: 2px solid #00d4ff;
            color: #00d4ff;
            border-radius: 6px;
            padding: 10px 22px;
            font-weight: 700;
            font-size: 12px;
        }
        
        QPushButton#addButton:hover {
            background: rgba(0, 212, 255, 0.15);
            border-color: #66e5ff;
        }
        
        QPushButton#engageButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff4455, stop:1 #ff3344);
            border: none;
            color: #ffffff;
            border-radius: 6px;
            padding: 12px 24px;
            font-weight: 700;
            font-size: 12px;
        }
        
        QPushButton#engageButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff6677, stop:1 #ff4455);
        }
        
        QPushButton#engageButton:pressed {
            background: #cc2937;
        }
    )");
}

QString SkyGuardTheme::getStatusBarStyleSheet() {
    return QString(R"(
        QWidget#statusBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            border-top: 1px solid #1a3344;
        }
        
        QLabel#statusText {
            color: #667788;
            font-size: 12px;
            padding: 8px 16px;
        }
        
        QLabel#onlineStatus {
            color: #00ff88;
            font-weight: 700;
            font-size: 12px;
        }
        
        QLabel#offlineStatus {
            color: #ff3344;
            font-weight: 700;
            font-size: 12px;
        }
        
        QLabel#versionText {
            color: #556677;
            font-size: 11px;
        }
    )");
}

QString SkyGuardTheme::getDialogStyleSheet() {
    return QString(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0d1a2d, stop:1 #0a1628);
        }
        
        QDialog QLabel#dialogTitle {
            font-size: 20px;
            font-weight: 700;
            color: #ffffff;
            letter-spacing: 1px;
        }
        
        QDialog QLabel#dialogSubtitle {
            font-size: 13px;
            color: #889aab;
        }
        
        QDialog QFrame#dialogHeader {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #111d2e);
            border-bottom: 1px solid #1a3344;
            padding: 20px 24px;
        }
        
        QDialog QFrame#dialogContent {
            padding: 24px;
        }
        
        QDialog QFrame#dialogFooter {
            background: #0d1a2d;
            border-top: 1px solid #1a3344;
            padding: 16px 24px;
        }
    )");
}

QString SkyGuardTheme::getTabWidgetStyleSheet() {
    return QString(R"(
        QTabWidget::pane {
            background: #0d1a2d;
            border: 1px solid #1a3344;
            border-radius: 0 0 10px 10px;
            border-top: none;
            padding: 20px;
        }
        
        QTabBar {
            background: transparent;
        }
        
        QTabBar::tab {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #111d2e, stop:1 #0d1a2d);
            color: #889aab;
            border: 1px solid #1a3344;
            border-bottom: none;
            padding: 14px 28px;
            margin-right: 4px;
            border-radius: 8px 8px 0 0;
            font-weight: 600;
            font-size: 12px;
            letter-spacing: 0.5px;
        }
        
        QTabBar::tab:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #111d2e);
            color: #ffffff;
        }
        
        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #0d1a2d);
            color: #00d4ff;
            border-color: #00d4ff;
            border-bottom: 2px solid #0d1a2d;
        }
    )");
}

QString SkyGuardTheme::getInputStyleSheet() {
    return QString(R"(
        QLineEdit, QTextEdit, QPlainTextEdit {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a1628, stop:1 #0d1a2d);
            color: #ffffff;
            border: 1px solid #1a3344;
            border-radius: 6px;
            padding: 10px 14px;
            font-size: 13px;
            selection-background-color: rgba(0, 212, 255, 0.3);
        }
        
        QLineEdit:hover, QTextEdit:hover, QPlainTextEdit:hover {
            border-color: #2a4455;
        }
        
        QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
            border-color: #00d4ff;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0d1a2d, stop:1 #111d2e);
        }
        
        QSpinBox, QDoubleSpinBox, QComboBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a2d44, stop:1 #111d2e);
            color: #ffffff;
            border: 1px solid #1a3344;
            border-radius: 6px;
            padding: 10px 14px;
            font-size: 13px;
        }
        
        QSpinBox:hover, QDoubleSpinBox:hover, QComboBox:hover {
            border-color: #2a4455;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {
            border-color: #00d4ff;
        }
    )");
}

QString SkyGuardTheme::getProgressBarStyleSheet() {
    return QString(R"(
        QProgressBar {
            background-color: #0d1a2d;
            border: 1px solid #1a3344;
            border-radius: 8px;
            height: 16px;
            text-align: center;
            font-size: 10px;
            font-weight: 600;
            color: #ffffff;
        }
        
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00d4ff, stop:0.5 #00a8cc, stop:1 #00d4ff);
            border-radius: 7px;
            margin: 1px;
        }
        
        QProgressBar#threatProgress::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ff3344, stop:0.5 #cc2937, stop:1 #ff3344);
        }
        
        QProgressBar#successProgress::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #00ff88, stop:0.5 #00cc6a, stop:1 #00ff88);
        }
        
        QProgressBar#warningProgress::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ffaa00, stop:0.5 #cc8800, stop:1 #ffaa00);
        }
    )");
}

} // namespace CounterUAS
