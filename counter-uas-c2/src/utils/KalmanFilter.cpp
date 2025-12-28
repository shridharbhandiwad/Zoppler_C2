#include "utils/KalmanFilter.h"
#include <cmath>

namespace CounterUAS {

// KalmanFilter2D implementation
KalmanFilter2D::KalmanFilter2D() {}

void KalmanFilter2D::initialize(double x, double y) {
    m_x = x;
    m_y = y;
    m_vx = 0.0;
    m_vy = 0.0;
    m_px = 1.0;
    m_py = 1.0;
    m_pvx = 1.0;
    m_pvy = 1.0;
    m_initialized = true;
}

void KalmanFilter2D::predict(double dt) {
    if (!m_initialized) return;
    
    // Predict state
    m_x += m_vx * dt;
    m_y += m_vy * dt;
    
    // Predict covariance
    m_px += m_pvx * dt * dt + m_processNoise;
    m_py += m_pvy * dt * dt + m_processNoise;
    m_pvx += m_processNoise;
    m_pvy += m_processNoise;
}

void KalmanFilter2D::update(double measX, double measY) {
    if (!m_initialized) {
        initialize(measX, measY);
        return;
    }
    
    // Predict first
    predict();
    
    // Kalman gain
    double kx = m_px / (m_px + m_measurementNoise);
    double ky = m_py / (m_py + m_measurementNoise);
    
    // Update state
    double innovationX = measX - m_x;
    double innovationY = measY - m_y;
    
    // Also estimate velocity from innovation
    double dt = 0.1;  // Assumed update interval
    double kvx = 0.5;  // Velocity gain
    
    m_x += kx * innovationX;
    m_y += ky * innovationY;
    m_vx += kvx * innovationX / dt;
    m_vy += kvx * innovationY / dt;
    
    // Update covariance
    m_px *= (1.0 - kx);
    m_py *= (1.0 - ky);
}

// AlphaBetaFilter implementation
AlphaBetaFilter::AlphaBetaFilter(double alpha, double beta)
    : m_alpha(alpha), m_beta(beta)
{}

void AlphaBetaFilter::initialize(double value) {
    m_value = value;
    m_velocity = 0.0;
    m_initialized = true;
}

void AlphaBetaFilter::update(double measurement, double dt) {
    if (!m_initialized) {
        initialize(measurement);
        return;
    }
    
    // Predict
    double predicted = m_value + m_velocity * dt;
    
    // Residual
    double residual = measurement - predicted;
    
    // Update
    m_value = predicted + m_alpha * residual;
    m_velocity = m_velocity + (m_beta / dt) * residual;
}

} // namespace CounterUAS
