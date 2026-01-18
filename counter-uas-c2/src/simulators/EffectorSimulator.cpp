/**
 * @file EffectorSimulator.cpp
 * @brief Implementation of complete effector simulation
 */

#include "simulators/EffectorSimulator.h"
#include "core/EngagementManager.h"
#include "effectors/RFJammer.h"
#include "effectors/KineticInterceptor.h"
#include "effectors/DirectedEnergySystem.h"
#include "utils/Logger.h"
#include <QDateTime>
#include <QtMath>

namespace CounterUAS {

EffectorSimulator::EffectorSimulator(EngagementManager* manager, QObject* parent)
    : QObject(parent)
    , m_engagementManager(manager)
    , m_updateTimer(new QTimer(this))
{
    connect(m_updateTimer, &QTimer::timeout, this, &EffectorSimulator::updateSimulation);
}

EffectorSimulator::~EffectorSimulator() {
    stop();
}

void EffectorSimulator::setEngagementManager(EngagementManager* manager) {
    m_engagementManager = manager;
}

void EffectorSimulator::start() {
    if (m_running) return;
    
    m_running = true;
    m_updateTimer->start(1000 / m_updateRateHz);
    
    Logger::instance().info("EffectorSimulator", "Simulation started");
    emit simulationStarted();
}

void EffectorSimulator::stop() {
    if (!m_running) return;
    
    m_running = false;
    m_updateTimer->stop();
    
    Logger::instance().info("EffectorSimulator", "Simulation stopped");
    emit simulationStopped();
}

void EffectorSimulator::setUpdateRate(int hz) {
    m_updateRateHz = qBound(1, hz, 100);
    if (m_running) {
        m_updateTimer->setInterval(1000 / m_updateRateHz);
    }
}

void EffectorSimulator::registerRFJammer(RFJammer* jammer) {
    if (!jammer) return;
    
    QString id = jammer->effectorId();
    m_rfJammers[id] = jammer;
    m_rfJammerStates[id] = RFJammerSimState();
    
    connect(jammer, &RFJammer::jamming, this, [this, id](bool active, double power) {
        if (m_rfJammerStates.contains(id)) {
            m_rfJammerStates[id].active = active;
            m_rfJammerStates[id].currentPowerW = power;
            if (active) {
                m_rfJammerStates[id].engagementStartTime = QDateTime::currentMSecsSinceEpoch();
            }
        }
    });
    
    Logger::instance().info("EffectorSimulator", "Registered RF Jammer: " + id);
}

void EffectorSimulator::registerKineticInterceptor(KineticInterceptor* interceptor) {
    if (!interceptor) return;
    
    QString id = interceptor->effectorId();
    m_kineticInterceptors[id] = interceptor;
    
    KineticSimState state;
    state.launcherPosition = interceptor->position();
    state.roundsRemaining = interceptor->remainingRounds();
    m_kineticStates[id] = state;
    
    connect(interceptor, &KineticInterceptor::launchPhaseChanged,
            this, [this, id](KineticInterceptor::LaunchPhase phase) {
        if (!m_kineticStates.contains(id)) return;
        
        auto& state = m_kineticStates[id];
        switch (phase) {
            case KineticInterceptor::LaunchPhase::Idle:
                state.phase = KineticSimState::Phase::Idle;
                break;
            case KineticInterceptor::LaunchPhase::Arming:
                state.phase = KineticSimState::Phase::Arming;
                break;
            case KineticInterceptor::LaunchPhase::Launching:
                state.phase = KineticSimState::Phase::Launching;
                state.launchTime = QDateTime::currentMSecsSinceEpoch();
                state.currentPosition = state.launcherPosition;
                break;
            case KineticInterceptor::LaunchPhase::InFlight:
                state.phase = KineticSimState::Phase::InFlight;
                state.speed = 100.0;  // m/s
                break;
            case KineticInterceptor::LaunchPhase::Terminal:
                state.phase = KineticSimState::Phase::Terminal;
                break;
            case KineticInterceptor::LaunchPhase::Complete:
                state.phase = KineticSimState::Phase::Complete;
                break;
        }
        emit kineticStateChanged(id, state);
    });
    
    connect(interceptor, &KineticInterceptor::roundsFired,
            this, [this, id](int remaining) {
        if (m_kineticStates.contains(id)) {
            m_kineticStates[id].roundsRemaining = remaining;
        }
    });
    
    Logger::instance().info("EffectorSimulator", "Registered Kinetic Interceptor: " + id);
}

void EffectorSimulator::registerDirectedEnergy(DirectedEnergySystem* de) {
    if (!de) return;
    
    QString id = de->effectorId();
    m_deSystems[id] = de;
    
    DESimState state;
    state.requiredDwellS = de->config().dwellTimeRequiredS;
    m_deStates[id] = state;
    
    connect(de, &DirectedEnergySystem::powerChanged,
            this, [this, id](double kw) {
        if (m_deStates.contains(id)) {
            m_deStates[id].currentPowerKW = kw;
            m_deStates[id].active = kw > 0.0;
        }
    });
    
    connect(de, &DirectedEnergySystem::trackingStatus,
            this, [this, id](bool tracking, double dwellTime) {
        if (m_deStates.contains(id)) {
            m_deStates[id].tracking = tracking;
            m_deStates[id].dwellTimeS = dwellTime;
            if (tracking && m_deStates[id].trackingStartTime == 0) {
                m_deStates[id].trackingStartTime = QDateTime::currentMSecsSinceEpoch();
            }
        }
    });
    
    connect(de, &DirectedEnergySystem::targetEffect,
            this, [this, id]() {
        if (m_deStates.contains(id)) {
            m_deStates[id].effectAchieved = true;
        }
    });
    
    Logger::instance().info("EffectorSimulator", "Registered Directed Energy: " + id);
}

void EffectorSimulator::unregisterEffector(const QString& effectorId) {
    m_rfJammers.remove(effectorId);
    m_rfJammerStates.remove(effectorId);
    m_kineticInterceptors.remove(effectorId);
    m_kineticStates.remove(effectorId);
    m_deSystems.remove(effectorId);
    m_deStates.remove(effectorId);
}

void EffectorSimulator::clearEffectors() {
    m_rfJammers.clear();
    m_rfJammerStates.clear();
    m_kineticInterceptors.clear();
    m_kineticStates.clear();
    m_deSystems.clear();
    m_deStates.clear();
}

RFJammerSimState EffectorSimulator::rfJammerState(const QString& id) const {
    return m_rfJammerStates.value(id);
}

KineticSimState EffectorSimulator::kineticState(const QString& id) const {
    return m_kineticStates.value(id);
}

DESimState EffectorSimulator::deState(const QString& id) const {
    return m_deStates.value(id);
}

void EffectorSimulator::simulateRFJamEngagement(const QString& jammerId, const GeoPosition& target) {
    auto* jammer = m_rfJammers.value(jammerId);
    if (!jammer) return;
    
    jammer->engage(target);
    
    m_stats.totalSimulatedEngagements++;
    m_stats.rfJammerEngagements++;
}

void EffectorSimulator::simulateKineticLaunch(const QString& interceptorId, const GeoPosition& target) {
    auto* interceptor = m_kineticInterceptors.value(interceptorId);
    if (!interceptor) return;
    
    if (m_kineticStates.contains(interceptorId)) {
        m_kineticStates[interceptorId].targetPosition = target;
    }
    
    interceptor->engage(target);
    
    m_stats.totalSimulatedEngagements++;
    m_stats.kineticEngagements++;
}

void EffectorSimulator::simulateDEEngagement(const QString& deId, const GeoPosition& target) {
    auto* de = m_deSystems.value(deId);
    if (!de) return;
    
    if (m_deStates.contains(deId)) {
        m_deStates[deId].targetPosition = target;
        m_deStates[deId].trackingStartTime = 0;
        m_deStates[deId].effectAchieved = false;
    }
    
    de->engage(target);
    
    m_stats.totalSimulatedEngagements++;
    m_stats.deEngagements++;
}

void EffectorSimulator::updateSimulation() {
    updateRFJammerSimulation();
    updateKineticSimulation();
    updateDESimulation();
}

void EffectorSimulator::updateRFJammerSimulation() {
    double dt = 1.0 / m_updateRateHz;
    
    for (auto it = m_rfJammerStates.begin(); it != m_rfJammerStates.end(); ++it) {
        const QString& id = it.key();
        RFJammerSimState& state = it.value();
        auto* jammer = m_rfJammers.value(id);
        
        if (!jammer || !state.active) continue;
        
        // Update active frequencies from jammer
        state.activeFrequencies = jammer->activeFrequencies();
        
        // Calculate signal effectiveness based on power and distance
        double baseEffectiveness = state.currentPowerW / 100.0;  // Normalize to max power
        
        // Add weather and random factors if realistic mode
        if (m_realisticMode) {
            baseEffectiveness *= m_weatherFactor;
            baseEffectiveness *= (0.9 + QRandomGenerator::global()->generateDouble() * 0.2);
            
            // Random interference spike
            if (QRandomGenerator::global()->generateDouble() < 0.1) {
                state.targetInterferenceLevel = qMin(100, state.targetInterferenceLevel + 
                    QRandomGenerator::global()->bounded(10, 30));
            }
        }
        
        // Update interference level (ramps up over time)
        qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - state.engagementStartTime;
        double rampFactor = qMin(1.0, elapsed / 5000.0);  // 5 second ramp
        state.targetInterferenceLevel = static_cast<int>(baseEffectiveness * 100 * rampFactor);
        
        // Check if link is disrupted (>70% interference)
        state.targetLinkDisrupted = state.targetInterferenceLevel > 70;
        state.signalEffectiveness = baseEffectiveness * rampFactor;
        
        emit rfJammerStateChanged(id, state);
        
        if (state.targetLinkDisrupted) {
            emit targetEffectSimulated(id, "", state.signalEffectiveness);
        }
    }
}

void EffectorSimulator::updateKineticSimulation() {
    double dt = 1.0 / m_updateRateHz;
    
    for (auto it = m_kineticStates.begin(); it != m_kineticStates.end(); ++it) {
        const QString& id = it.key();
        KineticSimState& state = it.value();
        
        if (state.phase == KineticSimState::Phase::InFlight) {
            simulateInterceptorFlight(id, dt);
        }
    }
}

void EffectorSimulator::simulateInterceptorFlight(const QString& id, double dt) {
    if (!m_kineticStates.contains(id)) return;
    
    KineticSimState& state = m_kineticStates[id];
    
    // Calculate direction to target
    double dLat = state.targetPosition.latitude - state.currentPosition.latitude;
    double dLon = state.targetPosition.longitude - state.currentPosition.longitude;
    double dAlt = state.targetPosition.altitude - state.currentPosition.altitude;
    
    double northOffset = dLat * 111000.0;
    double eastOffset = dLon * 111000.0 * std::cos(qDegreesToRadians(state.currentPosition.latitude));
    
    double horizontalDist = std::sqrt(northOffset * northOffset + eastOffset * eastOffset);
    double totalDist = std::sqrt(horizontalDist * horizontalDist + dAlt * dAlt);
    
    state.distanceToTarget = totalDist;
    
    if (totalDist < 1.0) {
        // Arrived at target
        checkInterceptCompletion(id);
        return;
    }
    
    // Normalize direction
    double invDist = 1.0 / totalDist;
    state.velocity.north = northOffset * invDist * state.speed;
    state.velocity.east = eastOffset * invDist * state.speed;
    state.velocity.down = -dAlt * invDist * state.speed;
    
    // Update position
    double move = state.speed * dt;
    state.currentPosition.latitude += (state.velocity.north * dt) / 111000.0;
    state.currentPosition.longitude += (state.velocity.east * dt) / 
        (111000.0 * std::cos(qDegreesToRadians(state.currentPosition.latitude)));
    state.currentPosition.altitude -= state.velocity.down * dt;
    
    // Add some noise in realistic mode
    if (m_realisticMode) {
        auto* gen = QRandomGenerator::global();
        state.currentPosition.latitude += (gen->generateDouble() - 0.5) * 0.000001;
        state.currentPosition.longitude += (gen->generateDouble() - 0.5) * 0.000001;
    }
    
    emit interceptorInFlight(id, state.currentPosition, state.distanceToTarget);
    emit kineticStateChanged(id, state);
    
    // Check if close enough for terminal phase
    if (totalDist < 50.0 && state.phase != KineticSimState::Phase::Terminal) {
        state.phase = KineticSimState::Phase::Terminal;
        emit kineticStateChanged(id, state);
    }
    
    // Auto-complete when very close
    if (totalDist < 5.0) {
        checkInterceptCompletion(id);
    }
}

void EffectorSimulator::checkInterceptCompletion(const QString& id) {
    if (!m_kineticStates.contains(id)) return;
    
    KineticSimState& state = m_kineticStates[id];
    auto* interceptor = m_kineticInterceptors.value(id);
    
    // Calculate intercept probability
    double prob = 0.85;  // Base probability
    
    if (m_realisticMode) {
        prob *= m_weatherFactor;
        
        // Random failure check
        if (QRandomGenerator::global()->generateDouble() < m_failureRate) {
            prob *= 0.5;
        }
    }
    
    state.interceptSuccess = QRandomGenerator::global()->generateDouble() < prob;
    state.phase = KineticSimState::Phase::Complete;
    
    SimulatedEngagementResult result;
    result.effectorId = id;
    result.effectorType = "KINETIC";
    result.targetPosition = state.targetPosition;
    result.success = state.interceptSuccess;
    result.effectivenessAchieved = state.interceptSuccess ? 1.0 : 0.0;
    result.resultDetails = state.interceptSuccess ? "Target intercepted" : "Intercept missed";
    result.timestamp = QDateTime::currentMSecsSinceEpoch();
    
    if (state.interceptSuccess) {
        m_stats.successfulEngagements++;
    }
    
    emit kineticStateChanged(id, state);
    emit engagementSimulated(result);
    
    Logger::instance().info("EffectorSimulator",
        QString("Kinetic intercept %1: %2")
            .arg(id)
            .arg(state.interceptSuccess ? "SUCCESS" : "MISS"));
}

void EffectorSimulator::updateDESimulation() {
    double dt = 1.0 / m_updateRateHz;
    
    for (auto it = m_deStates.begin(); it != m_deStates.end(); ++it) {
        const QString& id = it.key();
        DESimState& state = it.value();
        auto* de = m_deSystems.value(id);
        
        if (!de || !state.active || !state.tracking) continue;
        
        // Calculate tracking error
        if (m_realisticMode) {
            // Simulate tracking jitter
            state.trackingError = QRandomGenerator::global()->generateDouble() * 2.0;  // 0-2 mrad
            
            // Weather affects tracking
            state.trackingError *= (2.0 - m_weatherFactor);
        } else {
            state.trackingError = 0.5;  // Ideal tracking
        }
        
        // Update dwell time
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        if (state.trackingStartTime > 0) {
            state.dwellTimeS = (now - state.trackingStartTime) / 1000.0;
        }
        
        emit deStateChanged(id, state);
        
        // Check if effect is achieved
        if (!state.effectAchieved && state.dwellTimeS >= state.requiredDwellS) {
            double effectProb = calculateDEEffect(de, state.targetPosition, state.dwellTimeS);
            
            if (QRandomGenerator::global()->generateDouble() < effectProb) {
                state.effectAchieved = true;
                
                SimulatedEngagementResult result;
                result.effectorId = id;
                result.effectorType = "DIRECTED_ENERGY";
                result.targetPosition = state.targetPosition;
                result.success = true;
                result.effectivenessAchieved = effectProb;
                result.resultDetails = QString("Target effect achieved after %1s dwell")
                                           .arg(state.dwellTimeS, 0, 'f', 1);
                result.timestamp = now;
                
                m_stats.successfulEngagements++;
                emit engagementSimulated(result);
                emit targetEffectSimulated(id, "", effectProb);
                
                Logger::instance().info("EffectorSimulator",
                    QString("DE %1: Target effect achieved").arg(id));
            }
        }
    }
}

double EffectorSimulator::calculateRFJamEffectiveness(RFJammer* jammer, const GeoPosition& target) {
    if (!jammer) return 0.0;
    
    // Calculate distance
    Track temp("temp");
    temp.setPosition(jammer->position());
    double distance = temp.distanceTo(target);
    
    // Effectiveness drops with distance squared (inverse square law)
    double maxRange = jammer->maxRange();
    if (distance > maxRange) return 0.0;
    
    double rangeFactor = 1.0 - std::pow(distance / maxRange, 2);
    
    // Power factor
    double powerFactor = jammer->currentPowerW() / jammer->config().maxPowerW;
    
    return rangeFactor * powerFactor * m_weatherFactor;
}

double EffectorSimulator::calculateInterceptProbability(KineticInterceptor* interceptor,
                                                        const GeoPosition& target,
                                                        double distanceToTarget) {
    if (!interceptor) return 0.0;
    
    double basePk = interceptor->config().interceptProbability;
    
    // Probability decreases with range
    double maxRange = interceptor->maxRange();
    double rangeFactor = 1.0 - (distanceToTarget / maxRange) * 0.3;
    
    // Weather factor
    double weatherPenalty = 1.0 - (1.0 - m_weatherFactor) * 0.4;
    
    return basePk * rangeFactor * weatherPenalty;
}

double EffectorSimulator::calculateDEEffect(DirectedEnergySystem* de,
                                           const GeoPosition& target,
                                           double dwellTime) {
    if (!de) return 0.0;
    
    // Calculate distance
    Track temp("temp");
    temp.setPosition(de->position());
    double distance = temp.distanceTo(target);
    
    // Effectiveness drops with distance
    double maxRange = de->maxRange();
    if (distance > maxRange) return 0.0;
    
    double rangeFactor = 1.0 - (distance / maxRange) * 0.5;
    
    // Power factor
    double powerFactor = de->currentPowerKW() / de->config().maxPowerKW;
    
    // Dwell time factor (more dwell = more effect)
    double requiredDwell = de->config().dwellTimeRequiredS;
    double dwellFactor = qMin(1.5, dwellTime / requiredDwell);
    
    // Weather significantly affects DE
    double weatherPenalty = m_weatherFactor * m_weatherFactor;  // Squared for DE
    
    return rangeFactor * powerFactor * dwellFactor * weatherPenalty * 0.75;  // Base 75% Pk
}

void EffectorSimulator::onRFJammerEngaged(bool engaged) {
    Q_UNUSED(engaged)
}

void EffectorSimulator::onKineticLaunched(int remaining) {
    Q_UNUSED(remaining)
}

void EffectorSimulator::onDEPowerChanged(double kw) {
    Q_UNUSED(kw)
}

} // namespace CounterUAS
