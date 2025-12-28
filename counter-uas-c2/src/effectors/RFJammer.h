#ifndef RFJAMMER_H
#define RFJAMMER_H

#include "effectors/EffectorInterface.h"
#include <QTcpSocket>

namespace CounterUAS {

/**
 * @brief RF Jammer configuration
 */
struct RFJammerConfig {
    QString host = "127.0.0.1";
    int port = 5010;
    
    // Frequency bands
    double minFrequencyMHz = 2400.0;
    double maxFrequencyMHz = 5800.0;
    QList<double> jamFrequenciesMHz = {2400.0, 2450.0, 5200.0, 5800.0};
    
    // Power settings
    double maxPowerW = 100.0;
    double defaultPowerW = 50.0;
    
    // Timing
    int engagementTimeMs = 30000;  // Default 30 seconds
    int cooldownTimeMs = 5000;
    
    // Range
    double effectiveRangeM = 2000.0;
    double minimumRangeM = 50.0;
};

/**
 * @brief RF Jammer effector for disrupting drone control links
 */
class RFJammer : public EffectorInterface {
    Q_OBJECT
    
public:
    explicit RFJammer(const QString& effectorId, QObject* parent = nullptr);
    ~RFJammer() override;
    
    QString effectorType() const override { return "RF_JAMMER"; }
    
    // Configuration
    void setConfig(const RFJammerConfig& config);
    RFJammerConfig config() const { return m_config; }
    
    // EffectorInterface implementation
    bool engage(const GeoPosition& target) override;
    void disengage() override;
    
    double minRange() const override { return m_config.minimumRangeM; }
    double maxRange() const override { return m_config.effectiveRangeM; }
    double effectiveness() const override { return 0.85; }  // RF jamming effectiveness
    
    // Jammer-specific controls
    void setJamFrequencies(const QList<double>& frequenciesMHz);
    void setPower(double watts);
    void setEngagementTime(int ms);
    
    // Status
    double currentPowerW() const { return m_currentPowerW; }
    int remainingEngagementTimeMs() const;
    QList<double> activeFrequencies() const { return m_activeFrequencies; }
    
signals:
    void jamming(bool active, double powerW);
    void frequencyChanged(const QList<double>& frequencies);
    
private slots:
    void onEngagementTimeout();
    void onCooldownComplete();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    
private:
    void sendJamCommand(bool engage, const QList<double>& frequencies, double power);
    bool connectToJammer();
    void disconnectFromJammer();
    
    RFJammerConfig m_config;
    QTcpSocket* m_socket;
    QTimer* m_engagementTimer;
    QTimer* m_cooldownTimer;
    
    double m_currentPowerW = 0.0;
    QList<double> m_activeFrequencies;
    qint64 m_engagementStartTime = 0;
};

} // namespace CounterUAS

#endif // RFJAMMER_H
