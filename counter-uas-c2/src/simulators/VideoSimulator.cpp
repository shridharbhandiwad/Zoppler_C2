#include "simulators/VideoSimulator.h"
#include <QPainter>
#include <QDateTime>
#include <QRandomGenerator>
#include <QtMath>

namespace CounterUAS {

VideoSimulator::VideoSimulator(QObject* parent) : QObject(parent), m_timer(new QTimer(this)) {
    connect(m_timer, &QTimer::timeout, this, &VideoSimulator::generateFrame);
}

void VideoSimulator::start() {
    m_timer->start(33);  // ~30 fps
}

void VideoSimulator::stop() {
    m_timer->stop();
}

void VideoSimulator::setResolution(int width, int height) {
    m_width = width;
    m_height = height;
}

void VideoSimulator::generateFrame() {
    QImage frame(m_width, m_height, QImage::Format_RGB888);
    frame.fill(QColor(30, 30, 40));
    
    QPainter painter(&frame);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw grid
    painter.setPen(QPen(QColor(50, 50, 60), 1));
    for (int x = 0; x < m_width; x += 50) {
        painter.drawLine(x, 0, x, m_height);
    }
    for (int y = 0; y < m_height; y += 50) {
        painter.drawLine(0, y, m_width, y);
    }
    
    // Draw horizon line
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawLine(0, m_height / 2, m_width, m_height / 2);
    
    // Draw crosshairs
    painter.setPen(QPen(Qt::green, 2));
    int cx = m_width / 2;
    int cy = m_height / 2;
    painter.drawLine(cx - 30, cy, cx - 10, cy);
    painter.drawLine(cx + 10, cy, cx + 30, cy);
    painter.drawLine(cx, cy - 30, cx, cy - 10);
    painter.drawLine(cx, cy + 10, cx, cy + 30);
    
    // Draw simulated target (moving)
    auto* gen = QRandomGenerator::global();
    double t = m_frameCount * 0.03;
    int targetX = cx + static_cast<int>(100 * std::sin(t));
    int targetY = cy + static_cast<int>(50 * std::cos(t * 0.7));
    
    painter.setPen(QPen(Qt::red, 2));
    painter.drawRect(targetX - 20, targetY - 15, 40, 30);
    painter.drawLine(targetX - 25, targetY, targetX + 25, targetY);
    painter.drawLine(targetX, targetY - 20, targetX, targetY + 20);
    
    // Draw telemetry text
    painter.setPen(Qt::white);
    painter.setFont(QFont("Courier", 10));
    painter.drawText(10, 20, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    painter.drawText(10, 40, QString("Frame: %1").arg(m_frameCount));
    painter.drawText(10, m_height - 10, QString("Az: %1°  El: %2°  Zoom: 1.0x")
                                            .arg(180 + 10 * std::sin(t * 0.1), 0, 'f', 1)
                                            .arg(5 * std::cos(t * 0.15), 0, 'f', 1));
    
    // Simulated camera name
    painter.drawText(m_width - 100, 20, "CAM-001");
    
    painter.end();
    
    m_frameCount++;
    emit frameReady(frame, QDateTime::currentMSecsSinceEpoch());
}

} // namespace CounterUAS
