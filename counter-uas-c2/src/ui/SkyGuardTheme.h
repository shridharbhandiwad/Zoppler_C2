#ifndef SKYGUARDTHEME_H
#define SKYGUARDTHEME_H

#include <QString>
#include <QColor>
#include <QFont>
#include <QApplication>

namespace CounterUAS {

/**
 * SkyGuard Theme - Modern Defense-Grade UI Theme
 * 
 * A professional dark tactical theme with cyan/teal accents,
 * subtle gradients, soft shadows, and modern visual hierarchy.
 * Designed for defense/industrial monitoring applications.
 */
class SkyGuardTheme {
public:
    // ===== COLOR PALETTE =====
    
    // Background Colors (Dark to Light progression)
    static const QColor BackgroundDarkest;    // Deepest background #080e1a
    static const QColor BackgroundDark;       // Main background #0a1628
    static const QColor BackgroundPanel;      // Panel/sidebar background #0d1a2d
    static const QColor BackgroundCard;       // Card/elevated surfaces #111d2e
    static const QColor BackgroundHover;      // Hover state #1a2d44
    static const QColor BackgroundActive;     // Active/pressed state #243a52
    static const QColor BackgroundElevated;   // Floating elements #1e3148
    
    // Primary Accent (Cyan/Teal)
    static const QColor AccentPrimary;        // Primary accent #00d4ff
    static const QColor AccentPrimaryDark;    // Darker accent #00a8cc
    static const QColor AccentPrimaryLight;   // Lighter accent #66e5ff
    static const QColor AccentPrimaryGlow;    // Glow effect rgba(0, 212, 255, 0.25)
    static const QColor AccentPrimaryMuted;   // Muted accent #0088aa
    
    // Secondary Accent (Violet/Purple for contrast)
    static const QColor AccentSecondary;      // Secondary accent #7c4dff
    static const QColor AccentSecondaryDark;  // Darker secondary #5a3dcc
    
    // Status Colors
    static const QColor StatusOnline;         // Online/Operational #00ff88
    static const QColor StatusOnlineDark;     // Darker online #00cc6a
    static const QColor StatusHostile;        // Hostile/Critical #ff3344
    static const QColor StatusHostileDark;    // Darker hostile #cc2937
    static const QColor StatusFriendly;       // Friendly #00cc66
    static const QColor StatusWarning;        // Warning/Elevated #ffaa00
    static const QColor StatusWarningDark;    // Darker warning #cc8800
    static const QColor StatusInfo;           // Info #3388ff
    static const QColor StatusInfoDark;       // Darker info #2866cc
    static const QColor StatusProcessing;     // Processing #ff8800
    static const QColor StatusNeutral;        // Neutral/Unknown #6688aa
    
    // Text Colors
    static const QColor TextPrimary;          // Primary text #ffffff
    static const QColor TextSecondary;        // Secondary text #a8b8c8
    static const QColor TextMuted;            // Muted/disabled text #667788
    static const QColor TextLabel;            // Labels/captions #889aab
    static const QColor TextDark;             // Dark text (on light bg) #0d1a2d
    
    // Border Colors
    static const QColor BorderPrimary;        // Primary border #1a3344
    static const QColor BorderSecondary;      // Secondary border #2a4455
    static const QColor BorderHighlight;      // Highlighted border #00d4ff
    static const QColor BorderActive;         // Active selection #00d4ff
    static const QColor BorderSubtle;         // Subtle/light border #152535
    
    // Gradient Colors
    static const QColor GradientStart;        // Gradient start #0d1a2d
    static const QColor GradientEnd;          // Gradient end #0a1425
    
    // Shadow Colors
    static const QColor ShadowDark;           // Dark shadow rgba(0, 0, 0, 0.4)
    static const QColor ShadowMedium;         // Medium shadow rgba(0, 0, 0, 0.25)
    static const QColor ShadowLight;          // Light shadow rgba(0, 0, 0, 0.15)
    static const QColor ShadowGlow;           // Glow shadow rgba(0, 212, 255, 0.2)
    
    // ===== SPACING & SIZING =====
    static constexpr int SpacingXS = 4;
    static constexpr int SpacingS = 8;
    static constexpr int SpacingM = 12;
    static constexpr int SpacingL = 16;
    static constexpr int SpacingXL = 24;
    static constexpr int SpacingXXL = 32;
    
    static constexpr int BorderRadiusS = 4;
    static constexpr int BorderRadiusM = 6;
    static constexpr int BorderRadiusL = 8;
    static constexpr int BorderRadiusXL = 12;
    static constexpr int BorderRadiusRound = 50;
    
    // ===== FONT SETTINGS =====
    static QString primaryFontFamily();
    static QString monospaceFontFamily();
    static int fontSizeXS();  // 10px
    static int fontSizeS();   // 11px
    static int fontSizeM();   // 13px
    static int fontSizeL();   // 14px
    static int fontSizeXL();  // 16px
    static int fontSizeXXL(); // 20px
    static int fontSizeTitle();  // 28px
    static int fontSizeHuge();   // 36px
    
    // ===== ANIMATION DURATIONS =====
    static constexpr int AnimationFast = 100;
    static constexpr int AnimationNormal = 200;
    static constexpr int AnimationSlow = 350;
    
    // ===== STYLESHEET GETTERS =====
    
    // Complete application stylesheet
    static QString getStyleSheet();
    
    // Component-specific stylesheets
    static QString getSidebarStyleSheet();
    static QString getHeaderStyleSheet();
    static QString getCardStyleSheet();
    static QString getTableStyleSheet();
    static QString getButtonStyleSheet();
    static QString getStatusBarStyleSheet();
    static QString getDialogStyleSheet();
    static QString getTabWidgetStyleSheet();
    static QString getInputStyleSheet();
    static QString getProgressBarStyleSheet();
    
    // ===== UTILITY METHODS =====
    
    // Apply theme to application
    static void applyTheme(QApplication* app);
    
    // Get color as hex string
    static QString toHex(const QColor& color);
    static QString toRgba(const QColor& color);
    
    // Get styled icon path
    static QString iconPath(const QString& iconName);
    
private:
    SkyGuardTheme() = default;
};

} // namespace CounterUAS

#endif // SKYGUARDTHEME_H
