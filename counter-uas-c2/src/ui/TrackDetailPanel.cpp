#include "ui/TrackDetailPanel.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>

namespace CounterUAS {

TrackDetailPanel::TrackDetailPanel(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QFormLayout* formLayout = new QFormLayout();
    
    m_idLabel = new QLabel("-");
    m_classLabel = new QLabel("-");
    m_posLabel = new QLabel("-");
    m_velLabel = new QLabel("-");
    m_threatLabel = new QLabel("-");
    m_stateLabel = new QLabel("-");
    
    formLayout->addRow("Track ID:", m_idLabel);
    formLayout->addRow("Classification:", m_classLabel);
    formLayout->addRow("Position:", m_posLabel);
    formLayout->addRow("Velocity:", m_velLabel);
    formLayout->addRow("Threat Level:", m_threatLabel);
    formLayout->addRow("State:", m_stateLabel);
    
    mainLayout->addLayout(formLayout);
    
    QPushButton* engageBtn = new QPushButton("Engage");
    QPushButton* slewBtn = new QPushButton("Slew Camera");
    
    connect(engageBtn, &QPushButton::clicked, this, [this]() {
        if (m_track) emit engageRequested(m_track->trackId());
    });
    connect(slewBtn, &QPushButton::clicked, this, [this]() {
        if (m_track) emit slewCameraRequested(m_track->trackId());
    });
    
    mainLayout->addWidget(engageBtn);
    mainLayout->addWidget(slewBtn);
    mainLayout->addStretch();
}

void TrackDetailPanel::setTrack(Track* track) {
    m_track = track;
    updateDisplay();
    
    if (track) {
        connect(track, &Track::updated, this, &TrackDetailPanel::updateDisplay);
    }
}

void TrackDetailPanel::clear() {
    m_track = nullptr;
    m_idLabel->setText("-");
    m_classLabel->setText("-");
    m_posLabel->setText("-");
    m_velLabel->setText("-");
    m_threatLabel->setText("-");
    m_stateLabel->setText("-");
}

void TrackDetailPanel::updateDisplay() {
    if (!m_track) return;
    
    m_idLabel->setText(m_track->trackId());
    m_classLabel->setText(m_track->classificationString());
    
    GeoPosition pos = m_track->position();
    m_posLabel->setText(QString("%1, %2, %3m")
                           .arg(pos.latitude, 0, 'f', 5)
                           .arg(pos.longitude, 0, 'f', 5)
                           .arg(pos.altitude, 0, 'f', 1));
    
    VelocityVector vel = m_track->velocity();
    m_velLabel->setText(QString("%1 m/s @ %2°")
                           .arg(vel.speed(), 0, 'f', 1)
                           .arg(vel.heading(), 0, 'f', 0));
    
    m_threatLabel->setText(QString::number(m_track->threatLevel()));
    m_stateLabel->setText(m_track->stateString());
}

} // namespace CounterUAS
