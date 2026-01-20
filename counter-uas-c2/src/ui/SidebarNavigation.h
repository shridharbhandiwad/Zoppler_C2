#ifndef SIDEBARNAVIGATION_H
#define SIDEBARNAVIGATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QFrame>

namespace CounterUAS {

/**
 * Navigation item for the sidebar
 */
struct NavItem {
    QString id;
    QString label;
    QString icon;  // Unicode icon or path
};

/**
 * SidebarNavigation - Left navigation panel for SKYGUARD interface
 * 
 * Provides navigation between main application pages:
 * - Tactical Map
 * - Threat List
 * - Sensor Net
 * - Alert Log
 * - System Health
 * - Gallery / Ref
 */
class SidebarNavigation : public QWidget {
    Q_OBJECT
    
public:
    explicit SidebarNavigation(QWidget* parent = nullptr);
    ~SidebarNavigation() override = default;
    
    void setCurrentPage(const QString& pageId);
    QString currentPage() const { return m_currentPage; }
    
    void setOnlineStatus(bool online);
    
signals:
    void pageSelected(const QString& pageId);
    
private slots:
    void onNavButtonClicked(int index);
    
private:
    void setupUI();
    QPushButton* createNavButton(const NavItem& item);
    
    QVBoxLayout* m_layout;
    QButtonGroup* m_buttonGroup;
    QList<NavItem> m_navItems;
    QString m_currentPage;
    
    QLabel* m_logoLabel;
    QLabel* m_subtitleLabel;
    QLabel* m_statusLabel;
    QLabel* m_statusValue;
    QLabel* m_versionLabel;
};

} // namespace CounterUAS

#endif // SIDEBARNAVIGATION_H
