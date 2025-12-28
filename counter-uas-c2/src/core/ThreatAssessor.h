#ifndef THREATASSESSOR_H
#define THREATASSESSOR_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QJsonObject>
#include "core/Track.h"

namespace CounterUAS {

class TrackManager;

/**
 * @brief Defended asset definition
 */
struct DefendedAsset {
    QString id;
    QString name;
    GeoPosition position;
    double criticalRadiusM = 500.0;     // High alert radius
    double warningRadiusM = 1500.0;     // Warning radius
    int priorityLevel = 1;              // 1-5, higher is more critical
    
    QJsonObject toJson() const;
    static DefendedAsset fromJson(const QJsonObject& json);
};

/**
 * @brief Threat assessment rule
 */
struct ThreatRule {
    QString id;
    QString name;
    QString description;
    bool enabled = true;
    
    // Conditions
    double minProximityM = -1;          // -1 means any
    double maxProximityM = -1;
    double minVelocityMps = -1;
    double maxVelocityMps = -1;
    double minHeadingToAssetDeg = -1;   // Heading toward defended asset
    double maxHeadingToAssetDeg = -1;
    bool requiresVisualConfirmation = false;
    bool requiresRFDetection = false;
    
    // Actions
    int threatLevelIncrease = 0;
    int setThreatLevel = -1;            // -1 means don't set, use increase
    TrackClassification forceClassification = TrackClassification::Unknown;
    bool generateAlert = false;
    QString alertMessage;
    
    QJsonObject toJson() const;
    static ThreatRule fromJson(const QJsonObject& json);
};

/**
 * @brief Threat alert structure
 */
struct ThreatAlert {
    QString alertId;
    QString trackId;
    QString message;
    int threatLevel;
    QDateTime timestamp;
    bool acknowledged = false;
    QString acknowledgedBy;
    QDateTime acknowledgedTime;
    
    QJsonObject toJson() const;
};

/**
 * @brief Threat assessment configuration
 */
struct ThreatAssessorConfig {
    int assessmentIntervalMs = 500;
    int alertQueueMaxSize = 100;
    bool autoSlewToHighestThreat = true;
    int highThreatThreshold = 4;
    double headingToleranceDeg = 30.0;
};

/**
 * @brief Threat Assessment Module for evaluating and prioritizing threats
 */
class ThreatAssessor : public QObject {
    Q_OBJECT
    
public:
    explicit ThreatAssessor(TrackManager* trackManager, QObject* parent = nullptr);
    ~ThreatAssessor() override;
    
    // Configuration
    void setConfig(const ThreatAssessorConfig& config);
    ThreatAssessorConfig config() const { return m_config; }
    
    // Lifecycle
    void start();
    void stop();
    bool isRunning() const { return m_running; }
    
    // Defended assets
    void addDefendedAsset(const DefendedAsset& asset);
    void removeDefendedAsset(const QString& assetId);
    void clearDefendedAssets();
    QList<DefendedAsset> defendedAssets() const { return m_assets; }
    DefendedAsset* nearestAsset(const GeoPosition& pos);
    
    // Threat rules
    void addRule(const ThreatRule& rule);
    void removeRule(const QString& ruleId);
    void setRuleEnabled(const QString& ruleId, bool enabled);
    void clearRules();
    QList<ThreatRule> rules() const { return m_rules; }
    void loadDefaultRules();
    
    // Manual assessment
    void assessTrack(const QString& trackId);
    void assessAllTracks();
    
    // Threat queue
    QList<Track*> threatQueue() const;  // Sorted by threat level
    Track* highestUnconfirmedThreat() const;
    
    // Alert management
    QList<ThreatAlert> alerts() const { return m_alerts; }
    QList<ThreatAlert> unacknowledgedAlerts() const;
    void acknowledgeAlert(const QString& alertId, const QString& operatorId);
    void clearAlerts();
    
    // Real-time metrics
    struct ThreatMetrics {
        int hostileCount = 0;
        int pendingCount = 0;
        int highThreatCount = 0;
        double avgThreatLevel = 0.0;
        QString highestThreatTrackId;
        double closestDistanceM = -1;
        qint64 lastAssessmentMs = 0;
    };
    ThreatMetrics metrics() const { return m_metrics; }
    
signals:
    void threatLevelChanged(const QString& trackId, int oldLevel, int newLevel);
    void newAlert(const ThreatAlert& alert);
    void alertAcknowledged(const QString& alertId);
    void highThreatDetected(const QString& trackId);
    void metricsUpdated(const ThreatMetrics& metrics);
    void assessmentComplete();
    void slewCameraRequest(const QString& cameraId, const GeoPosition& pos);
    
public slots:
    void onTrackUpdated(const QString& trackId);
    void onTrackCreated(const QString& trackId);
    
private slots:
    void performAssessmentCycle();
    
private:
    int calculateThreatLevel(Track* track);
    void applyRules(Track* track, int& threatLevel, TrackClassification& classification);
    double calculateProximityToAssets(Track* track, DefendedAsset** nearestAsset);
    bool isHeadingTowardAsset(Track* track, const DefendedAsset& asset);
    void generateAlert(Track* track, const ThreatRule& rule);
    void updateMetrics();
    QString generateAlertId();
    
    TrackManager* m_trackManager;
    ThreatAssessorConfig m_config;
    QTimer* m_assessmentTimer;
    bool m_running = false;
    
    QList<DefendedAsset> m_assets;
    QList<ThreatRule> m_rules;
    QList<ThreatAlert> m_alerts;
    
    ThreatMetrics m_metrics;
    int m_nextAlertNumber = 1;
};

} // namespace CounterUAS

#endif // THREATASSESSOR_H
