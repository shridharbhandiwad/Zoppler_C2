#ifndef MODERNCOMPONENTS_H
#define MODERNCOMPONENTS_H

#include &lt;QWidget&gt;
#include &lt;QFrame&gt;
#include &lt;QPushButton&gt;
#include &lt;QLabel&gt;
#include &lt;QVBoxLayout&gt;
#include &lt;QHBoxLayout&gt;
#include &lt;QPropertyAnimation&gt;
#include &lt;QGraphicsDropShadowEffect&gt;
#include &lt;QTimer&gt;
#include &lt;QPainter&gt;
#include &lt;QEnterEvent&gt;

namespace CounterUAS {

/**
 * GlowFrame - A frame with a subtle glow effect and hover animation
 */
class GlowFrame : public QFrame {
    Q_OBJECT
    Q_PROPERTY(qreal glowOpacity READ glowOpacity WRITE setGlowOpacity)
    
public:
    explicit GlowFrame(QWidget* parent = nullptr);
    
    void setGlowColor(const QColor&amp; color);
    void setGlowEnabled(bool enabled);
    void setBorderAccentColor(const QColor&amp; color);
    
    qreal glowOpacity() const { return m_glowOpacity; }
    void setGlowOpacity(qreal opacity);
    
protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    
private:
    QColor m_glowColor;
    QColor m_borderAccentColor;
    qreal m_glowOpacity;
    bool m_glowEnabled;
    QPropertyAnimation* m_glowAnimation;
    QGraphicsDropShadowEffect* m_shadowEffect;
};

/**
 * StatusIndicator - Animated status dot with optional pulse effect
 */
class StatusIndicator : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal pulseScale READ pulseScale WRITE setPulseScale)
    
public:
    enum Status {
        Online,
        Offline,
        Warning,
        Processing,
        Neutral
    };
    Q_ENUM(Status)
    
    explicit StatusIndicator(QWidget* parent = nullptr);
    
    void setStatus(Status status);
    Status status() const { return m_status; }
    
    void setPulseEnabled(bool enabled);
    void setSize(int size);
    
    qreal pulseScale() const { return m_pulseScale; }
    void setPulseScale(qreal scale);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    void updateAnimation();
    QColor statusColor() const;
    
    Status m_status;
    int m_size;
    bool m_pulseEnabled;
    qreal m_pulseScale;
    QPropertyAnimation* m_pulseAnimation;
};

/**
 * ModernButton - A styled button with hover effects and optional icon
 */
class ModernButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(qreal hoverProgress READ hoverProgress WRITE setHoverProgress)
    
public:
    enum ButtonStyle {
        Primary,     // Cyan outline
        Accent,      // Filled cyan
        Danger,      // Red outline
        Success,     // Green outline
        Secondary,   // Gray outline
        Flat         // No border
    };
    Q_ENUM(ButtonStyle)
    
    explicit ModernButton(const QString&amp; text = QString(), QWidget* parent = nullptr);
    ModernButton(const QIcon&amp; icon, const QString&amp; text, QWidget* parent = nullptr);
    
    void setButtonStyle(ButtonStyle style);
    void setIconPath(const QString&amp; iconPath);
    
    qreal hoverProgress() const { return m_hoverProgress; }
    void setHoverProgress(qreal progress);
    
protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    
private:
    void setupStyle();
    
    ButtonStyle m_buttonStyle;
    qreal m_hoverProgress;
    QPropertyAnimation* m_hoverAnimation;
};

/**
 * StatCard - A modern stat display card with title, value, and subtitle
 */
class ModernStatCard : public GlowFrame {
    Q_OBJECT
    
public:
    explicit ModernStatCard(QWidget* parent = nullptr);
    
    void setTitle(const QString&amp; title);
    void setValue(const QString&amp; value);
    void setSubtitle(const QString&amp; subtitle);
    void setIcon(const QIcon&amp; icon);
    void setAccentColor(const QColor&amp; color);
    
private:
    void setupUI();
    
    QLabel* m_titleLabel;
    QLabel* m_valueLabel;
    QLabel* m_subtitleLabel;
    QLabel* m_iconLabel;
    QColor m_accentColor;
};

/**
 * AnimatedProgressBar - Progress bar with smooth animation
 */
class AnimatedProgressBar : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int animatedValue READ animatedValue WRITE setAnimatedValue)
    
public:
    explicit AnimatedProgressBar(QWidget* parent = nullptr);
    
    void setValue(int value);
    void setBarColor(const QColor&amp; color);
    void setBackgroundColor(const QColor&amp; color);
    void setAnimationDuration(int msec);
    
    int animatedValue() const { return m_animatedValue; }
    void setAnimatedValue(int value);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    int m_targetValue;
    int m_animatedValue;
    QColor m_barColor;
    QColor m_backgroundColor;
    QPropertyAnimation* m_animation;
};

/**
 * IconLabel - A label optimized for SVG icons with color theming
 */
class IconLabel : public QLabel {
    Q_OBJECT
    
public:
    explicit IconLabel(QWidget* parent = nullptr);
    
    void setIconPath(const QString&amp; path);
    void setIconSize(const QSize&amp; size);
    void setIconColor(const QColor&amp; color);
    
private:
    void updateIcon();
    
    QString m_iconPath;
    QSize m_iconSize;
    QColor m_iconColor;
};

/**
 * BadgeLabel - A pill-shaped badge for status/category display
 */
class BadgeLabel : public QLabel {
    Q_OBJECT
    
public:
    enum BadgeType {
        Info,
        Success,
        Warning,
        Danger,
        Neutral,
        Primary
    };
    Q_ENUM(BadgeType)
    
    explicit BadgeLabel(const QString&amp; text = QString(), QWidget* parent = nullptr);
    
    void setBadgeType(BadgeType type);
    void setCustomColors(const QColor&amp; background, const QColor&amp; text);
    
private:
    void updateStyle();
    
    BadgeType m_type;
    QColor m_customBackground;
    QColor m_customText;
    bool m_useCustomColors;
};

/**
 * SectionHeader - A styled section header with icon and optional action button
 */
class SectionHeader : public QWidget {
    Q_OBJECT
    
public:
    explicit SectionHeader(const QString&amp; title, QWidget* parent = nullptr);
    
    void setTitle(const QString&amp; title);
    void setSubtitle(const QString&amp; subtitle);
    void setIcon(const QIcon&amp; icon);
    void setActionButton(const QString&amp; text, const QString&amp; iconPath = QString());
    
signals:
    void actionClicked();
    
private:
    void setupUI();
    
    QLabel* m_iconLabel;
    QLabel* m_titleLabel;
    QLabel* m_subtitleLabel;
    ModernButton* m_actionButton;
};

/**
 * LoadingSpinner - An animated loading indicator
 */
class LoadingSpinner : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int rotation READ rotation WRITE setRotation)
    
public:
    explicit LoadingSpinner(QWidget* parent = nullptr);
    
    void start();
    void stop();
    bool isSpinning() const;
    
    void setColor(const QColor&amp; color);
    void setLineWidth(int width);
    
    int rotation() const { return m_rotation; }
    void setRotation(int rotation);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    int m_rotation;
    int m_lineWidth;
    QColor m_color;
    QPropertyAnimation* m_animation;
    bool m_spinning;
};

} // namespace CounterUAS

#endif // MODERNCOMPONENTS_H
