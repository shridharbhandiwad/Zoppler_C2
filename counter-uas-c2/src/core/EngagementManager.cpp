#include "core/EngagementManager.h"
#include "core/TrackManager.h"
#include "effectors/EffectorInterface.h"
#include "utils/Logger.h"
#include <QJsonObject>
#include <QBuffer>

namespace CounterUAS {

// EngagementRecord serialization
QJsonObject EngagementRecord::toJson() const {
    QJsonObject obj;
    obj["engagementId"] = engagementId;
    obj["trackId"] = trackId;
    obj["effectorId"] = effectorId;
    obj["effectorType"] = effectorType;
    obj["operatorId"] = operatorId;
    obj["startTime"] = startTime.toString(Qt::ISODateWithMs);
    obj["authorizationTime"] = authorizationTime.toString(Qt::ISODateWithMs);
    obj["executionTime"] = executionTime.toString(Qt::ISODateWithMs);
    obj["completionTime"] = completionTime.toString(Qt::ISODateWithMs);
    obj["state"] = static_cast<int>(state);
    obj["bdaResult"] = static_cast<int>(bdaResult);
    obj["targetPosition"] = targetPosition.toJson();
    obj["targetDistance"] = targetDistance;
    obj["threatLevel"] = threatLevel;
    obj["notes"] = notes;
    obj["wasAborted"] = wasAborted;
    obj["abortReason"] = abortReason;
    return obj;
}

EngagementManager::EngagementManager(TrackManager* trackManager, QObject* parent)
    : QObject(parent)
    , m_trackManager(trackManager)
    , m_authorizationTimer(new QTimer(this))
    , m_completionCheckTimer(new QTimer(this))
{
    m_authorizationTimer->setSingleShot(true);
    connect(m_authorizationTimer, &QTimer::timeout,
            this, &EngagementManager::onAuthorizationTimeout);
    
    m_completionCheckTimer->setInterval(100);
    connect(m_completionCheckTimer, &QTimer::timeout,
            this, &EngagementManager::checkEngagementCompletion);
    
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackDropped,
                this, &EngagementManager::onTrackDropped);
    }
}

EngagementManager::~EngagementManager() {
    m_authorizationTimer->stop();
    m_completionCheckTimer->stop();
}

void EngagementManager::registerEffector(EffectorInterface* effector) {
    if (!effector || m_effectors.contains(effector)) return;
    
    m_effectors.append(effector);
    connect(effector, &EffectorInterface::statusChanged,
            this, [this, effector]() {
                onEffectorStatusChanged(effector->effectorId());
            });
    
    Logger::instance().info("EngagementManager",
                           "Registered effector: " + effector->effectorId());
}

void EngagementManager::unregisterEffector(const QString& effectorId) {
    for (int i = 0; i < m_effectors.size(); ++i) {
        if (m_effectors[i]->effectorId() == effectorId) {
            m_effectors.removeAt(i);
            Logger::instance().info("EngagementManager",
                                   "Unregistered effector: " + effectorId);
            return;
        }
    }
}

EffectorInterface* EngagementManager::effector(const QString& effectorId) const {
    for (auto* e : m_effectors) {
        if (e->effectorId() == effectorId) {
            return e;
        }
    }
    return nullptr;
}

EffectorInterface* EngagementManager::recommendedEffector(const QString& trackId) const {
    if (!m_trackManager) return nullptr;
    
    Track* track = m_trackManager->track(trackId);
    if (!track) return nullptr;
    
    EffectorInterface* best = nullptr;
    double bestScore = 0.0;
    
    for (auto* eff : m_effectors) {
        if (!eff->isReady()) continue;
        
        double score = const_cast<EngagementManager*>(this)->calculateEffectorScore(eff, track);
        if (score > bestScore) {
            bestScore = score;
            best = eff;
        }
    }
    
    return best;
}

void EngagementManager::selectTrack(const QString& trackId) {
    if (!m_trackManager) return;
    
    Track* track = m_trackManager->track(trackId);
    if (!track) {
        Logger::instance().warning("EngagementManager",
                                  "Track not found: " + trackId);
        return;
    }
    
    // Cancel any pending engagement
    if (m_currentState != EngagementState::Idle &&
        m_currentState != EngagementState::Completed &&
        m_currentState != EngagementState::Aborted &&
        m_currentState != EngagementState::Failed) {
        abort("New track selected");
    }
    
    m_selectedTrackId = trackId;
    m_selectedEffectorId.clear();
    
    transitionTo(EngagementState::TrackSelected);
    emit trackSelected(trackId);
    
    // Auto switch video feed to track's camera
    if (!track->associatedCameraId().isEmpty()) {
        emit switchVideoFeed(track->associatedCameraId());
    }
    
    // Auto recommend effector
    if (m_autoRecommend) {
        recommendEffector();
    }
}

void EngagementManager::clearSelection() {
    m_selectedTrackId.clear();
    m_selectedEffectorId.clear();
    transitionTo(EngagementState::Idle);
}

void EngagementManager::selectEffector(const QString& effectorId) {
    if (m_selectedTrackId.isEmpty()) {
        Logger::instance().warning("EngagementManager",
                                  "Cannot select effector without track");
        return;
    }
    
    EffectorInterface* eff = effector(effectorId);
    if (!eff) {
        Logger::instance().warning("EngagementManager",
                                  "Effector not found: " + effectorId);
        return;
    }
    
    m_selectedEffectorId = effectorId;
    emit effectorSelected(effectorId);
    
    if (m_currentState == EngagementState::TrackSelected) {
        transitionTo(EngagementState::EffectorRecommended);
    }
}

void EngagementManager::requestAuthorization() {
    if (m_selectedTrackId.isEmpty() || m_selectedEffectorId.isEmpty()) {
        Logger::instance().warning("EngagementManager",
                                  "Cannot request authorization without track and effector");
        return;
    }
    
    Track* track = m_trackManager->track(m_selectedTrackId);
    EffectorInterface* eff = effector(m_selectedEffectorId);
    
    if (!track || !eff) return;
    
    // Create engagement record
    createEngagementRecord();
    
    // Build authorization request
    m_currentAuthRequest.engagementId = m_currentEngagementId;
    m_currentAuthRequest.trackId = m_selectedTrackId;
    m_currentAuthRequest.effectorId = m_selectedEffectorId;
    m_currentAuthRequest.effectorType = eff->effectorType();
    m_currentAuthRequest.targetPosition = track->position();
    m_currentAuthRequest.distance = track->distanceTo(GeoPosition()); // TODO: base position
    m_currentAuthRequest.threatLevel = track->threatLevel();
    m_currentAuthRequest.classification = track->classification();
    m_currentAuthRequest.recommendationReason = QString("Effector %1 recommended for %2 target at threat level %3")
                                                .arg(eff->effectorId())
                                                .arg(track->classificationString())
                                                .arg(track->threatLevel());
    m_currentAuthRequest.requestTime = QDateTime::currentDateTimeUtc();
    m_currentAuthRequest.timeoutSeconds = m_authTimeoutSeconds;
    
    transitionTo(EngagementState::AwaitingAuthorization);
    
    // Start authorization timeout
    m_authorizationTimer->start(m_authTimeoutSeconds * 1000);
    
    Logger::instance().info("EngagementManager",
                           QString("Authorization requested for engagement %1")
                               .arg(m_currentEngagementId));
    
    emit authorizationRequested(m_currentAuthRequest);
}

void EngagementManager::authorize(const QString& operatorId) {
    if (m_currentState != EngagementState::AwaitingAuthorization) {
        Logger::instance().warning("EngagementManager",
                                  "Not awaiting authorization");
        return;
    }
    
    m_authorizationTimer->stop();
    
    m_currentRecord.operatorId = operatorId;
    m_currentRecord.authorizationTime = QDateTime::currentDateTimeUtc();
    
    transitionTo(EngagementState::Authorized);
    
    Logger::instance().info("EngagementManager",
                           QString("Engagement %1 authorized by %2")
                               .arg(m_currentEngagementId)
                               .arg(operatorId));
    
    emit authorizationGranted(operatorId);
}

void EngagementManager::deny(const QString& reason) {
    if (m_currentState != EngagementState::AwaitingAuthorization) return;
    
    m_authorizationTimer->stop();
    
    m_currentRecord.notes = "Denied: " + reason;
    
    transitionTo(EngagementState::Aborted);
    finalizeEngagement(EngagementState::Aborted);
    
    Logger::instance().info("EngagementManager",
                           QString("Engagement %1 denied: %2")
                               .arg(m_currentEngagementId)
                               .arg(reason));
    
    emit authorizationDenied(reason);
}

void EngagementManager::execute() {
    if (m_currentState != EngagementState::Authorized) {
        Logger::instance().warning("EngagementManager",
                                  "Cannot execute without authorization");
        return;
    }
    
    EffectorInterface* eff = effector(m_selectedEffectorId);
    Track* track = m_trackManager->track(m_selectedTrackId);
    
    if (!eff || !track) {
        transitionTo(EngagementState::Failed);
        finalizeEngagement(EngagementState::Failed);
        emit engagementFailed(m_currentEngagementId, "Effector or track unavailable");
        return;
    }
    
    if (!eff->isReady()) {
        transitionTo(EngagementState::Failed);
        finalizeEngagement(EngagementState::Failed);
        emit engagementFailed(m_currentEngagementId, "Effector not ready");
        return;
    }
    
    // Execute engagement
    m_currentRecord.executionTime = QDateTime::currentDateTimeUtc();
    track->setEngaged(true);
    
    bool success = eff->engage(track->position());
    
    if (success) {
        transitionTo(EngagementState::Engaging);
        m_completionCheckTimer->start();
        
        Logger::instance().info("EngagementManager",
                               QString("Engagement %1 executing")
                                   .arg(m_currentEngagementId));
        
        emit engagementStarted(m_currentEngagementId);
    } else {
        transitionTo(EngagementState::Failed);
        finalizeEngagement(EngagementState::Failed);
        emit engagementFailed(m_currentEngagementId, "Effector engagement failed");
    }
}

void EngagementManager::abort(const QString& reason) {
    if (m_currentState == EngagementState::Idle ||
        m_currentState == EngagementState::Completed ||
        m_currentState == EngagementState::Aborted ||
        m_currentState == EngagementState::Failed) {
        return;
    }
    
    m_authorizationTimer->stop();
    m_completionCheckTimer->stop();
    
    // Disengage effector if currently engaging
    if (m_currentState == EngagementState::Engaging) {
        EffectorInterface* eff = effector(m_selectedEffectorId);
        if (eff) {
            eff->disengage();
        }
    }
    
    m_currentRecord.wasAborted = true;
    m_currentRecord.abortReason = reason;
    
    transitionTo(EngagementState::Aborted);
    finalizeEngagement(EngagementState::Aborted);
    
    Logger::instance().info("EngagementManager",
                           QString("Engagement %1 aborted: %2")
                               .arg(m_currentEngagementId)
                               .arg(reason));
    
    emit engagementAborted(m_currentEngagementId, reason);
}

void EngagementManager::setBDAResult(BDAResult result) {
    m_currentRecord.bdaResult = result;
}

void EngagementManager::addNote(const QString& note) {
    if (!m_currentRecord.notes.isEmpty()) {
        m_currentRecord.notes += "\n";
    }
    m_currentRecord.notes += note;
}

void EngagementManager::setVideoSnapshot(const QImage& snapshot) {
    m_currentRecord.videoSnapshot = snapshot;
    m_currentAuthRequest.videoThumbnail = snapshot.scaled(160, 120, Qt::KeepAspectRatio);
}

EngagementRecord* EngagementManager::currentEngagement() {
    if (m_currentEngagementId.isEmpty()) return nullptr;
    return &m_currentRecord;
}

EngagementRecord* EngagementManager::engagement(const QString& engagementId) {
    for (auto& record : m_history) {
        if (record.engagementId == engagementId) {
            return &record;
        }
    }
    return nullptr;
}

void EngagementManager::onEffectorStatusChanged(const QString& effectorId) {
    if (effectorId == m_selectedEffectorId) {
        EffectorInterface* eff = effector(effectorId);
        if (eff && !eff->isReady() && m_currentState == EngagementState::Engaging) {
            checkEngagementCompletion();
        }
    }
}

void EngagementManager::onTrackDropped(const QString& trackId) {
    if (trackId == m_selectedTrackId) {
        if (m_currentState == EngagementState::Engaging) {
            // Track dropped while engaging - possible success
            m_currentRecord.bdaResult = BDAResult::AssessmentPending;
            checkEngagementCompletion();
        } else if (m_currentState != EngagementState::Idle &&
                   m_currentState != EngagementState::Completed) {
            abort("Target track dropped");
        }
    }
}

void EngagementManager::onAuthorizationTimeout() {
    if (m_currentState == EngagementState::AwaitingAuthorization) {
        Logger::instance().warning("EngagementManager",
                                  "Authorization timeout for engagement " +
                                  m_currentEngagementId);
        
        transitionTo(EngagementState::Aborted);
        finalizeEngagement(EngagementState::Aborted);
        
        emit authorizationTimeout();
    }
}

void EngagementManager::checkEngagementCompletion() {
    if (m_currentState != EngagementState::Engaging) {
        m_completionCheckTimer->stop();
        return;
    }
    
    EffectorInterface* eff = effector(m_selectedEffectorId);
    if (!eff) {
        transitionTo(EngagementState::Failed);
        finalizeEngagement(EngagementState::Failed);
        emit engagementFailed(m_currentEngagementId, "Effector lost");
        return;
    }
    
    // Check if effector has completed its cycle
    if (!eff->isEngaged()) {
        m_completionCheckTimer->stop();
        
        transitionTo(EngagementState::Completed);
        m_currentRecord.completionTime = QDateTime::currentDateTimeUtc();
        
        // Check track status for BDA
        Track* track = m_trackManager->track(m_selectedTrackId);
        if (!track || track->state() == TrackState::Dropped) {
            if (m_currentRecord.bdaResult == BDAResult::Unknown) {
                m_currentRecord.bdaResult = BDAResult::AssessmentPending;
            }
        }
        
        finalizeEngagement(EngagementState::Completed);
        
        Logger::instance().info("EngagementManager",
                               QString("Engagement %1 completed")
                                   .arg(m_currentEngagementId));
        
        emit engagementCompleted(m_currentEngagementId, m_currentRecord.bdaResult);
    }
}

void EngagementManager::transitionTo(EngagementState newState) {
    if (m_currentState != newState) {
        m_currentState = newState;
        m_currentRecord.state = newState;
        emit stateChanged(newState);
    }
}

QString EngagementManager::generateEngagementId() {
    return QString("ENG-%1").arg(m_nextEngagementNumber++, 6, 10, QChar('0'));
}

void EngagementManager::recommendEffector() {
    EffectorInterface* best = recommendedEffector(m_selectedTrackId);
    
    if (best) {
        m_selectedEffectorId = best->effectorId();
        
        Track* track = m_trackManager->track(m_selectedTrackId);
        QString reason = QString("Recommended based on target range and %1 capability")
                             .arg(best->effectorType());
        
        transitionTo(EngagementState::EffectorRecommended);
        emit effectorRecommended(best->effectorId(), reason);
    }
}

void EngagementManager::createEngagementRecord() {
    m_currentEngagementId = generateEngagementId();
    
    Track* track = m_trackManager->track(m_selectedTrackId);
    EffectorInterface* eff = effector(m_selectedEffectorId);
    
    m_currentRecord = EngagementRecord();
    m_currentRecord.engagementId = m_currentEngagementId;
    m_currentRecord.trackId = m_selectedTrackId;
    m_currentRecord.effectorId = m_selectedEffectorId;
    m_currentRecord.effectorType = eff ? eff->effectorType() : "";
    m_currentRecord.startTime = QDateTime::currentDateTimeUtc();
    m_currentRecord.state = EngagementState::Idle;
    
    if (track) {
        m_currentRecord.targetPosition = track->position();
        m_currentRecord.threatLevel = track->threatLevel();
    }
    
    emit engagementChanged(m_currentEngagementId);
}

void EngagementManager::finalizeEngagement(EngagementState finalState) {
    m_currentRecord.state = finalState;
    
    // Update statistics
    m_stats.totalEngagements++;
    switch (finalState) {
        case EngagementState::Completed:
            m_stats.successfulEngagements++;
            break;
        case EngagementState::Aborted:
            m_stats.abortedEngagements++;
            break;
        case EngagementState::Failed:
            m_stats.failedEngagements++;
            break;
        default:
            break;
    }
    
    // Calculate average engagement time
    if (m_currentRecord.completionTime.isValid()) {
        qint64 duration = m_currentRecord.startTime.msecsTo(m_currentRecord.completionTime);
        double total = m_stats.avgEngagementTimeMs * (m_stats.totalEngagements - 1) + duration;
        m_stats.avgEngagementTimeMs = total / m_stats.totalEngagements;
    }
    
    // Add to history
    m_history.append(m_currentRecord);
    
    // Clear current engagement state
    Track* track = m_trackManager->track(m_selectedTrackId);
    if (track) {
        track->setEngaged(false);
    }
    
    m_selectedTrackId.clear();
    m_selectedEffectorId.clear();
    m_currentEngagementId.clear();
}

double EngagementManager::calculateEffectorScore(EffectorInterface* effector, Track* track) {
    if (!effector || !track) return 0.0;
    
    double score = 0.0;
    
    // Check if effector is ready
    if (!effector->isReady()) return 0.0;
    
    // Check range
    double distance = track->distanceTo(GeoPosition()); // TODO: use base position
    double maxRange = effector->maxRange();
    double minRange = effector->minRange();
    
    if (distance < minRange || distance > maxRange) {
        return 0.0;  // Out of range
    }
    
    // Range score (prefer middle of engagement envelope)
    double rangeCenter = (maxRange + minRange) / 2.0;
    double rangeDiff = std::abs(distance - rangeCenter);
    double rangeScore = 1.0 - (rangeDiff / (maxRange - minRange));
    score += rangeScore * 0.4;
    
    // Effectiveness score
    score += effector->effectiveness() * 0.3;
    
    // Availability score (is it loaded/charged?)
    score += (effector->isReady() ? 0.2 : 0.0);
    
    // Prefer RF jamming for unknown/pending classifications
    if (effector->effectorType() == "RF_JAMMER") {
        if (track->classification() == TrackClassification::Pending ||
            track->classification() == TrackClassification::Unknown) {
            score += 0.1;
        }
    }
    
    return score;
}

} // namespace CounterUAS
