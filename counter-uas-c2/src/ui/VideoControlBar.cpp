#include "ui/VideoControlBar.h"
#include <QHBoxLayout>

namespace CounterUAS {

VideoControlBar::VideoControlBar(QWidget* parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    m_cameraCombo = new QComboBox(this);
    layout->addWidget(m_cameraCombo);
    
    m_recordBtn = new QPushButton("Record", this);
    m_recordBtn->setCheckable(true);
    layout->addWidget(m_recordBtn);
    
    m_snapshotBtn = new QPushButton("Snapshot", this);
    layout->addWidget(m_snapshotBtn);
    
    connect(m_recordBtn, &QPushButton::toggled, this, &VideoControlBar::recordToggled);
    connect(m_snapshotBtn, &QPushButton::clicked, this, &VideoControlBar::snapshotRequested);
    connect(m_cameraCombo, &QComboBox::currentTextChanged, this, &VideoControlBar::cameraSelected);
}

} // namespace CounterUAS
