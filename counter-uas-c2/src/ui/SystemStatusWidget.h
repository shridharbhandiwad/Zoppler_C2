#ifndef SYSTEMSTATUSWIDGET_H
#define SYSTEMSTATUSWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>

namespace CounterUAS {

class TrackManager;
class ThreatAssessor;
class EngagementManager;
class VideoStreamManager;

/**
 * @brief SystemStatusWidget - Consolidated system health and status view
 * 
 * Provides an at-a-glance overview of the entire system organized into groups:
 * - System Overview: Track counts, threat metrics
 * - Sensor Status: Radar, RF, Camera health indicators
 * - Effector Status: Jammer, Kinetic, DE system status
 * - Video Status: Stream health, recording status
 */
class SystemStatusWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit SystemStatusWidget(QWidget* parent = nullptr);
    
    void setTrackManager(TrackManager* manager);
    void setThreatAssessor(ThreatAssessor* assessor);
    void setEngagementManager(EngagementManager* manager);
    void setVideoManager(VideoStreamManager* manager);
    
public slots:
    void updateStatus();
    
    // Sensor status updates
    void setSensorStatus(const QString& sensorId, const QString& status, double health);
    
    // Effector status updates
    void setEffectorStatus(const QString& effectorId, const QString& status, bool ready);
    
    // Video status updates
    void setVideoStreamCount(int active, int total);
    void setRecordingStatus(bool recording);
    
private:
    void setupUI();
    void createOverviewGroup();
    void createSensorGroup();
    void createEffectorGroup();
    void createVideoGroup();
    
    QLabel* createStatusIndicator(const QString& label, QWidget* parent);
    void updateIndicator(QLabel* indicator, const QString& status, bool healthy);
    
    // Subsystem pointers
    TrackManager* m_trackManager = nullptr;
    ThreatAssessor* m_threatAssessor = nullptr;
    EngagementManager* m_engagementManager = nullptr;
    VideoStreamManager* m_videoManager = nullptr;
    
    QTimer* m_updateTimer;
    
    // Overview group
    QGroupBox* m_overviewGroup;
    QLabel* m_trackCountLabel;
    QLabel* m_hostileCountLabel;
    QLabel* m_threatLevelLabel;
    QProgressBar* m_systemLoadBar;
    
    // Sensor group
    QGroupBox* m_sensorGroup;
    QLabel* m_radarStatus;
    QLabel* m_rfStatus;
    QLabel* m_cameraStatus;
    
    // Effector group
    QGroupBox* m_effectorGroup;
    QLabel* m_jammerStatus;
    QLabel* m_kineticStatus;
    QLabel* m_deStatus;
    
    // Video group
    QGroupBox* m_videoGroup;
    QLabel* m_streamCountLabel;
    QLabel* m_recordingLabel;
    QLabel* m_fpsLabel;
};

} // namespace CounterUAS

#endif // SYSTEMSTATUSWIDGET_H
