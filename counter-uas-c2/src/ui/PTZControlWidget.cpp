#include "ui/PTZControlWidget.h"
#include <QGridLayout>
#include <QVBoxLayout>

namespace CounterUAS {

PTZControlWidget::PTZControlWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QGridLayout* ptzLayout = new QGridLayout();
    
    m_upBtn = new QPushButton("▲", this);
    m_downBtn = new QPushButton("▼", this);
    m_leftBtn = new QPushButton("◄", this);
    m_rightBtn = new QPushButton("►", this);
    QPushButton* homeBtn = new QPushButton("⌂", this);
    
    ptzLayout->addWidget(m_upBtn, 0, 1);
    ptzLayout->addWidget(m_leftBtn, 1, 0);
    ptzLayout->addWidget(homeBtn, 1, 1);
    ptzLayout->addWidget(m_rightBtn, 1, 2);
    ptzLayout->addWidget(m_downBtn, 2, 1);
    
    mainLayout->addLayout(ptzLayout);
    
    m_zoomSlider = new QSlider(Qt::Horizontal, this);
    m_zoomSlider->setRange(1, 30);
    m_zoomSlider->setValue(1);
    mainLayout->addWidget(m_zoomSlider);
    
    connect(m_upBtn, &QPushButton::clicked, this, &PTZControlWidget::tiltUp);
    connect(m_downBtn, &QPushButton::clicked, this, &PTZControlWidget::tiltDown);
    connect(m_leftBtn, &QPushButton::clicked, this, &PTZControlWidget::panLeft);
    connect(m_rightBtn, &QPushButton::clicked, this, &PTZControlWidget::panRight);
    connect(homeBtn, &QPushButton::clicked, this, &PTZControlWidget::goHome);
}

} // namespace CounterUAS
