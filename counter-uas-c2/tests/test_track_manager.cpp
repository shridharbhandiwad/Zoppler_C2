#include <QtTest>
#include "core/TrackManager.h"
#include "core/Track.h"

using namespace CounterUAS;

class TestTrackManager : public QObject {
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testTrackCreation();
    void testTrackUpdate();
    void testTrackCorrelation();
    void testTrackLifecycle();
    void testThreatLevel();
    
private:
    TrackManager* m_manager;
};

void TestTrackManager::initTestCase() {
    m_manager = new TrackManager(this);
    
    TrackManagerConfig config;
    config.updateRateHz = 10;
    config.correlationDistanceM = 100.0;
    config.coastingTimeoutMs = 1000;
    config.dropTimeoutMs = 3000;
    m_manager->setConfig(config);
}

void TestTrackManager::cleanupTestCase() {
    delete m_manager;
}

void TestTrackManager::testTrackCreation() {
    GeoPosition pos;
    pos.latitude = 34.0522;
    pos.longitude = -118.2437;
    pos.altitude = 100.0;
    
    QString trackId = m_manager->createTrack(pos, DetectionSource::Radar);
    
    QVERIFY(!trackId.isEmpty());
    QCOMPARE(m_manager->trackCount(), 1);
    
    Track* track = m_manager->track(trackId);
    QVERIFY(track != nullptr);
    QCOMPARE(track->classification(), TrackClassification::Pending);
    QCOMPARE(track->state(), TrackState::Initiated);
}

void TestTrackManager::testTrackUpdate() {
    GeoPosition pos;
    pos.latitude = 34.0522;
    pos.longitude = -118.2437;
    pos.altitude = 100.0;
    
    QString trackId = m_manager->createTrack(pos, DetectionSource::Radar);
    
    GeoPosition newPos = pos;
    newPos.latitude += 0.001;
    
    m_manager->updateTrack(trackId, newPos);
    
    Track* track = m_manager->track(trackId);
    QVERIFY(track != nullptr);
    
    // Position should be updated (may differ slightly due to Kalman filter)
    QVERIFY(qAbs(track->position().latitude - newPos.latitude) < 0.01);
}

void TestTrackManager::testTrackCorrelation() {
    m_manager->clearAllTracks();
    
    GeoPosition pos1;
    pos1.latitude = 34.0522;
    pos1.longitude = -118.2437;
    pos1.altitude = 100.0;
    
    VelocityVector vel;
    vel.north = 10.0;
    vel.east = 5.0;
    vel.down = 0.0;
    
    // First detection creates track
    m_manager->processRadarDetection(pos1, vel, 0.9, QDateTime::currentMSecsSinceEpoch());
    QCOMPARE(m_manager->trackCount(), 1);
    
    // Second detection nearby should correlate
    GeoPosition pos2 = pos1;
    pos2.latitude += 0.0005;  // ~50m
    
    m_manager->processRadarDetection(pos2, vel, 0.9, QDateTime::currentMSecsSinceEpoch());
    QCOMPARE(m_manager->trackCount(), 1);  // Should still be 1 track
}

void TestTrackManager::testTrackLifecycle() {
    m_manager->clearAllTracks();
    
    GeoPosition pos;
    pos.latitude = 34.0522;
    pos.longitude = -118.2437;
    pos.altitude = 100.0;
    
    QString trackId = m_manager->createTrack(pos, DetectionSource::Radar);
    Track* track = m_manager->track(trackId);
    
    QCOMPARE(track->state(), TrackState::Initiated);
    
    // Update should promote to Active
    m_manager->updateTrack(trackId, pos);
    QCOMPARE(track->state(), TrackState::Active);
    
    // Manual drop
    m_manager->dropTrack(trackId);
    QCOMPARE(track->state(), TrackState::Dropped);
}

void TestTrackManager::testThreatLevel() {
    m_manager->clearAllTracks();
    
    GeoPosition pos;
    pos.latitude = 34.0522;
    pos.longitude = -118.2437;
    pos.altitude = 100.0;
    
    QString trackId = m_manager->createTrack(pos, DetectionSource::Radar);
    
    m_manager->setTrackThreatLevel(trackId, 4);
    
    Track* track = m_manager->track(trackId);
    QCOMPARE(track->threatLevel(), 4);
    
    // Test bounds
    m_manager->setTrackThreatLevel(trackId, 10);
    QCOMPARE(track->threatLevel(), 5);  // Should be capped at 5
    
    m_manager->setTrackThreatLevel(trackId, -5);
    QCOMPARE(track->threatLevel(), 1);  // Should be minimum 1
}

QTEST_MAIN(TestTrackManager)
#include "test_track_manager.moc"
