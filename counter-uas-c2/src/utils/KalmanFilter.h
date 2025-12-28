#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <QObject>

namespace CounterUAS {

/**
 * @brief 2D Kalman filter for position smoothing
 */
class KalmanFilter2D {
public:
    KalmanFilter2D();
    
    void initialize(double x, double y);
    void update(double measX, double measY);
    void predict(double dt = 0.1);
    
    double stateX() const { return m_x; }
    double stateY() const { return m_y; }
    double velocityX() const { return m_vx; }
    double velocityY() const { return m_vy; }
    
    // Configuration
    void setProcessNoise(double q) { m_processNoise = q; }
    void setMeasurementNoise(double r) { m_measurementNoise = r; }
    
private:
    // State: [x, y, vx, vy]
    double m_x = 0.0;
    double m_y = 0.0;
    double m_vx = 0.0;
    double m_vy = 0.0;
    
    // Covariance matrix (simplified)
    double m_px = 1.0;
    double m_py = 1.0;
    double m_pvx = 1.0;
    double m_pvy = 1.0;
    
    double m_processNoise = 0.1;
    double m_measurementNoise = 1.0;
    
    bool m_initialized = false;
};

/**
 * @brief Alpha-Beta filter for simpler smoothing
 */
class AlphaBetaFilter {
public:
    AlphaBetaFilter(double alpha = 0.85, double beta = 0.005);
    
    void initialize(double value);
    void update(double measurement, double dt = 0.1);
    
    double value() const { return m_value; }
    double velocity() const { return m_velocity; }
    
    void setAlpha(double alpha) { m_alpha = alpha; }
    void setBeta(double beta) { m_beta = beta; }
    
private:
    double m_value = 0.0;
    double m_velocity = 0.0;
    double m_alpha;
    double m_beta;
    bool m_initialized = false;
};

} // namespace CounterUAS

#endif // KALMANFILTER_H
