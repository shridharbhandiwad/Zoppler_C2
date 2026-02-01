#include "ui/ModernComponents.h"
#include "ui/SkyGuardTheme.h"
#include <QPainterPath>
#include <QSvgRenderer>
#include <QtMath>

namespace CounterUAS {

// ==================== GlowFrame ====================

GlowFrame::GlowFrame(QWidget* parent)
    : QFrame(parent)
    , m_glowColor(SkyGuardTheme::AccentPrimary)
    , m_borderAccentColor(SkyGuardTheme::BorderPrimary)
    , m_glowOpacity(0.0)
    , m_glowEnabled(true)
    , m_glowAnimation(new QPropertyAnimation(this, "glowOpacity", this))
    , m_shadowEffect(new QGraphicsDropShadowEffect(this))
{
    setObjectName("glowFrame");
    
    // Setup shadow effect
    m_shadowEffect->setBlurRadius(20);
    m_shadowEffect->setOffset(0, 4);
    m_shadowEffect->setColor(SkyGuardTheme::ShadowMedium);
    setGraphicsEffect(m_shadowEffect);
    
    // Setup animation
    m_glowAnimation->setDuration(SkyGuardTheme::AnimationNormal);
    m_glowAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void GlowFrame::setGlowColor(const QColor& color) {
    m_glowColor = color;
    update();
}

void GlowFrame::setGlowEnabled(bool enabled) {
    m_glowEnabled = enabled;
    if (!enabled) {
        setGlowOpacity(0);
    }
}

void GlowFrame::setBorderAccentColor(const QColor& color) {
    m_borderAccentColor = color;
    update();
}

void GlowFrame::setGlowOpacity(qreal opacity) {
    m_glowOpacity = qBound(0.0, opacity, 1.0);
    
    // Update shadow for glow effect
    if (m_glowEnabled && m_glowOpacity > 0) {
        QColor glowShadow = m_glowColor;
        glowShadow.setAlphaF(m_glowOpacity * 0.4);
        m_shadowEffect->setColor(glowShadow);
        m_shadowEffect->setBlurRadius(25 + m_glowOpacity * 15);
    } else {
        m_shadowEffect->setColor(SkyGuardTheme::ShadowMedium);
        m_shadowEffect->setBlurRadius(20);
    }
    
    update();
}

void GlowFrame::enterEvent(QEnterEvent* event) {
    if (m_glowEnabled) {
        m_glowAnimation->stop();
        m_glowAnimation->setStartValue(m_glowOpacity);
        m_glowAnimation->setEndValue(1.0);
        m_glowAnimation->start();
    }
    QFrame::enterEvent(event);
}

void GlowFrame::leaveEvent(QEvent* event) {
    if (m_glowEnabled) {
        m_glowAnimation->stop();
        m_glowAnimation->setStartValue(m_glowOpacity);
        m_glowAnimation->setEndValue(0.0);
        m_glowAnimation->start();
    }
    QFrame::leaveEvent(event);
}

void GlowFrame::paintEvent(QPaintEvent* event) {
    QFrame::paintEvent(event);
    
    if (m_glowEnabled && m_glowOpacity > 0) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // Draw subtle glow border
        QColor borderColor = m_glowColor;
        borderColor.setAlphaF(m_glowOpacity * 0.6);
        
        QPen pen(borderColor, 1);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 8, 8);
    }
}

// ==================== StatusIndicator ====================

StatusIndicator::StatusIndicator(QWidget* parent)
    : QWidget(parent)
    , m_status(Online)
    , m_size(12)
    , m_pulseEnabled(true)
    , m_pulseScale(1.0)
    , m_pulseAnimation(new QPropertyAnimation(this, "pulseScale", this))
{
    setFixedSize(m_size * 2, m_size * 2);
    setAttribute(Qt::WA_TranslucentBackground);
    
    updateAnimation();
}

void StatusIndicator::setStatus(Status status) {
    m_status = status;
    updateAnimation();
    update();
}

void StatusIndicator::setPulseEnabled(bool enabled) {
    m_pulseEnabled = enabled;
    updateAnimation();
}

void StatusIndicator::setSize(int size) {
    m_size = size;
    setFixedSize(size * 2, size * 2);
    update();
}

void StatusIndicator::setPulseScale(qreal scale) {
    m_pulseScale = scale;
    update();
}

void StatusIndicator::updateAnimation() {
    m_pulseAnimation->stop();
    
    if (m_pulseEnabled && (m_status == Online || m_status == Processing)) {
        m_pulseAnimation->setDuration(1500);
        m_pulseAnimation->setStartValue(1.0);
        m_pulseAnimation->setEndValue(1.5);
        m_pulseAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        m_pulseAnimation->setLoopCount(-1);
        m_pulseAnimation->start();
    } else {
        m_pulseScale = 1.0;
    }
}

QColor StatusIndicator::statusColor() const {
    switch (m_status) {
        case Online:     return SkyGuardTheme::StatusOnline;
        case Offline:    return SkyGuardTheme::StatusHostile;
        case Warning:    return SkyGuardTheme::StatusWarning;
        case Processing: return SkyGuardTheme::StatusProcessing;
        case Neutral:    
        default:         return SkyGuardTheme::StatusNeutral;
    }
}

void StatusIndicator::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QColor color = statusColor();
    QPointF center(width() / 2.0, height() / 2.0);
    
    // Draw pulse ring
    if (m_pulseEnabled && m_pulseScale > 1.0) {
        QColor pulseColor = color;
        pulseColor.setAlphaF(0.3 * (2.0 - m_pulseScale));
        painter.setPen(Qt::NoPen);
        painter.setBrush(pulseColor);
        qreal pulseRadius = m_size / 2.0 * m_pulseScale;
        painter.drawEllipse(center, pulseRadius, pulseRadius);
    }
    
    // Draw glow
    QColor glowColor = color;
    glowColor.setAlphaF(0.3);
    painter.setBrush(glowColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(center, m_size * 0.7, m_size * 0.7);
    
    // Draw main dot
    painter.setBrush(color);
    painter.drawEllipse(center, m_size / 2.0, m_size / 2.0);
}

// ==================== ModernButton ====================

ModernButton::ModernButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
    , m_buttonStyle(Primary)
    , m_hoverProgress(0)
    , m_hoverAnimation(new QPropertyAnimation(this, "hoverProgress", this))
{
    setCursor(Qt::PointingHandCursor);
    m_hoverAnimation->setDuration(SkyGuardTheme::AnimationNormal);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
    setupStyle();
}

ModernButton::ModernButton(const QIcon& icon, const QString& text, QWidget* parent)
    : QPushButton(icon, text, parent)
    , m_buttonStyle(Primary)
    , m_hoverProgress(0)
    , m_hoverAnimation(new QPropertyAnimation(this, "hoverProgress", this))
{
    setCursor(Qt::PointingHandCursor);
    m_hoverAnimation->setDuration(SkyGuardTheme::AnimationNormal);
    m_hoverAnimation->setEasingCurve(QEasingCurve::OutCubic);
    setupStyle();
}

void ModernButton::setButtonStyle(ButtonStyle style) {
    m_buttonStyle = style;
    setupStyle();
}

void ModernButton::setIconPath(const QString& iconPath) {
    setIcon(QIcon(iconPath));
}

void ModernButton::setHoverProgress(qreal progress) {
    m_hoverProgress = qBound(0.0, progress, 1.0);
    update();
}

void ModernButton::enterEvent(QEnterEvent* event) {
    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(m_hoverProgress);
    m_hoverAnimation->setEndValue(1.0);
    m_hoverAnimation->start();
    QPushButton::enterEvent(event);
}

void ModernButton::leaveEvent(QEvent* event) {
    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(m_hoverProgress);
    m_hoverAnimation->setEndValue(0.0);
    m_hoverAnimation->start();
    QPushButton::leaveEvent(event);
}

void ModernButton::paintEvent(QPaintEvent* event) {
    QPushButton::paintEvent(event);
}

void ModernButton::setupStyle() {
    switch (m_buttonStyle) {
        case Primary:
            setObjectName("primaryButton");
            break;
        case Accent:
            setObjectName("accentButton");
            break;
        case Danger:
            setObjectName("dangerButton");
            break;
        case Success:
            setObjectName("successButton");
            break;
        case Secondary:
            setObjectName("");
            break;
        case Flat:
            setObjectName("flatButton");
            break;
    }
}

// ==================== ModernStatCard ====================

ModernStatCard::ModernStatCard(QWidget* parent)
    : GlowFrame(parent)
    , m_accentColor(SkyGuardTheme::AccentPrimary)
{
    setupUI();
    setGlowColor(m_accentColor);
}

void ModernStatCard::setupUI() {
    setMinimumSize(180, 100);
    setMaximumHeight(130);
    
    setStyleSheet(QString(
        "GlowFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #141f32, stop:1 #111d2e);"
        "   border: 1px solid %1;"
        "   border-radius: 10px;"
        "}"
    ).arg(SkyGuardTheme::toHex(SkyGuardTheme::AccentPrimary)));
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(4);
    
    // Header row
    QHBoxLayout* headerRow = new QHBoxLayout();
    headerRow->setSpacing(8);
    
    m_titleLabel = new QLabel(this);
    m_titleLabel->setStyleSheet(QString(
        "font-size: 10px; font-weight: 700; color: %1; letter-spacing: 1px;"
    ).arg(SkyGuardTheme::toHex(SkyGuardTheme::AccentPrimary)));
    headerRow->addWidget(m_titleLabel);
    
    headerRow->addStretch();
    
    m_iconLabel = new QLabel(this);
    m_iconLabel->setStyleSheet("font-size: 16px;");
    headerRow->addWidget(m_iconLabel);
    
    layout->addLayout(headerRow);
    
    // Value
    m_valueLabel = new QLabel(this);
    m_valueLabel->setStyleSheet(QString(
        "font-size: 28px; font-weight: 700; color: %1;"
    ).arg(SkyGuardTheme::toHex(SkyGuardTheme::AccentPrimary)));
    layout->addWidget(m_valueLabel);
    
    // Subtitle
    m_subtitleLabel = new QLabel(this);
    m_subtitleLabel->setStyleSheet(QString(
        "font-size: 11px; color: %1;"
    ).arg(SkyGuardTheme::toHex(SkyGuardTheme::TextMuted)));
    layout->addWidget(m_subtitleLabel);
}

void ModernStatCard::setTitle(const QString& title) {
    m_titleLabel->setText(title.toUpper());
}

void ModernStatCard::setValue(const QString& value) {
    m_valueLabel->setText(value);
}

void ModernStatCard::setSubtitle(const QString& subtitle) {
    m_subtitleLabel->setText(subtitle);
}

void ModernStatCard::setIcon(const QIcon& icon) {
    m_iconLabel->setPixmap(icon.pixmap(16, 16));
}

void ModernStatCard::setAccentColor(const QColor& color) {
    m_accentColor = color;
    setGlowColor(color);
    
    m_titleLabel->setStyleSheet(QString(
        "font-size: 10px; font-weight: 700; color: %1; letter-spacing: 1px;"
    ).arg(SkyGuardTheme::toHex(color)));
    
    m_valueLabel->setStyleSheet(QString(
        "font-size: 28px; font-weight: 700; color: %1;"
    ).arg(SkyGuardTheme::toHex(color)));
    
    setStyleSheet(QString(
        "GlowFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #141f32, stop:1 #111d2e);"
        "   border: 1px solid %1;"
        "   border-radius: 10px;"
        "}"
    ).arg(SkyGuardTheme::toHex(color)));
}

// ==================== AnimatedProgressBar ====================

AnimatedProgressBar::AnimatedProgressBar(QWidget* parent)
    : QWidget(parent)
    , m_targetValue(0)
    , m_animatedValue(0)
    , m_barColor(SkyGuardTheme::AccentPrimary)
    , m_backgroundColor(SkyGuardTheme::BackgroundPanel)
    , m_animation(new QPropertyAnimation(this, "animatedValue", this))
{
    setMinimumHeight(12);
    setMaximumHeight(16);
    
    m_animation->setDuration(SkyGuardTheme::AnimationSlow);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
}

void AnimatedProgressBar::setValue(int value) {
    m_targetValue = qBound(0, value, 100);
    
    m_animation->stop();
    m_animation->setStartValue(m_animatedValue);
    m_animation->setEndValue(m_targetValue);
    m_animation->start();
}

void AnimatedProgressBar::setBarColor(const QColor& color) {
    m_barColor = color;
    update();
}

void AnimatedProgressBar::setBackgroundColor(const QColor& color) {
    m_backgroundColor = color;
    update();
}

void AnimatedProgressBar::setAnimationDuration(int msec) {
    m_animation->setDuration(msec);
}

void AnimatedProgressBar::setAnimatedValue(int value) {
    m_animatedValue = value;
    update();
}

void AnimatedProgressBar::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int borderRadius = height() / 2;
    
    // Background
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundColor);
    painter.drawRoundedRect(rect(), borderRadius, borderRadius);
    
    // Progress bar
    if (m_animatedValue > 0) {
        int barWidth = (width() - 2) * m_animatedValue / 100;
        QRect barRect(1, 1, barWidth, height() - 2);
        
        QLinearGradient gradient(barRect.topLeft(), barRect.topRight());
        gradient.setColorAt(0, m_barColor);
        gradient.setColorAt(0.5, m_barColor.lighter(110));
        gradient.setColorAt(1, m_barColor);
        
        painter.setBrush(gradient);
        painter.drawRoundedRect(barRect, borderRadius - 1, borderRadius - 1);
    }
}

// ==================== IconLabel ====================

IconLabel::IconLabel(QWidget* parent)
    : QLabel(parent)
    , m_iconSize(24, 24)
    , m_iconColor(SkyGuardTheme::AccentPrimary)
{
    setAlignment(Qt::AlignCenter);
}

void IconLabel::setIconPath(const QString& path) {
    m_iconPath = path;
    updateIcon();
}

void IconLabel::setIconSize(const QSize& size) {
    m_iconSize = size;
    updateIcon();
}

void IconLabel::setIconColor(const QColor& color) {
    m_iconColor = color;
    updateIcon();
}

void IconLabel::updateIcon() {
    if (m_iconPath.isEmpty()) {
        clear();
        return;
    }
    
    QIcon icon(m_iconPath);
    QPixmap pixmap = icon.pixmap(m_iconSize);
    setPixmap(pixmap);
}

// ==================== BadgeLabel ====================

BadgeLabel::BadgeLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent)
    , m_type(Info)
    , m_useCustomColors(false)
{
    setAlignment(Qt::AlignCenter);
    updateStyle();
}

void BadgeLabel::setBadgeType(BadgeType type) {
    m_type = type;
    m_useCustomColors = false;
    updateStyle();
}

void BadgeLabel::setCustomColors(const QColor& background, const QColor& text) {
    m_customBackground = background;
    m_customText = text;
    m_useCustomColors = true;
    updateStyle();
}

void BadgeLabel::updateStyle() {
    QColor bg, fg;
    
    if (m_useCustomColors) {
        bg = m_customBackground;
        fg = m_customText;
    } else {
        switch (m_type) {
            case Info:
                bg = SkyGuardTheme::StatusInfo;
                fg = Qt::white;
                break;
            case Success:
                bg = SkyGuardTheme::StatusOnline;
                fg = SkyGuardTheme::TextDark;
                break;
            case Warning:
                bg = SkyGuardTheme::StatusWarning;
                fg = SkyGuardTheme::TextDark;
                break;
            case Danger:
                bg = SkyGuardTheme::StatusHostile;
                fg = Qt::white;
                break;
            case Primary:
                bg = SkyGuardTheme::AccentPrimary;
                fg = SkyGuardTheme::TextDark;
                break;
            case Neutral:
            default:
                bg = SkyGuardTheme::StatusNeutral;
                fg = Qt::white;
                break;
        }
    }
    
    setStyleSheet(QString(
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "    stop:0 %1, stop:1 %2);"
        "color: %3;"
        "font-size: 10px;"
        "font-weight: 700;"
        "padding: 5px 14px;"
        "border-radius: 12px;"
    ).arg(bg.lighter(105).name())
     .arg(bg.name())
     .arg(fg.name()));
}

// ==================== SectionHeader ====================

SectionHeader::SectionHeader(const QString& title, QWidget* parent)
    : QWidget(parent)
    , m_iconLabel(new QLabel(this))
    , m_titleLabel(new QLabel(title, this))
    , m_subtitleLabel(new QLabel(this))
    , m_actionButton(nullptr)
{
    setupUI();
}

void SectionHeader::setupUI() {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);
    
    m_iconLabel->setFixedSize(24, 24);
    layout->addWidget(m_iconLabel);
    
    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);
    
    m_titleLabel->setStyleSheet(QString(
        "font-size: 16px; font-weight: 700; color: %1; letter-spacing: 1px;"
    ).arg(SkyGuardTheme::toHex(SkyGuardTheme::TextPrimary)));
    textLayout->addWidget(m_titleLabel);
    
    m_subtitleLabel->setStyleSheet(QString(
        "font-size: 12px; color: %1;"
    ).arg(SkyGuardTheme::toHex(SkyGuardTheme::TextMuted)));
    m_subtitleLabel->hide();
    textLayout->addWidget(m_subtitleLabel);
    
    layout->addLayout(textLayout);
    layout->addStretch();
}

void SectionHeader::setTitle(const QString& title) {
    m_titleLabel->setText(title);
}

void SectionHeader::setSubtitle(const QString& subtitle) {
    m_subtitleLabel->setText(subtitle);
    m_subtitleLabel->setVisible(!subtitle.isEmpty());
}

void SectionHeader::setIcon(const QIcon& icon) {
    m_iconLabel->setPixmap(icon.pixmap(24, 24));
}

void SectionHeader::setActionButton(const QString& text, const QString& iconPath) {
    if (!m_actionButton) {
        m_actionButton = new ModernButton(text, this);
        m_actionButton->setButtonStyle(ModernButton::Primary);
        connect(m_actionButton, &QPushButton::clicked, this, &SectionHeader::actionClicked);
        static_cast<QHBoxLayout*>(layout())->addWidget(m_actionButton);
    } else {
        m_actionButton->setText(text);
    }
    
    if (!iconPath.isEmpty()) {
        m_actionButton->setIconPath(iconPath);
    }
}

// ==================== LoadingSpinner ====================

LoadingSpinner::LoadingSpinner(QWidget* parent)
    : QWidget(parent)
    , m_rotation(0)
    , m_lineWidth(3)
    , m_color(SkyGuardTheme::AccentPrimary)
    , m_animation(new QPropertyAnimation(this, "rotation", this))
    , m_spinning(false)
{
    setFixedSize(32, 32);
    setAttribute(Qt::WA_TranslucentBackground);
    
    m_animation->setDuration(1000);
    m_animation->setStartValue(0);
    m_animation->setEndValue(360);
    m_animation->setLoopCount(-1);
}

void LoadingSpinner::start() {
    m_spinning = true;
    m_animation->start();
    show();
}

void LoadingSpinner::stop() {
    m_spinning = false;
    m_animation->stop();
}

bool LoadingSpinner::isSpinning() const {
    return m_spinning;
}

void LoadingSpinner::setColor(const QColor& color) {
    m_color = color;
    update();
}

void LoadingSpinner::setLineWidth(int width) {
    m_lineWidth = width;
    update();
}

void LoadingSpinner::setRotation(int rotation) {
    m_rotation = rotation;
    update();
}

void LoadingSpinner::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int side = qMin(width(), height());
    painter.translate(width() / 2, height() / 2);
    painter.rotate(m_rotation);
    
    // Draw arc
    QPen pen(m_color, m_lineWidth, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    
    QRectF rect(-side / 2 + m_lineWidth, -side / 2 + m_lineWidth,
                side - 2 * m_lineWidth, side - 2 * m_lineWidth);
    
    // Draw partial arc
    painter.drawArc(rect, 0, 270 * 16);
}

} // namespace CounterUAS
