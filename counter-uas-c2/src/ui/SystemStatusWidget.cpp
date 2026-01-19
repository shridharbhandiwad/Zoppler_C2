#include "ui/SystemStatusWidget.h"
#include "core/TrackManager.h"
#include "core/ThreatAssessor.h"
#include "core/EngagementManager.h"
#include "video/VideoStreamManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

namespace CounterUAS {

SystemStatusWidget::SystemStatusWidget(QWidget* parent)
    : QWidget(parent)
    , m_updateTimer(new QTimer(this))
{
    setupUI();
    
    connect(m_updateTimer, &QTimer::timeout, this, &SystemStatusWidget::updateStatus);
    m_updateTimer->start(1000);  // Update every second
}

void SystemStatusWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(6);
    
    // Panel title
    QLabel* titleLabel = new QLabel("SYSTEM STATUS", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 11px; color: #aaa; "
                             "padding: 4px; background-color: #2a2a2a; border-radius: 3px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    createOverviewGroup();
    createSensorGroup();
    createEffectorGroup();
    createVideoGroup();
    
    mainLayout->addWidget(m_overviewGroup);
    mainLayout->addWidget(m_sensorGroup);
    mainLayout->addWidget(m_effectorGroup);
    mainLayout->addWidget(m_videoGroup);
    mainLayout->addStretch();
}

void SystemStatusWidget::createOverviewGroup() {
    m_overviewGroup = new QGroupBox("Overview", this);
    m_overviewGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #8cf; }");
    
    QGridLayout* layout = new QGridLayout(m_overviewGroup);
    layout->setSpacing(4);
    layout->setContentsMargins(8, 8, 8, 8);
    
    // Track count
    layout->addWidget(new QLabel("Tracks:", this), 0, 0);
    m_trackCountLabel = new QLabel("0", this);
    m_trackCountLabel->setStyleSheet("QLabel { color: #fff; font-weight: bold; font-size: 14px; }");
    layout->addWidget(m_trackCountLabel, 0, 1);
    
    // Hostile count
    layout->addWidget(new QLabel("Hostile:", this), 0, 2);
    m_hostileCountLabel = new QLabel("0", this);
    m_hostileCountLabel->setStyleSheet("QLabel { color: #f44; font-weight: bold; font-size: 14px; }");
    layout->addWidget(m_hostileCountLabel, 0, 3);
    
    // Threat level
    layout->addWidget(new QLabel("Max Threat:", this), 1, 0);
    m_threatLevelLabel = new QLabel("-", this);
    m_threatLevelLabel->setStyleSheet("QLabel { color: #ff8; font-weight: bold; }");
    layout->addWidget(m_threatLevelLabel, 1, 1);
    
    // System load
    layout->addWidget(new QLabel("Load:", this), 1, 2);
    m_systemLoadBar = new QProgressBar(this);
    m_systemLoadBar->setRange(0, 100);
    m_systemLoadBar->setValue(0);
    m_systemLoadBar->setTextVisible(false);
    m_systemLoadBar->setFixedHeight(14);
    m_systemLoadBar->setStyleSheet(
        "QProgressBar { background-color: #333; border-radius: 3px; }"
        "QProgressBar::chunk { background-color: #4a4; border-radius: 3px; }");
    layout->addWidget(m_systemLoadBar, 1, 3);
}

void SystemStatusWidget::createSensorGroup() {
    m_sensorGroup = new QGroupBox("Sensors", this);
    m_sensorGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #8cf; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_sensorGroup);
    layout->setSpacing(3);
    layout->setContentsMargins(8, 8, 8, 8);
    
    // Radar status
    QHBoxLayout* radarLayout = new QHBoxLayout();
    radarLayout->addWidget(new QLabel("Radar:", this));
    m_radarStatus = createStatusIndicator("OFFLINE", this);
    radarLayout->addWidget(m_radarStatus);
    radarLayout->addStretch();
    layout->addLayout(radarLayout);
    
    // RF Detector status
    QHBoxLayout* rfLayout = new QHBoxLayout();
    rfLayout->addWidget(new QLabel("RF Det:", this));
    m_rfStatus = createStatusIndicator("OFFLINE", this);
    rfLayout->addWidget(m_rfStatus);
    rfLayout->addStretch();
    layout->addLayout(rfLayout);
    
    // Camera status
    QHBoxLayout* camLayout = new QHBoxLayout();
    camLayout->addWidget(new QLabel("Camera:", this));
    m_cameraStatus = createStatusIndicator("OFFLINE", this);
    camLayout->addWidget(m_cameraStatus);
    camLayout->addStretch();
    layout->addLayout(camLayout);
}

void SystemStatusWidget::createEffectorGroup() {
    m_effectorGroup = new QGroupBox("Effectors", this);
    m_effectorGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #f88; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_effectorGroup);
    layout->setSpacing(3);
    layout->setContentsMargins(8, 8, 8, 8);
    
    // Jammer status
    QHBoxLayout* jamLayout = new QHBoxLayout();
    jamLayout->addWidget(new QLabel("Jammer:", this));
    m_jammerStatus = createStatusIndicator("OFFLINE", this);
    jamLayout->addWidget(m_jammerStatus);
    jamLayout->addStretch();
    layout->addLayout(jamLayout);
    
    // Kinetic status
    QHBoxLayout* kinLayout = new QHBoxLayout();
    kinLayout->addWidget(new QLabel("Kinetic:", this));
    m_kineticStatus = createStatusIndicator("OFFLINE", this);
    kinLayout->addWidget(m_kineticStatus);
    kinLayout->addStretch();
    layout->addLayout(kinLayout);
    
    // DE status
    QHBoxLayout* deLayout = new QHBoxLayout();
    deLayout->addWidget(new QLabel("DE Sys:", this));
    m_deStatus = createStatusIndicator("OFFLINE", this);
    deLayout->addWidget(m_deStatus);
    deLayout->addStretch();
    layout->addLayout(deLayout);
}

void SystemStatusWidget::createVideoGroup() {
    m_videoGroup = new QGroupBox("Video", this);
    m_videoGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #8cf; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_videoGroup);
    layout->setSpacing(3);
    layout->setContentsMargins(8, 8, 8, 8);
    
    // Stream count
    QHBoxLayout* streamLayout = new QHBoxLayout();
    streamLayout->addWidget(new QLabel("Streams:", this));
    m_streamCountLabel = new QLabel("0 / 0", this);
    m_streamCountLabel->setStyleSheet("QLabel { color: #ccc; }");
    streamLayout->addWidget(m_streamCountLabel);
    streamLayout->addStretch();
    layout->addLayout(streamLayout);
    
    // Recording status
    QHBoxLayout* recLayout = new QHBoxLayout();
    recLayout->addWidget(new QLabel("Recording:", this));
    m_recordingLabel = new QLabel("OFF", this);
    m_recordingLabel->setStyleSheet("QLabel { color: #888; }");
    recLayout->addWidget(m_recordingLabel);
    recLayout->addStretch();
    layout->addLayout(recLayout);
    
    // FPS
    QHBoxLayout* fpsLayout = new QHBoxLayout();
    fpsLayout->addWidget(new QLabel("FPS:", this));
    m_fpsLabel = new QLabel("-", this);
    m_fpsLabel->setStyleSheet("QLabel { color: #ccc; }");
    fpsLayout->addWidget(m_fpsLabel);
    fpsLayout->addStretch();
    layout->addLayout(fpsLayout);
}

QLabel* SystemStatusWidget::createStatusIndicator(const QString& label, QWidget* parent) {
    QLabel* indicator = new QLabel(label, parent);
    indicator->setFixedWidth(60);
    indicator->setAlignment(Qt::AlignCenter);
    indicator->setStyleSheet(
        "QLabel { color: #888; background-color: #333; padding: 2px 6px; "
        "border-radius: 3px; font-size: 10px; font-weight: bold; }");
    return indicator;
}

void SystemStatusWidget::updateIndicator(QLabel* indicator, const QString& status, bool healthy) {
    indicator->setText(status);
    
    if (status == "ONLINE" || status == "READY" || healthy) {
        indicator->setStyleSheet(
            "QLabel { color: #fff; background-color: #2a6a2a; padding: 2px 6px; "
            "border-radius: 3px; font-size: 10px; font-weight: bold; }");
    } else if (status == "ENGAGED" || status == "ACTIVE") {
        indicator->setStyleSheet(
            "QLabel { color: #fff; background-color: #6a6a2a; padding: 2px 6px; "
            "border-radius: 3px; font-size: 10px; font-weight: bold; }");
    } else if (status == "FAULT" || status == "ERROR") {
        indicator->setStyleSheet(
            "QLabel { color: #fff; background-color: #8a2a2a; padding: 2px 6px; "
            "border-radius: 3px; font-size: 10px; font-weight: bold; }");
    } else {
        indicator->setStyleSheet(
            "QLabel { color: #888; background-color: #333; padding: 2px 6px; "
            "border-radius: 3px; font-size: 10px; font-weight: bold; }");
    }
}

void SystemStatusWidget::setTrackManager(TrackManager* manager) {
    m_trackManager = manager;
}

void SystemStatusWidget::setThreatAssessor(ThreatAssessor* assessor) {
    m_threatAssessor = assessor;
}

void SystemStatusWidget::setEngagementManager(EngagementManager* manager) {
    m_engagementManager = manager;
}

void SystemStatusWidget::setVideoManager(VideoStreamManager* manager) {
    m_videoManager = manager;
}

void SystemStatusWidget::updateStatus() {
    // Update track counts
    if (m_trackManager) {
        m_trackCountLabel->setText(QString::number(m_trackManager->trackCount()));
    }
    
    // Update threat metrics
    if (m_threatAssessor) {
        auto metrics = m_threatAssessor->metrics();
        m_hostileCountLabel->setText(QString::number(metrics.hostileCount));
        
        if (metrics.highThreatCount > 0) {
            m_threatLevelLabel->setText(QString("Level %1").arg(5));  // Max threat
            m_threatLevelLabel->setStyleSheet("QLabel { color: #f44; font-weight: bold; }");
        } else if (metrics.hostileCount > 0) {
            m_threatLevelLabel->setText("Elevated");
            m_threatLevelLabel->setStyleSheet("QLabel { color: #fa4; font-weight: bold; }");
        } else {
            m_threatLevelLabel->setText("Normal");
            m_threatLevelLabel->setStyleSheet("QLabel { color: #4f4; }");
        }
    }
    
    // Update system load (simulated based on track count)
    if (m_trackManager) {
        int load = qMin(100, m_trackManager->trackCount() * 5);
        m_systemLoadBar->setValue(load);
        
        if (load > 80) {
            m_systemLoadBar->setStyleSheet(
                "QProgressBar { background-color: #333; border-radius: 3px; }"
                "QProgressBar::chunk { background-color: #f44; border-radius: 3px; }");
        } else if (load > 50) {
            m_systemLoadBar->setStyleSheet(
                "QProgressBar { background-color: #333; border-radius: 3px; }"
                "QProgressBar::chunk { background-color: #fa4; border-radius: 3px; }");
        } else {
            m_systemLoadBar->setStyleSheet(
                "QProgressBar { background-color: #333; border-radius: 3px; }"
                "QProgressBar::chunk { background-color: #4a4; border-radius: 3px; }");
        }
    }
}

void SystemStatusWidget::setSensorStatus(const QString& sensorId, const QString& status, double health) {
    Q_UNUSED(health)
    
    if (sensorId.contains("RADAR", Qt::CaseInsensitive)) {
        updateIndicator(m_radarStatus, status, status == "ONLINE");
    } else if (sensorId.contains("RF", Qt::CaseInsensitive)) {
        updateIndicator(m_rfStatus, status, status == "ONLINE");
    } else if (sensorId.contains("CAM", Qt::CaseInsensitive) || sensorId.contains("DAY", Qt::CaseInsensitive) || sensorId.contains("NIGHT", Qt::CaseInsensitive)) {
        updateIndicator(m_cameraStatus, status, status == "ONLINE");
    }
}

void SystemStatusWidget::setEffectorStatus(const QString& effectorId, const QString& status, bool ready) {
    if (effectorId.contains("JAMMER", Qt::CaseInsensitive) || effectorId.contains("RF", Qt::CaseInsensitive)) {
        updateIndicator(m_jammerStatus, status, ready);
    } else if (effectorId.contains("KINETIC", Qt::CaseInsensitive) || effectorId.contains("MISSILE", Qt::CaseInsensitive)) {
        updateIndicator(m_kineticStatus, status, ready);
    } else if (effectorId.contains("DE", Qt::CaseInsensitive) || effectorId.contains("LASER", Qt::CaseInsensitive)) {
        updateIndicator(m_deStatus, status, ready);
    }
}

void SystemStatusWidget::setVideoStreamCount(int active, int total) {
    m_streamCountLabel->setText(QString("%1 / %2").arg(active).arg(total));
    
    if (active == total && total > 0) {
        m_streamCountLabel->setStyleSheet("QLabel { color: #4f4; }");
    } else if (active > 0) {
        m_streamCountLabel->setStyleSheet("QLabel { color: #ff4; }");
    } else {
        m_streamCountLabel->setStyleSheet("QLabel { color: #888; }");
    }
}

void SystemStatusWidget::setRecordingStatus(bool recording) {
    if (recording) {
        m_recordingLabel->setText("REC");
        m_recordingLabel->setStyleSheet("QLabel { color: #f44; font-weight: bold; }");
    } else {
        m_recordingLabel->setText("OFF");
        m_recordingLabel->setStyleSheet("QLabel { color: #888; }");
    }
}

} // namespace CounterUAS
