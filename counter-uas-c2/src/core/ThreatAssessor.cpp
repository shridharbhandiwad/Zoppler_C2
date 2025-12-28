#include "core/ThreatAssessor.h"
#include "core/TrackManager.h"
#include "utils/Logger.h"
#include <algorithm>
#include <cmath>

namespace CounterUAS {

// DefendedAsset serialization
QJsonObject DefendedAsset::toJson() const {
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = name;
    obj["position"] = position.toJson();
    obj["criticalRadiusM"] = criticalRadiusM;
    obj["warningRadiusM"] = warningRadiusM;
    obj["priorityLevel"] = priorityLevel;
    return obj;
}

DefendedAsset DefendedAsset::fromJson(const QJsonObject& json) {
    DefendedAsset asset;
    asset.id = json["id"].toString();
    asset.name = json["name"].toString();
    asset.position = GeoPosition::fromJson(json["position"].toObject());
    asset.criticalRadiusM = json["criticalRadiusM"].toDouble(500.0);
    asset.warningRadiusM = json["warningRadiusM"].toDouble(1500.0);
    asset.priorityLevel = json["priorityLevel"].toInt(1);
    return asset;
}

// ThreatRule serialization
QJsonObject ThreatRule::toJson() const {
    QJsonObject obj;
    obj["id"] = id;
    obj["name"] = name;
    obj["description"] = description;
    obj["enabled"] = enabled;
    obj["minProximityM"] = minProximityM;
    obj["maxProximityM"] = maxProximityM;
    obj["minVelocityMps"] = minVelocityMps;
    obj["maxVelocityMps"] = maxVelocityMps;
    obj["minHeadingToAssetDeg"] = minHeadingToAssetDeg;
    obj["maxHeadingToAssetDeg"] = maxHeadingToAssetDeg;
    obj["requiresVisualConfirmation"] = requiresVisualConfirmation;
    obj["requiresRFDetection"] = requiresRFDetection;
    obj["threatLevelIncrease"] = threatLevelIncrease;
    obj["setThreatLevel"] = setThreatLevel;
    obj["forceClassification"] = static_cast<int>(forceClassification);
    obj["generateAlert"] = generateAlert;
    obj["alertMessage"] = alertMessage;
    return obj;
}

ThreatRule ThreatRule::fromJson(const QJsonObject& json) {
    ThreatRule rule;
    rule.id = json["id"].toString();
    rule.name = json["name"].toString();
    rule.description = json["description"].toString();
    rule.enabled = json["enabled"].toBool(true);
    rule.minProximityM = json["minProximityM"].toDouble(-1);
    rule.maxProximityM = json["maxProximityM"].toDouble(-1);
    rule.minVelocityMps = json["minVelocityMps"].toDouble(-1);
    rule.maxVelocityMps = json["maxVelocityMps"].toDouble(-1);
    rule.minHeadingToAssetDeg = json["minHeadingToAssetDeg"].toDouble(-1);
    rule.maxHeadingToAssetDeg = json["maxHeadingToAssetDeg"].toDouble(-1);
    rule.requiresVisualConfirmation = json["requiresVisualConfirmation"].toBool();
    rule.requiresRFDetection = json["requiresRFDetection"].toBool();
    rule.threatLevelIncrease = json["threatLevelIncrease"].toInt();
    rule.setThreatLevel = json["setThreatLevel"].toInt(-1);
    rule.forceClassification = static_cast<TrackClassification>(json["forceClassification"].toInt());
    rule.generateAlert = json["generateAlert"].toBool();
    rule.alertMessage = json["alertMessage"].toString();
    return rule;
}

// ThreatAlert serialization
QJsonObject ThreatAlert::toJson() const {
    QJsonObject obj;
    obj["alertId"] = alertId;
    obj["trackId"] = trackId;
    obj["message"] = message;
    obj["threatLevel"] = threatLevel;
    obj["timestamp"] = timestamp.toString(Qt::ISODateWithMs);
    obj["acknowledged"] = acknowledged;
    obj["acknowledgedBy"] = acknowledgedBy;
    if (acknowledgedTime.isValid()) {
        obj["acknowledgedTime"] = acknowledgedTime.toString(Qt::ISODateWithMs);
    }
    return obj;
}

// ThreatAssessor implementation
ThreatAssessor::ThreatAssessor(TrackManager* trackManager, QObject* parent)
    : QObject(parent)
    , m_trackManager(trackManager)
    , m_assessmentTimer(new QTimer(this))
{
    connect(m_assessmentTimer, &QTimer::timeout, 
            this, &ThreatAssessor::performAssessmentCycle);
    
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackCreated,
                this, &ThreatAssessor::onTrackCreated);
        connect(m_trackManager, &TrackManager::trackUpdated,
                this, &ThreatAssessor::onTrackUpdated);
    }
    
    loadDefaultRules();
}

ThreatAssessor::~ThreatAssessor() {
    stop();
}

void ThreatAssessor::setConfig(const ThreatAssessorConfig& config) {
    m_config = config;
    if (m_running) {
        m_assessmentTimer->setInterval(m_config.assessmentIntervalMs);
    }
}

void ThreatAssessor::start() {
    if (m_running) return;
    
    m_assessmentTimer->setInterval(m_config.assessmentIntervalMs);
    m_assessmentTimer->start();
    m_running = true;
    
    Logger::instance().info("ThreatAssessor", "Started with interval: " +
                           QString::number(m_config.assessmentIntervalMs) + " ms");
}

void ThreatAssessor::stop() {
    if (!m_running) return;
    
    m_assessmentTimer->stop();
    m_running = false;
    
    Logger::instance().info("ThreatAssessor", "Stopped");
}

void ThreatAssessor::addDefendedAsset(const DefendedAsset& asset) {
    m_assets.append(asset);
    Logger::instance().info("ThreatAssessor", "Added defended asset: " + asset.name);
}

void ThreatAssessor::removeDefendedAsset(const QString& assetId) {
    m_assets.erase(std::remove_if(m_assets.begin(), m_assets.end(),
        [&assetId](const DefendedAsset& a) { return a.id == assetId; }), m_assets.end());
}

void ThreatAssessor::clearDefendedAssets() {
    m_assets.clear();
}

DefendedAsset* ThreatAssessor::nearestAsset(const GeoPosition& pos) {
    if (m_assets.isEmpty()) return nullptr;
    
    Track tempTrack("temp");
    tempTrack.setPosition(pos);
    
    DefendedAsset* nearest = nullptr;
    double minDist = std::numeric_limits<double>::max();
    
    for (auto& asset : m_assets) {
        double dist = tempTrack.distanceTo(asset.position);
        if (dist < minDist) {
            minDist = dist;
            nearest = &asset;
        }
    }
    
    return nearest;
}

void ThreatAssessor::addRule(const ThreatRule& rule) {
    m_rules.append(rule);
}

void ThreatAssessor::removeRule(const QString& ruleId) {
    m_rules.erase(std::remove_if(m_rules.begin(), m_rules.end(),
        [&ruleId](const ThreatRule& r) { return r.id == ruleId; }), m_rules.end());
}

void ThreatAssessor::setRuleEnabled(const QString& ruleId, bool enabled) {
    for (auto& rule : m_rules) {
        if (rule.id == ruleId) {
            rule.enabled = enabled;
            break;
        }
    }
}

void ThreatAssessor::clearRules() {
    m_rules.clear();
}

void ThreatAssessor::loadDefaultRules() {
    m_rules.clear();
    
    // Rule 1: Critical proximity - immediate threat
    ThreatRule criticalProximity;
    criticalProximity.id = "RULE-001";
    criticalProximity.name = "Critical Proximity";
    criticalProximity.description = "Track within critical radius of defended asset";
    criticalProximity.maxProximityM = 500.0;
    criticalProximity.setThreatLevel = 5;
    criticalProximity.forceClassification = TrackClassification::Hostile;
    criticalProximity.generateAlert = true;
    criticalProximity.alertMessage = "CRITICAL: Track %TRACK% within critical radius!";
    m_rules.append(criticalProximity);
    
    // Rule 2: Warning proximity
    ThreatRule warningProximity;
    warningProximity.id = "RULE-002";
    warningProximity.name = "Warning Proximity";
    warningProximity.description = "Track within warning radius";
    warningProximity.minProximityM = 500.0;
    warningProximity.maxProximityM = 1500.0;
    warningProximity.threatLevelIncrease = 2;
    warningProximity.generateAlert = true;
    warningProximity.alertMessage = "WARNING: Track %TRACK% approaching defended area";
    m_rules.append(warningProximity);
    
    // Rule 3: High velocity approach
    ThreatRule highVelocity;
    highVelocity.id = "RULE-003";
    highVelocity.name = "High Velocity Approach";
    highVelocity.description = "Fast moving track heading toward asset";
    highVelocity.minVelocityMps = 20.0;
    highVelocity.minHeadingToAssetDeg = 0.0;
    highVelocity.maxHeadingToAssetDeg = 30.0;
    highVelocity.threatLevelIncrease = 1;
    m_rules.append(highVelocity);
    
    // Rule 4: RF confirmed drone
    ThreatRule rfConfirmed;
    rfConfirmed.id = "RULE-004";
    rfConfirmed.name = "RF Confirmed";
    rfConfirmed.description = "Track confirmed by RF detection";
    rfConfirmed.requiresRFDetection = true;
    rfConfirmed.threatLevelIncrease = 1;
    rfConfirmed.forceClassification = TrackClassification::Hostile;
    m_rules.append(rfConfirmed);
    
    // Rule 5: Visual confirmation required
    ThreatRule visualUnconfirmed;
    visualUnconfirmed.id = "RULE-005";
    visualUnconfirmed.name = "Unconfirmed Visual";
    visualUnconfirmed.description = "High threat without visual confirmation";
    visualUnconfirmed.requiresVisualConfirmation = false;  // Triggers when NO visual
    visualUnconfirmed.minProximityM = 0;
    visualUnconfirmed.maxProximityM = 2000.0;
    visualUnconfirmed.generateAlert = true;
    visualUnconfirmed.alertMessage = "Track %TRACK% requires visual confirmation";
    m_rules.append(visualUnconfirmed);
    
    Logger::instance().info("ThreatAssessor", 
                           QString("Loaded %1 default rules").arg(m_rules.size()));
}

void ThreatAssessor::assessTrack(const QString& trackId) {
    if (!m_trackManager) return;
    
    Track* track = m_trackManager->track(trackId);
    if (!track || track->state() == TrackState::Dropped) return;
    
    int oldThreatLevel = track->threatLevel();
    int newThreatLevel = calculateThreatLevel(track);
    TrackClassification newClassification = track->classification();
    
    applyRules(track, newThreatLevel, newClassification);
    
    // Apply changes
    if (newThreatLevel != oldThreatLevel) {
        m_trackManager->setTrackThreatLevel(trackId, newThreatLevel);
        emit threatLevelChanged(trackId, oldThreatLevel, newThreatLevel);
        
        if (newThreatLevel >= m_config.highThreatThreshold) {
            emit highThreatDetected(trackId);
            
            // Auto-slew camera to threat if enabled
            if (m_config.autoSlewToHighestThreat && !track->isVisuallyTracked()) {
                emit slewCameraRequest(QString(), track->position());
            }
        }
    }
    
    if (newClassification != track->classification() &&
        newClassification != TrackClassification::Unknown) {
        m_trackManager->setTrackClassification(trackId, newClassification);
    }
}

void ThreatAssessor::assessAllTracks() {
    if (!m_trackManager) return;
    
    QList<Track*> tracks = m_trackManager->allTracks();
    for (Track* track : tracks) {
        if (track->state() != TrackState::Dropped) {
            assessTrack(track->trackId());
        }
    }
    
    updateMetrics();
    emit assessmentComplete();
}

QList<Track*> ThreatAssessor::threatQueue() const {
    if (!m_trackManager) return QList<Track*>();
    
    QList<Track*> queue;
    QList<Track*> allTracks = m_trackManager->allTracks();
    
    for (Track* t : allTracks) {
        if (t->state() != TrackState::Dropped &&
            (t->classification() == TrackClassification::Hostile ||
             t->classification() == TrackClassification::Pending)) {
            queue.append(t);
        }
    }
    
    // Sort by threat level (descending), then by distance to nearest asset
    std::sort(queue.begin(), queue.end(), [this](Track* a, Track* b) {
        if (a->threatLevel() != b->threatLevel()) {
            return a->threatLevel() > b->threatLevel();
        }
        // Equal threat level, sort by proximity
        DefendedAsset* assetA = nullptr;
        DefendedAsset* assetB = nullptr;
        double distA = const_cast<ThreatAssessor*>(this)->calculateProximityToAssets(a, &assetA);
        double distB = const_cast<ThreatAssessor*>(this)->calculateProximityToAssets(b, &assetB);
        return distA < distB;
    });
    
    return queue;
}

Track* ThreatAssessor::highestUnconfirmedThreat() const {
    QList<Track*> queue = threatQueue();
    for (Track* t : queue) {
        if (!t->isVisuallyTracked()) {
            return t;
        }
    }
    return nullptr;
}

QList<ThreatAlert> ThreatAssessor::unacknowledgedAlerts() const {
    QList<ThreatAlert> result;
    for (const auto& alert : m_alerts) {
        if (!alert.acknowledged) {
            result.append(alert);
        }
    }
    return result;
}

void ThreatAssessor::acknowledgeAlert(const QString& alertId, const QString& operatorId) {
    for (auto& alert : m_alerts) {
        if (alert.alertId == alertId) {
            alert.acknowledged = true;
            alert.acknowledgedBy = operatorId;
            alert.acknowledgedTime = QDateTime::currentDateTimeUtc();
            emit alertAcknowledged(alertId);
            break;
        }
    }
}

void ThreatAssessor::clearAlerts() {
    m_alerts.clear();
}

void ThreatAssessor::onTrackUpdated(const QString& trackId) {
    assessTrack(trackId);
}

void ThreatAssessor::onTrackCreated(const QString& trackId) {
    assessTrack(trackId);
}

void ThreatAssessor::performAssessmentCycle() {
    assessAllTracks();
    m_metrics.lastAssessmentMs = QDateTime::currentMSecsSinceEpoch();
    emit metricsUpdated(m_metrics);
}

int ThreatAssessor::calculateThreatLevel(Track* track) {
    if (m_assets.isEmpty()) {
        return track->threatLevel();
    }
    
    int level = 1;  // Base level
    
    // Factor 1: Classification
    switch (track->classification()) {
        case TrackClassification::Hostile:
            level += 2;
            break;
        case TrackClassification::Pending:
            level += 1;
            break;
        case TrackClassification::Friendly:
            return 1;  // Friendlies are always low threat
        default:
            break;
    }
    
    // Factor 2: Proximity to defended assets
    DefendedAsset* nearestAsset = nullptr;
    double distance = calculateProximityToAssets(track, &nearestAsset);
    
    if (nearestAsset) {
        if (distance < nearestAsset->criticalRadiusM) {
            level += 3;
        } else if (distance < nearestAsset->warningRadiusM) {
            level += 2;
        } else if (distance < nearestAsset->warningRadiusM * 2) {
            level += 1;
        }
        
        // Factor 3: Heading toward asset
        if (isHeadingTowardAsset(track, *nearestAsset)) {
            level += 1;
        }
    }
    
    // Factor 4: Velocity
    double speed = track->velocity().speed();
    if (speed > 30.0) {
        level += 1;
    }
    
    // Factor 5: Classification confidence
    if (track->classificationConfidence() < 0.5) {
        level = std::max(1, level - 1);  // Reduce if uncertain
    }
    
    return std::min(5, std::max(1, level));
}

void ThreatAssessor::applyRules(Track* track, int& threatLevel, 
                                 TrackClassification& classification) {
    DefendedAsset* nearestAsset = nullptr;
    double proximity = calculateProximityToAssets(track, &nearestAsset);
    double velocity = track->velocity().speed();
    bool hasRF = track->hasSource(DetectionSource::RFDetector);
    bool hasVisual = track->isVisuallyTracked();
    
    double headingToAsset = -1;
    if (nearestAsset) {
        double trackHeading = track->velocity().heading();
        double bearingToAsset = track->bearingTo(nearestAsset->position);
        headingToAsset = std::abs(trackHeading - bearingToAsset);
        if (headingToAsset > 180.0) {
            headingToAsset = 360.0 - headingToAsset;
        }
    }
    
    for (const ThreatRule& rule : m_rules) {
        if (!rule.enabled) continue;
        
        bool ruleMatches = true;
        
        // Check proximity conditions
        if (rule.minProximityM >= 0 && proximity < rule.minProximityM) {
            ruleMatches = false;
        }
        if (rule.maxProximityM >= 0 && proximity > rule.maxProximityM) {
            ruleMatches = false;
        }
        
        // Check velocity conditions
        if (rule.minVelocityMps >= 0 && velocity < rule.minVelocityMps) {
            ruleMatches = false;
        }
        if (rule.maxVelocityMps >= 0 && velocity > rule.maxVelocityMps) {
            ruleMatches = false;
        }
        
        // Check heading conditions
        if (rule.minHeadingToAssetDeg >= 0 && headingToAsset >= 0) {
            if (headingToAsset < rule.minHeadingToAssetDeg ||
                headingToAsset > rule.maxHeadingToAssetDeg) {
                ruleMatches = false;
            }
        }
        
        // Check RF detection requirement
        if (rule.requiresRFDetection && !hasRF) {
            ruleMatches = false;
        }
        
        // Check visual confirmation requirement
        if (rule.requiresVisualConfirmation && !hasVisual) {
            ruleMatches = false;
        }
        
        if (ruleMatches) {
            // Apply rule actions
            if (rule.setThreatLevel >= 0) {
                threatLevel = rule.setThreatLevel;
            } else {
                threatLevel += rule.threatLevelIncrease;
            }
            
            if (rule.forceClassification != TrackClassification::Unknown) {
                classification = rule.forceClassification;
            }
            
            if (rule.generateAlert) {
                generateAlert(track, rule);
            }
        }
    }
    
    threatLevel = std::min(5, std::max(1, threatLevel));
}

double ThreatAssessor::calculateProximityToAssets(Track* track, DefendedAsset** nearestAsset) {
    if (m_assets.isEmpty()) {
        if (nearestAsset) *nearestAsset = nullptr;
        return std::numeric_limits<double>::max();
    }
    
    double minDist = std::numeric_limits<double>::max();
    DefendedAsset* nearest = nullptr;
    
    for (auto& asset : m_assets) {
        double dist = track->distanceTo(asset.position);
        if (dist < minDist) {
            minDist = dist;
            nearest = &asset;
        }
    }
    
    if (nearestAsset) *nearestAsset = nearest;
    return minDist;
}

bool ThreatAssessor::isHeadingTowardAsset(Track* track, const DefendedAsset& asset) {
    double trackHeading = track->velocity().heading();
    double bearingToAsset = track->bearingTo(asset.position);
    
    double diff = std::abs(trackHeading - bearingToAsset);
    if (diff > 180.0) {
        diff = 360.0 - diff;
    }
    
    return diff < m_config.headingToleranceDeg;
}

void ThreatAssessor::generateAlert(Track* track, const ThreatRule& rule) {
    // Check for duplicate recent alerts
    for (const auto& existing : m_alerts) {
        if (existing.trackId == track->trackId() && 
            !existing.acknowledged &&
            existing.timestamp.secsTo(QDateTime::currentDateTimeUtc()) < 30) {
            return;  // Don't spam alerts
        }
    }
    
    ThreatAlert alert;
    alert.alertId = generateAlertId();
    alert.trackId = track->trackId();
    alert.message = rule.alertMessage;
    alert.message.replace("%TRACK%", track->trackId());
    alert.threatLevel = track->threatLevel();
    alert.timestamp = QDateTime::currentDateTimeUtc();
    
    m_alerts.append(alert);
    
    // Trim alert queue if too large
    while (m_alerts.size() > m_config.alertQueueMaxSize) {
        m_alerts.removeFirst();
    }
    
    Logger::instance().warning("ThreatAssessor", 
                               QString("Alert: %1").arg(alert.message));
    emit newAlert(alert);
}

void ThreatAssessor::updateMetrics() {
    if (!m_trackManager) return;
    
    m_metrics.hostileCount = 0;
    m_metrics.pendingCount = 0;
    m_metrics.highThreatCount = 0;
    double totalThreat = 0.0;
    int trackCount = 0;
    double closestDist = std::numeric_limits<double>::max();
    int highestThreat = 0;
    
    QList<Track*> tracks = m_trackManager->allTracks();
    for (Track* t : tracks) {
        if (t->state() == TrackState::Dropped) continue;
        
        trackCount++;
        totalThreat += t->threatLevel();
        
        if (t->classification() == TrackClassification::Hostile) {
            m_metrics.hostileCount++;
        } else if (t->classification() == TrackClassification::Pending) {
            m_metrics.pendingCount++;
        }
        
        if (t->threatLevel() >= m_config.highThreatThreshold) {
            m_metrics.highThreatCount++;
        }
        
        if (t->threatLevel() > highestThreat) {
            highestThreat = t->threatLevel();
            m_metrics.highestThreatTrackId = t->trackId();
        }
        
        DefendedAsset* asset = nullptr;
        double dist = calculateProximityToAssets(t, &asset);
        if (dist < closestDist) {
            closestDist = dist;
        }
    }
    
    m_metrics.avgThreatLevel = trackCount > 0 ? totalThreat / trackCount : 0.0;
    m_metrics.closestDistanceM = closestDist < std::numeric_limits<double>::max() ? closestDist : -1;
}

QString ThreatAssessor::generateAlertId() {
    return QString("ALERT-%1").arg(m_nextAlertNumber++, 6, 10, QChar('0'));
}

} // namespace CounterUAS
