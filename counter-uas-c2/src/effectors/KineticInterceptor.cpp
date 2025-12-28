#include "effectors/KineticInterceptor.h"
#include "utils/Logger.h"
#include <QRandomGenerator>

namespace CounterUAS {

KineticInterceptor::KineticInterceptor(const QString& effectorId, QObject* parent)
    : EffectorInterface(effectorId, parent)
    , m_armingTimer(new QTimer(this))
    , m_launchTimer(new QTimer(this))
    , m_flightTimer(new QTimer(this))
    , m_reloadTimer(new QTimer(this))
{
    m_armingTimer->setSingleShot(true);
    m_launchTimer->setSingleShot(true);
    m_flightTimer->setSingleShot(true);
    m_reloadTimer->setSingleShot(true);
    
    connect(m_armingTimer, &QTimer::timeout, this, &KineticInterceptor::onArmingComplete);
    connect(m_launchTimer, &QTimer::timeout, this, &KineticInterceptor::onLaunchComplete);
    connect(m_flightTimer, &QTimer::timeout, this, &KineticInterceptor::onTerminalPhase);
    connect(m_reloadTimer, &QTimer::timeout, this, &KineticInterceptor::onReloadComplete);
    
    setDisplayName("Kinetic Interceptor");
    m_remainingRounds = m_config.magazineCapacity;
}

KineticInterceptor::~KineticInterceptor() {
    disengage();
}

void KineticInterceptor::setConfig(const KineticInterceptorConfig& config) {
    m_config = config;
    m_remainingRounds = config.magazineCapacity;
    m_health.remainingShots = m_remainingRounds;
}

bool KineticInterceptor::engage(const GeoPosition& target) {
    if (!canEngage(target)) {
        Logger::instance().warning("KineticInterceptor",
                                  m_effectorId + " cannot engage - not ready or out of range");
        return false;
    }
    
    if (m_remainingRounds <= 0) {
        Logger::instance().warning("KineticInterceptor",
                                  m_effectorId + " cannot engage - no rounds remaining");
        return false;
    }
    
    m_currentTarget = target;
    
    // Start launch sequence
    setStatus(EffectorStatus::Engaged);
    transitionPhase(LaunchPhase::Arming);
    
    m_armingTimer->start(500);  // Arming takes 500ms
    
    Logger::instance().info("KineticInterceptor",
                           QString("%1 engaging target at %2m range")
                               .arg(m_effectorId)
                               .arg(distanceToTarget(target), 0, 'f', 0));
    
    emit engagementStarted(target);
    
    return true;
}

void KineticInterceptor::disengage() {
    if (!isEngaged()) return;
    
    // Can only abort during arming phase
    if (m_launchPhase == LaunchPhase::Arming) {
        m_armingTimer->stop();
        transitionPhase(LaunchPhase::Idle);
        setStatus(EffectorStatus::Ready);
        
        Logger::instance().info("KineticInterceptor",
                               m_effectorId + " engagement aborted");
        
        emit engagementComplete(false);
    } else {
        Logger::instance().warning("KineticInterceptor",
                                  m_effectorId + " cannot abort - interceptor already launched");
    }
}

bool KineticInterceptor::isReady() const {
    return m_health.status == EffectorStatus::Ready && m_remainingRounds > 0;
}

void KineticInterceptor::reload() {
    if (m_health.status == EffectorStatus::Engaged) {
        Logger::instance().warning("KineticInterceptor",
                                  m_effectorId + " cannot reload while engaged");
        return;
    }
    
    if (m_remainingRounds >= m_config.magazineCapacity) {
        Logger::instance().info("KineticInterceptor",
                               m_effectorId + " magazine already full");
        return;
    }
    
    setStatus(EffectorStatus::Reloading);
    m_reloadTimer->start(m_config.reloadTimeMs);
    
    Logger::instance().info("KineticInterceptor", m_effectorId + " reloading");
    emit reloadStarted();
}

void KineticInterceptor::onArmingComplete() {
    transitionPhase(LaunchPhase::Launching);
    
    sendLaunchCommand(m_currentTarget);
    
    m_launchTimer->start(m_config.launchTimeMs);
    
    Logger::instance().info("KineticInterceptor", m_effectorId + " arming complete, launching");
}

void KineticInterceptor::onLaunchComplete() {
    // Decrement round count
    m_remainingRounds--;
    m_health.remainingShots = m_remainingRounds;
    m_health.totalEngagements++;
    m_health.lastEngagementTime = QDateTime::currentDateTimeUtc();
    
    emit roundsFired(m_remainingRounds);
    emit healthUpdated(m_health);
    
    transitionPhase(LaunchPhase::InFlight);
    
    // Calculate flight time based on range
    double range = distanceToTarget(m_currentTarget);
    double avgSpeed = 100.0;  // m/s
    int flightTime = static_cast<int>(range / avgSpeed * 1000);
    flightTime = qBound(1000, flightTime, m_config.flightTimeMs);
    
    m_flightTimer->start(flightTime);
    
    Logger::instance().info("KineticInterceptor",
                           QString("%1 interceptor in flight, ETA %2ms")
                               .arg(m_effectorId)
                               .arg(flightTime));
}

void KineticInterceptor::onTerminalPhase() {
    transitionPhase(LaunchPhase::Terminal);
    
    Logger::instance().info("KineticInterceptor", m_effectorId + " terminal phase");
    
    // Short delay for terminal phase
    QTimer::singleShot(500, this, &KineticInterceptor::onInterceptComplete);
}

void KineticInterceptor::onInterceptComplete() {
    simulateIntercept();
}

void KineticInterceptor::onReloadComplete() {
    m_remainingRounds = m_config.magazineCapacity;
    m_health.remainingShots = m_remainingRounds;
    
    setStatus(EffectorStatus::Ready);
    
    Logger::instance().info("KineticInterceptor",
                           QString("%1 reload complete, %2 rounds available")
                               .arg(m_effectorId)
                               .arg(m_remainingRounds));
    
    emit reloadComplete();
    emit healthUpdated(m_health);
}

void KineticInterceptor::transitionPhase(LaunchPhase phase) {
    m_launchPhase = phase;
    emit launchPhaseChanged(phase);
}

void KineticInterceptor::sendLaunchCommand(const GeoPosition& target) {
    // In a real implementation, send command to launcher
    Q_UNUSED(target)
}

void KineticInterceptor::simulateIntercept() {
    // Simulate intercept probability
    double roll = QRandomGenerator::global()->generateDouble();
    bool success = roll < m_config.interceptProbability;
    
    transitionPhase(LaunchPhase::Complete);
    
    Logger::instance().info("KineticInterceptor",
                           QString("%1 intercept %2")
                               .arg(m_effectorId)
                               .arg(success ? "SUCCESS" : "MISS"));
    
    emit interceptResult(success);
    emit engagementComplete(success);
    
    // Return to ready state
    QTimer::singleShot(1000, this, [this]() {
        transitionPhase(LaunchPhase::Idle);
        
        if (m_remainingRounds > 0) {
            setStatus(EffectorStatus::Ready);
        } else {
            setStatus(EffectorStatus::Reloading);
            // Auto-reload
            m_reloadTimer->start(m_config.reloadTimeMs);
            emit reloadStarted();
        }
    });
}

} // namespace CounterUAS
