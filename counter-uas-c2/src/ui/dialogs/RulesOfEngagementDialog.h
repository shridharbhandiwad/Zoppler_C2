/**
 * @file RulesOfEngagementDialog.h
 * @brief Dialog for configuring rules of engagement
 */

#ifndef RULESOFENGAGEMENTDIALOG_H
#define RULESOFENGAGEMENTDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QListWidget>

namespace CounterUAS {

class EngagementManager;

/**
 * @brief Rules of engagement configuration
 */
struct ROESettings {
    bool requireOperatorAuthorization = true;
    int authorizationTimeoutSeconds = 60;
    int minimumThreatLevel = 3;
    bool autoEngageHighThreat = false;
    bool autoSelectEffector = true;
    double minimumEngageDistance = 100.0;
    double maximumEngageDistance = 2000.0;
    bool requireBDA = true;
    bool allowMultipleEngagements = false;
    QStringList authorizedEffectorTypes = {"RF_JAMMER", "KINETIC", "DIRECTED_ENERGY"};
    QStringList restrictedZones;
};

/**
 * @brief Dialog for configuring rules of engagement
 */
class RulesOfEngagementDialog : public QDialog {
    Q_OBJECT
    
public:
    explicit RulesOfEngagementDialog(EngagementManager* manager, QWidget* parent = nullptr);
    
    ROESettings getSettings() const;
    void setSettings(const ROESettings& settings);
    
signals:
    void settingsChanged(const ROESettings& settings);
    
private slots:
    void onApply();
    void onAddRestrictedZone();
    void onRemoveRestrictedZone();
    
private:
    void setupUI();
    
    EngagementManager* m_manager;
    
    // Authorization settings
    QCheckBox* m_requireAuth;
    QSpinBox* m_authTimeout;
    QSpinBox* m_minThreatLevel;
    QCheckBox* m_autoEngageHigh;
    QCheckBox* m_autoSelectEffector;
    
    // Engagement parameters
    QDoubleSpinBox* m_minDistance;
    QDoubleSpinBox* m_maxDistance;
    QCheckBox* m_requireBDA;
    QCheckBox* m_allowMultiple;
    
    // Effector authorization
    QCheckBox* m_authRFJammer;
    QCheckBox* m_authKinetic;
    QCheckBox* m_authDE;
    
    // Restricted zones
    QListWidget* m_restrictedZonesList;
};

} // namespace CounterUAS

#endif // RULESOFENGAGEMENTDIALOG_H
