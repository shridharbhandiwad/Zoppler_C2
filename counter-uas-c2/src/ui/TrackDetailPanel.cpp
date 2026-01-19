#include "ui/TrackDetailPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QtMath>

namespace CounterUAS {

TrackDetailPanel::TrackDetailPanel(QWidget* parent) : QWidget(parent) {
    // Initialize reference position to default
    m_referencePosition.latitude = 34.0522;
    m_referencePosition.longitude = -118.2437;
    m_referencePosition.altitude = 100.0;
    
    setupUI();
}

void TrackDetailPanel::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(6);
    
    // Panel title
    QLabel* titleLabel = new QLabel("TRACK DETAILS", this);
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 11px; color: #aaa; "
                             "padding: 4px; background-color: #2a2a2a; border-radius: 3px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Create grouped sections
    createIdentityGroup();
    createPositionGroup();
    createKinematicsGroup();
    createThreatGroup();
    createActionsGroup();
    
    mainLayout->addWidget(m_identityGroup);
    mainLayout->addWidget(m_positionGroup);
    mainLayout->addWidget(m_kinematicsGroup);
    mainLayout->addWidget(m_threatGroup);
    mainLayout->addWidget(m_actionsGroup);
    mainLayout->addStretch();
}

void TrackDetailPanel::createIdentityGroup() {
    m_identityGroup = new QGroupBox("Identity", this);
    m_identityGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #8cf; }");
    
    QFormLayout* layout = new QFormLayout(m_identityGroup);
    layout->setSpacing(3);
    layout->setContentsMargins(8, 8, 8, 8);
    
    m_trackIdLabel = new QLabel("-", this);
    m_trackIdLabel->setStyleSheet("QLabel { color: #fff; font-weight: bold; font-size: 12px; }");
    layout->addRow("Track ID:", m_trackIdLabel);
    
    m_classificationLabel = new QLabel("-", this);
    layout->addRow("Class:", m_classificationLabel);
    
    m_stateLabel = new QLabel("-", this);
    layout->addRow("State:", m_stateLabel);
    
    m_sourceLabel = new QLabel("-", this);
    m_sourceLabel->setStyleSheet("QLabel { color: #aaa; font-size: 10px; }");
    layout->addRow("Source:", m_sourceLabel);
}

void TrackDetailPanel::createPositionGroup() {
    m_positionGroup = new QGroupBox("Position", this);
    m_positionGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #8cf; }");
    
    QGridLayout* layout = new QGridLayout(m_positionGroup);
    layout->setSpacing(3);
    layout->setContentsMargins(8, 8, 8, 8);
    
    // Geographic coordinates
    layout->addWidget(new QLabel("Lat:", this), 0, 0);
    m_latLabel = new QLabel("-", this);
    m_latLabel->setStyleSheet("QLabel { color: #ccc; }");
    layout->addWidget(m_latLabel, 0, 1);
    
    layout->addWidget(new QLabel("Lon:", this), 1, 0);
    m_lonLabel = new QLabel("-", this);
    m_lonLabel->setStyleSheet("QLabel { color: #ccc; }");
    layout->addWidget(m_lonLabel, 1, 1);
    
    layout->addWidget(new QLabel("Alt:", this), 2, 0);
    m_altLabel = new QLabel("-", this);
    m_altLabel->setStyleSheet("QLabel { color: #ccc; }");
    layout->addWidget(m_altLabel, 2, 1);
    
    // Add separator line
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #444;");
    layout->addWidget(line, 3, 0, 1, 2);
    
    // Range/Bearing/Elevation (relative to base)
    layout->addWidget(new QLabel("Range:", this), 4, 0);
    m_rangeLabel = new QLabel("-", this);
    m_rangeLabel->setStyleSheet("QLabel { color: #ff8; font-weight: bold; }");
    layout->addWidget(m_rangeLabel, 4, 1);
    
    layout->addWidget(new QLabel("Bearing:", this), 5, 0);
    m_bearingLabel = new QLabel("-", this);
    m_bearingLabel->setStyleSheet("QLabel { color: #ff8; }");
    layout->addWidget(m_bearingLabel, 5, 1);
    
    layout->addWidget(new QLabel("Elevation:", this), 6, 0);
    m_elevationLabel = new QLabel("-", this);
    m_elevationLabel->setStyleSheet("QLabel { color: #ff8; }");
    layout->addWidget(m_elevationLabel, 6, 1);
}

void TrackDetailPanel::createKinematicsGroup() {
    m_kinematicsGroup = new QGroupBox("Kinematics", this);
    m_kinematicsGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #8cf; }");
    
    QFormLayout* layout = new QFormLayout(m_kinematicsGroup);
    layout->setSpacing(3);
    layout->setContentsMargins(8, 8, 8, 8);
    
    m_speedLabel = new QLabel("-", this);
    m_speedLabel->setStyleSheet("QLabel { color: #8f8; font-weight: bold; }");
    layout->addRow("Speed:", m_speedLabel);
    
    m_headingLabel = new QLabel("-", this);
    m_headingLabel->setStyleSheet("QLabel { color: #ccc; }");
    layout->addRow("Heading:", m_headingLabel);
    
    m_climbRateLabel = new QLabel("-", this);
    m_climbRateLabel->setStyleSheet("QLabel { color: #ccc; }");
    layout->addRow("Climb Rate:", m_climbRateLabel);
}

void TrackDetailPanel::createThreatGroup() {
    m_threatGroup = new QGroupBox("Threat Assessment", this);
    m_threatGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #f88; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_threatGroup);
    layout->setSpacing(4);
    layout->setContentsMargins(8, 8, 8, 8);
    
    // Threat level with bar
    QHBoxLayout* threatLayout = new QHBoxLayout();
    QLabel* threatLabel = new QLabel("Level:", this);
    threatLayout->addWidget(threatLabel);
    
    m_threatLevelLabel = new QLabel("-", this);
    m_threatLevelLabel->setStyleSheet("QLabel { color: #fff; font-weight: bold; font-size: 14px; }");
    m_threatLevelLabel->setFixedWidth(30);
    threatLayout->addWidget(m_threatLevelLabel);
    
    m_threatBar = new QProgressBar(this);
    m_threatBar->setRange(0, 5);
    m_threatBar->setValue(0);
    m_threatBar->setTextVisible(false);
    m_threatBar->setFixedHeight(16);
    m_threatBar->setStyleSheet(
        "QProgressBar { background-color: #333; border-radius: 3px; }"
        "QProgressBar::chunk { background-color: #666; border-radius: 3px; }");
    threatLayout->addWidget(m_threatBar);
    layout->addLayout(threatLayout);
    
    // Confidence
    QHBoxLayout* confLayout = new QHBoxLayout();
    confLayout->addWidget(new QLabel("Confidence:", this));
    m_confidenceLabel = new QLabel("-", this);
    m_confidenceLabel->setStyleSheet("QLabel { color: #aaa; }");
    confLayout->addWidget(m_confidenceLabel);
    confLayout->addStretch();
    layout->addLayout(confLayout);
    
    // Proximity to assets
    QHBoxLayout* proxLayout = new QHBoxLayout();
    proxLayout->addWidget(new QLabel("Proximity:", this));
    m_proximityLabel = new QLabel("-", this);
    m_proximityLabel->setStyleSheet("QLabel { color: #aaa; }");
    proxLayout->addWidget(m_proximityLabel);
    proxLayout->addStretch();
    layout->addLayout(proxLayout);
}

void TrackDetailPanel::createActionsGroup() {
    m_actionsGroup = new QGroupBox("Actions", this);
    m_actionsGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; padding: 0 4px; color: #8cf; }");
    
    QVBoxLayout* layout = new QVBoxLayout(m_actionsGroup);
    layout->setSpacing(4);
    layout->setContentsMargins(8, 8, 8, 8);
    
    // Engage and Slew buttons in a row
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(4);
    
    m_engageBtn = new QPushButton("ENGAGE", this);
    m_engageBtn->setStyleSheet(
        "QPushButton { background-color: #8a2a2a; color: white; font-weight: bold; "
        "padding: 8px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #aa3a3a; }"
        "QPushButton:disabled { background-color: #444; color: #888; }");
    connect(m_engageBtn, &QPushButton::clicked, this, [this]() {
        if (m_track) emit engageRequested(m_track->trackId());
    });
    btnLayout->addWidget(m_engageBtn);
    
    m_slewBtn = new QPushButton("SLEW CAM", this);
    m_slewBtn->setStyleSheet(
        "QPushButton { background-color: #2a4a6a; color: white; font-weight: bold; "
        "padding: 8px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #3a5a7a; }"
        "QPushButton:disabled { background-color: #444; color: #888; }");
    connect(m_slewBtn, &QPushButton::clicked, this, [this]() {
        if (m_track) emit slewCameraRequested(m_track->trackId());
    });
    btnLayout->addWidget(m_slewBtn);
    layout->addLayout(btnLayout);
    
    // Classification override
    QHBoxLayout* classifyLayout = new QHBoxLayout();
    classifyLayout->setSpacing(4);
    
    m_classifyCombo = new QComboBox(this);
    m_classifyCombo->addItem("Hostile", static_cast<int>(TrackClassification::Hostile));
    m_classifyCombo->addItem("Friendly", static_cast<int>(TrackClassification::Friendly));
    m_classifyCombo->addItem("Neutral", static_cast<int>(TrackClassification::Neutral));
    m_classifyCombo->addItem("Unknown", static_cast<int>(TrackClassification::Unknown));
    m_classifyCombo->addItem("Pending", static_cast<int>(TrackClassification::Pending));
    m_classifyCombo->setStyleSheet(
        "QComboBox { background-color: #333; color: #ccc; padding: 4px; border-radius: 3px; }");
    classifyLayout->addWidget(m_classifyCombo, 1);
    
    m_classifyBtn = new QPushButton("Classify", this);
    m_classifyBtn->setStyleSheet(
        "QPushButton { background-color: #3a5a3a; color: white; padding: 6px 12px; "
        "border-radius: 4px; }"
        "QPushButton:hover { background-color: #4a6a4a; }"
        "QPushButton:disabled { background-color: #444; color: #888; }");
    connect(m_classifyBtn, &QPushButton::clicked, this, &TrackDetailPanel::onClassificationRequested);
    classifyLayout->addWidget(m_classifyBtn);
    layout->addLayout(classifyLayout);
}

void TrackDetailPanel::setTrack(Track* track) {
    // Disconnect from previous track
    if (m_track) {
        disconnect(m_track, nullptr, this, nullptr);
    }
    
    m_track = track;
    
    if (track) {
        connect(track, &Track::updated, this, &TrackDetailPanel::updateDisplay);
        
        // Set classification combo to current classification
        for (int i = 0; i < m_classifyCombo->count(); ++i) {
            if (m_classifyCombo->itemData(i).toInt() == static_cast<int>(track->classification())) {
                m_classifyCombo->setCurrentIndex(i);
                break;
            }
        }
    }
    
    updateDisplay();
}

void TrackDetailPanel::setReferencePosition(const GeoPosition& pos) {
    m_referencePosition = pos;
    updateDisplay();
}

void TrackDetailPanel::clear() {
    if (m_track) {
        disconnect(m_track, nullptr, this, nullptr);
    }
    m_track = nullptr;
    
    m_trackIdLabel->setText("-");
    m_classificationLabel->setText("-");
    m_stateLabel->setText("-");
    m_sourceLabel->setText("-");
    
    m_latLabel->setText("-");
    m_lonLabel->setText("-");
    m_altLabel->setText("-");
    m_rangeLabel->setText("-");
    m_bearingLabel->setText("-");
    m_elevationLabel->setText("-");
    
    m_speedLabel->setText("-");
    m_headingLabel->setText("-");
    m_climbRateLabel->setText("-");
    
    m_threatLevelLabel->setText("-");
    m_threatBar->setValue(0);
    m_confidenceLabel->setText("-");
    m_proximityLabel->setText("-");
}

void TrackDetailPanel::updateDisplay() {
    if (!m_track) {
        clear();
        return;
    }
    
    // Identity
    m_trackIdLabel->setText(m_track->trackId());
    
    QString classStr = m_track->classificationString();
    m_classificationLabel->setText(classStr);
    switch (m_track->classification()) {
        case TrackClassification::Hostile:
            m_classificationLabel->setStyleSheet("QLabel { color: #f44; font-weight: bold; }");
            break;
        case TrackClassification::Friendly:
            m_classificationLabel->setStyleSheet("QLabel { color: #4f4; font-weight: bold; }");
            break;
        case TrackClassification::Neutral:
            m_classificationLabel->setStyleSheet("QLabel { color: #44f; }");
            break;
        default:
            m_classificationLabel->setStyleSheet("QLabel { color: #ff4; }");
            break;
    }
    
    m_stateLabel->setText(m_track->stateString());
    switch (m_track->state()) {
        case TrackState::Active:
            m_stateLabel->setStyleSheet("QLabel { color: #4f4; }");
            break;
        case TrackState::Coasting:
            m_stateLabel->setStyleSheet("QLabel { color: #ff4; }");
            break;
        default:
            m_stateLabel->setStyleSheet("QLabel { color: #888; }");
            break;
    }
    
    // Source - display detection sources
    QStringList sources;
    if (m_track->hasSource(DetectionSource::Radar)) sources << "Radar";
    if (m_track->hasSource(DetectionSource::RFDetector)) sources << "RF";
    if (m_track->hasSource(DetectionSource::Camera)) sources << "Camera";
    m_sourceLabel->setText(sources.isEmpty() ? "Unknown" : sources.join(", "));
    
    // Position
    GeoPosition pos = m_track->position();
    m_latLabel->setText(QString("%1\u00B0").arg(pos.latitude, 0, 'f', 5));
    m_lonLabel->setText(QString("%1\u00B0").arg(pos.longitude, 0, 'f', 5));
    m_altLabel->setText(QString("%1 m AGL").arg(pos.altitude, 0, 'f', 1));
    
    // Range/Bearing/Elevation
    double range = calculateRange();
    double bearing = calculateBearing();
    double elevation = calculateElevation();
    
    if (range < 1000.0) {
        m_rangeLabel->setText(QString("%1 m").arg(range, 0, 'f', 0));
    } else {
        m_rangeLabel->setText(QString("%1 km").arg(range / 1000.0, 0, 'f', 2));
    }
    
    m_bearingLabel->setText(QString("%1\u00B0").arg(bearing, 0, 'f', 1));
    m_elevationLabel->setText(QString("%1\u00B0").arg(elevation, 0, 'f', 1));
    
    // Kinematics
    VelocityVector vel = m_track->velocity();
    m_speedLabel->setText(QString("%1 m/s").arg(vel.speed(), 0, 'f', 1));
    m_headingLabel->setText(QString("%1\u00B0").arg(vel.heading(), 0, 'f', 0));
    m_climbRateLabel->setText(QString("%1 m/s").arg(vel.climbRate(), 0, 'f', 1));
    
    // Threat Assessment
    int threatLevel = m_track->threatLevel();
    m_threatLevelLabel->setText(QString::number(threatLevel));
    m_threatBar->setValue(threatLevel);
    
    // Color code threat level
    QString threatColor;
    if (threatLevel >= 4) {
        threatColor = "#f44";
        m_threatBar->setStyleSheet(
            "QProgressBar { background-color: #333; border-radius: 3px; }"
            "QProgressBar::chunk { background-color: #f44; border-radius: 3px; }");
    } else if (threatLevel >= 3) {
        threatColor = "#fa4";
        m_threatBar->setStyleSheet(
            "QProgressBar { background-color: #333; border-radius: 3px; }"
            "QProgressBar::chunk { background-color: #fa4; border-radius: 3px; }");
    } else if (threatLevel >= 2) {
        threatColor = "#ff4";
        m_threatBar->setStyleSheet(
            "QProgressBar { background-color: #333; border-radius: 3px; }"
            "QProgressBar::chunk { background-color: #ff4; border-radius: 3px; }");
    } else {
        threatColor = "#4f4";
        m_threatBar->setStyleSheet(
            "QProgressBar { background-color: #333; border-radius: 3px; }"
            "QProgressBar::chunk { background-color: #4f4; border-radius: 3px; }");
    }
    m_threatLevelLabel->setStyleSheet(QString("QLabel { color: %1; font-weight: bold; font-size: 14px; }").arg(threatColor));
    
    m_confidenceLabel->setText(QString("%1%").arg(m_track->classificationConfidence() * 100, 0, 'f', 0));
    
    // Proximity description
    if (range < 500) {
        m_proximityLabel->setText("CRITICAL");
        m_proximityLabel->setStyleSheet("QLabel { color: #f44; font-weight: bold; }");
    } else if (range < 1500) {
        m_proximityLabel->setText("WARNING");
        m_proximityLabel->setStyleSheet("QLabel { color: #fa4; font-weight: bold; }");
    } else if (range < 3000) {
        m_proximityLabel->setText("Approaching");
        m_proximityLabel->setStyleSheet("QLabel { color: #ff4; }");
    } else {
        m_proximityLabel->setText("Distant");
        m_proximityLabel->setStyleSheet("QLabel { color: #888; }");
    }
}

void TrackDetailPanel::onClassificationRequested() {
    if (!m_track) return;
    
    TrackClassification newClass = static_cast<TrackClassification>(
        m_classifyCombo->currentData().toInt());
    emit classificationChanged(m_track->trackId(), newClass);
}

double TrackDetailPanel::calculateRange() const {
    if (!m_track) return 0.0;
    return m_track->distanceTo(m_referencePosition);
}

double TrackDetailPanel::calculateBearing() const {
    if (!m_track) return 0.0;
    
    GeoPosition trackPos = m_track->position();
    
    double lat1 = qDegreesToRadians(m_referencePosition.latitude);
    double lat2 = qDegreesToRadians(trackPos.latitude);
    double dLon = qDegreesToRadians(trackPos.longitude - m_referencePosition.longitude);
    
    double y = std::sin(dLon) * std::cos(lat2);
    double x = std::cos(lat1) * std::sin(lat2) -
               std::sin(lat1) * std::cos(lat2) * std::cos(dLon);
    
    double bearing = qRadiansToDegrees(std::atan2(y, x));
    if (bearing < 0) bearing += 360.0;
    
    return bearing;
}

double TrackDetailPanel::calculateElevation() const {
    if (!m_track) return 0.0;
    
    GeoPosition trackPos = m_track->position();
    double range = calculateRange();
    
    if (range < 0.001) return 0.0;
    
    double verticalDist = trackPos.altitude - m_referencePosition.altitude;
    return qRadiansToDegrees(std::atan2(verticalDist, range));
}

} // namespace CounterUAS
