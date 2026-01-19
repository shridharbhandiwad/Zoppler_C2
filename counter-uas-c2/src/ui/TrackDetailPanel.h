#ifndef TRACKDETAILPANEL_H
#define TRACKDETAILPANEL_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QProgressBar>
#include <QComboBox>
#include "core/Track.h"

namespace CounterUAS {

/**
 * @brief TrackDetailPanel - Comprehensive track information display
 * 
 * Displays detailed information about a selected track organized into
 * logical groups:
 * - Identity: Track ID, classification, state
 * - Position: Lat/Lon/Alt, range/bearing from base
 * - Kinematics: Speed, heading, climb rate
 * - Threat Assessment: Level, confidence, proximity to assets
 * - Actions: Engage, slew camera, classify
 */
class TrackDetailPanel : public QWidget {
    Q_OBJECT
    
public:
    explicit TrackDetailPanel(QWidget* parent = nullptr);
    void setTrack(Track* track);
    void setReferencePosition(const GeoPosition& pos);
    void clear();
    
signals:
    void engageRequested(const QString& trackId);
    void slewCameraRequested(const QString& trackId);
    void classificationChanged(const QString& trackId, TrackClassification classification);
    
private slots:
    void updateDisplay();
    void onClassificationRequested();
    
private:
    void setupUI();
    void createIdentityGroup();
    void createPositionGroup();
    void createKinematicsGroup();
    void createThreatGroup();
    void createActionsGroup();
    
    double calculateRange() const;
    double calculateBearing() const;
    double calculateElevation() const;
    
    Track* m_track = nullptr;
    GeoPosition m_referencePosition;
    
    // Identity group
    QGroupBox* m_identityGroup;
    QLabel* m_trackIdLabel;
    QLabel* m_classificationLabel;
    QLabel* m_stateLabel;
    QLabel* m_sourceLabel;
    
    // Position group
    QGroupBox* m_positionGroup;
    QLabel* m_latLabel;
    QLabel* m_lonLabel;
    QLabel* m_altLabel;
    QLabel* m_rangeLabel;
    QLabel* m_bearingLabel;
    QLabel* m_elevationLabel;
    
    // Kinematics group
    QGroupBox* m_kinematicsGroup;
    QLabel* m_speedLabel;
    QLabel* m_headingLabel;
    QLabel* m_climbRateLabel;
    
    // Threat group
    QGroupBox* m_threatGroup;
    QLabel* m_threatLevelLabel;
    QProgressBar* m_threatBar;
    QLabel* m_confidenceLabel;
    QLabel* m_proximityLabel;
    
    // Actions group
    QGroupBox* m_actionsGroup;
    QPushButton* m_engageBtn;
    QPushButton* m_slewBtn;
    QComboBox* m_classifyCombo;
    QPushButton* m_classifyBtn;
};

} // namespace CounterUAS

#endif // TRACKDETAILPANEL_H
