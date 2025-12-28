#ifndef ENGAGEMENTMANAGER_H
#define ENGAGEMENTMANAGER_H

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>
#include <QList>
#include <QImage>
#include "core/Track.h"

namespace CounterUAS {

class TrackManager;
class EffectorInterface;

/**
 * @brief Engagement state enum
 */
enum class EngagementState {
    Idle = 0,
    TrackSelected,
    EffectorRecommended,
    AwaitingAuthorization,
    Authorized,
    Engaging,
    Completed,
    Aborted,
    Failed
};

/**
 * @brief Battle Damage Assessment result
 */
enum class BDAResult {
    Unknown = 0,
    TargetDestroyed,
    TargetDamaged,
    TargetMissed,
    TargetEvaded,
    AssessmentPending
};

/**
 * @brief Engagement record
 */
struct EngagementRecord {
    QString engagementId;
    QString trackId;
    QString effectorId;
    QString effectorType;
    QString operatorId;
    
    QDateTime startTime;
    QDateTime authorizationTime;
    QDateTime executionTime;
    QDateTime completionTime;
    
    EngagementState state = EngagementState::Idle;
    BDAResult bdaResult = BDAResult::Unknown;
    
    GeoPosition targetPosition;
    double targetDistance = 0.0;
    int threatLevel = 0;
    
    QImage videoSnapshot;
    QString notes;
    
    bool wasAborted = false;
    QString abortReason;
    
    QJsonObject toJson() const;
};

/**
 * @brief Engagement authorization request
 */
struct AuthorizationRequest {
    QString engagementId;
    QString trackId;
    QString effectorId;
    QString effectorType;
    
    GeoPosition targetPosition;
    double distance = 0.0;
    int threatLevel = 0;
    TrackClassification classification;
    
    QString recommendationReason;
    QImage videoThumbnail;
    
    QDateTime requestTime;
    int timeoutSeconds = 60;
};

/**
 * @brief Engagement workflow manager
 */
class EngagementManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(EngagementState currentState READ currentState NOTIFY stateChanged)
    Q_PROPERTY(QString currentEngagementId READ currentEngagementId NOTIFY engagementChanged)
    
public:
    explicit EngagementManager(TrackManager* trackManager, QObject* parent = nullptr);
    ~EngagementManager() override;
    
    // Effector registration
    void registerEffector(EffectorInterface* effector);
    void unregisterEffector(const QString& effectorId);
    QList<EffectorInterface*> effectors() const { return m_effectors; }
    EffectorInterface* effector(const QString& effectorId) const;
    EffectorInterface* recommendedEffector(const QString& trackId) const;
    
    // Engagement workflow
    void selectTrack(const QString& trackId);
    void clearSelection();
    QString selectedTrackId() const { return m_selectedTrackId; }
    
    void selectEffector(const QString& effectorId);
    QString selectedEffectorId() const { return m_selectedEffectorId; }
    
    void requestAuthorization();
    void authorize(const QString& operatorId);
    void deny(const QString& reason);
    
    void execute();
    void abort(const QString& reason);
    
    void setBDAResult(BDAResult result);
    void addNote(const QString& note);
    void setVideoSnapshot(const QImage& snapshot);
    
    // State access
    EngagementState currentState() const { return m_currentState; }
    QString currentEngagementId() const { return m_currentEngagementId; }
    EngagementRecord* currentEngagement();
    AuthorizationRequest currentAuthRequest() const { return m_currentAuthRequest; }
    
    // History
    QList<EngagementRecord> engagementHistory() const { return m_history; }
    EngagementRecord* engagement(const QString& engagementId);
    
    // Configuration
    void setAuthorizationTimeout(int seconds) { m_authTimeoutSeconds = seconds; }
    void setAutoRecommendEffector(bool enable) { m_autoRecommend = enable; }
    
    // Statistics
    struct Statistics {
        int totalEngagements = 0;
        int successfulEngagements = 0;
        int abortedEngagements = 0;
        int failedEngagements = 0;
        double avgEngagementTimeMs = 0.0;
    };
    Statistics statistics() const { return m_stats; }
    
signals:
    void stateChanged(EngagementState state);
    void engagementChanged(const QString& engagementId);
    void trackSelected(const QString& trackId);
    void effectorSelected(const QString& effectorId);
    void effectorRecommended(const QString& effectorId, const QString& reason);
    void authorizationRequested(const AuthorizationRequest& request);
    void authorizationGranted(const QString& operatorId);
    void authorizationDenied(const QString& reason);
    void authorizationTimeout();
    void engagementStarted(const QString& engagementId);
    void engagementCompleted(const QString& engagementId, BDAResult result);
    void engagementAborted(const QString& engagementId, const QString& reason);
    void engagementFailed(const QString& engagementId, const QString& reason);
    void switchVideoFeed(const QString& cameraId);
    
public slots:
    void onEffectorStatusChanged(const QString& effectorId);
    void onTrackDropped(const QString& trackId);
    
private slots:
    void onAuthorizationTimeout();
    void checkEngagementCompletion();
    
private:
    void transitionTo(EngagementState newState);
    QString generateEngagementId();
    void recommendEffector();
    void createEngagementRecord();
    void finalizeEngagement(EngagementState finalState);
    double calculateEffectorScore(EffectorInterface* effector, Track* track);
    
    TrackManager* m_trackManager;
    QList<EffectorInterface*> m_effectors;
    
    EngagementState m_currentState = EngagementState::Idle;
    QString m_currentEngagementId;
    QString m_selectedTrackId;
    QString m_selectedEffectorId;
    
    EngagementRecord m_currentRecord;
    AuthorizationRequest m_currentAuthRequest;
    QList<EngagementRecord> m_history;
    
    QTimer* m_authorizationTimer;
    QTimer* m_completionCheckTimer;
    int m_authTimeoutSeconds = 60;
    bool m_autoRecommend = true;
    
    Statistics m_stats;
    int m_nextEngagementNumber = 1;
};

} // namespace CounterUAS

Q_DECLARE_METATYPE(CounterUAS::EngagementState)
Q_DECLARE_METATYPE(CounterUAS::BDAResult)

#endif // ENGAGEMENTMANAGER_H
