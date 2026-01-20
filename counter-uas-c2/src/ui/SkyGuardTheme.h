#ifndef SKYGUARDTHEME_H
#define SKYGUARDTHEME_H

#include <QString>
#include <QColor>

namespace CounterUAS {

/**
 * Zoppler C-UAS Command System Theme
 * 
 * Dark tactical theme with cyan/teal accents matching the reference design.
 */
class SkyGuardTheme {
public:
    // Color Palette
    static const QColor BackgroundDark;      // Main background #0a1628
    static const QColor BackgroundPanel;     // Panel background #0d1a2d
    static const QColor BackgroundCard;      // Card background #111d2e
    static const QColor BackgroundHover;     // Hover state #1a2d44
    
    static const QColor AccentCyan;          // Primary accent #00d4ff
    static const QColor AccentCyanDark;      // Darker cyan #00a8cc
    static const QColor AccentCyanGlow;      // Glow effect #00d4ff40
    
    static const QColor StatusOnline;        // Online/Operational #00ff88
    static const QColor StatusHostile;       // Hostile/Critical #ff3344
    static const QColor StatusFriendly;      // Friendly #00cc66
    static const QColor StatusWarning;       // Warning/Elevated #ffaa00
    static const QColor StatusInfo;          // Info #3388ff
    static const QColor StatusProcessing;    // Processing #ff8800
    
    static const QColor TextPrimary;         // Primary text #ffffff
    static const QColor TextSecondary;       // Secondary text #8899aa
    static const QColor TextMuted;           // Muted text #556677
    static const QColor TextLabel;           // Labels #667788
    
    static const QColor BorderPrimary;       // Primary border #1a3344
    static const QColor BorderHighlight;     // Highlighted border #00d4ff
    static const QColor BorderActive;        // Active selection #00d4ff
    
    // Get the complete application stylesheet
    static QString getStyleSheet();
    
    // Get component-specific stylesheets
    static QString getSidebarStyleSheet();
    static QString getHeaderStyleSheet();
    static QString getCardStyleSheet();
    static QString getTableStyleSheet();
    static QString getButtonStyleSheet();
    static QString getStatusBarStyleSheet();
    
private:
    SkyGuardTheme() = default;
};

} // namespace CounterUAS

#endif // SKYGUARDTHEME_H
