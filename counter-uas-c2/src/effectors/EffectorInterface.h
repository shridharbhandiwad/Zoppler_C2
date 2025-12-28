#ifndef EFFECTORINTERFACE_H
#define EFFECTORINTERFACE_H

#include <QObject>
#include <QString>
#include <QTimer>
#include "core/Track.h"

namespace CounterUAS {

/**
 * @brief Effector status enum
 */
enum class EffectorStatus {
    Unknown = 0,
    Offline,
    Initializing,
    Ready,
    Engaged,
    Reloading,
    Cooldown,
    Maintenance,
    Fault
};

/**
 * @brief Effector health data
 */
struct EffectorHealth {
    EffectorStatus status = EffectorStatus::Unknown;
    double readiness = 0.0;  // 0-1
    int remainingShots = -1; // -1 for unlimited
    int totalEngagements = 0;
    QString faultMessage;
    QDateTime lastEngagementTime;
    QDateTime lastMaintenanceTime;
};

/**
 * @brief Abstract base class for effector interfaces
 */
class EffectorInterface : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString effectorId READ effectorId CONSTANT)
    Q_PROPERTY(EffectorStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(bool engaged READ isEngaged NOTIFY engagedChanged)
    
public:
    explicit EffectorInterface(const QString& effectorId, QObject* parent = nullptr);
    ~EffectorInterface() override = default;
    
    // Identity
    QString effectorId() const { return m_effectorId; }
    virtual QString effectorType() const = 0;
    virtual QString displayName() const { return m_displayName; }
    void setDisplayName(const QString& name) { m_displayName = name; }
    
    // Position
    void setPosition(const GeoPosition& pos) { m_position = pos; }
    GeoPosition position() const { return m_position; }
    
    // Status
    EffectorStatus status() const { return m_health.status; }
    EffectorHealth health() const { return m_health; }
    virtual bool isReady() const { return m_health.status == EffectorStatus::Ready; }
    virtual bool isEngaged() const { return m_health.status == EffectorStatus::Engaged; }
    
    // Engagement
    virtual bool engage(const GeoPosition& target) = 0;
    virtual void disengage() = 0;
    virtual bool canEngage(const GeoPosition& target) const;
    
    // Range
    virtual double minRange() const { return 0.0; }
    virtual double maxRange() const { return 1000.0; }
    
    // Effectiveness
    virtual double effectiveness() const { return 0.8; }
    
    // Control
    virtual void initialize();
    virtual void shutdown();
    virtual void reset();
    
signals:
    void statusChanged(EffectorStatus status);
    void readyChanged(bool ready);
    void engagedChanged(bool engaged);
    void healthUpdated(const EffectorHealth& health);
    void engagementStarted(const GeoPosition& target);
    void engagementComplete(bool success);
    void fault(const QString& message);
    
protected:
    void setStatus(EffectorStatus status);
    void reportFault(const QString& message);
    double distanceToTarget(const GeoPosition& target) const;
    
    QString m_effectorId;
    QString m_displayName;
    GeoPosition m_position;
    EffectorHealth m_health;
    
    GeoPosition m_currentTarget;
    QTimer* m_engagementTimer = nullptr;
};

} // namespace CounterUAS

Q_DECLARE_METATYPE(CounterUAS::EffectorStatus)

#endif // EFFECTORINTERFACE_H
