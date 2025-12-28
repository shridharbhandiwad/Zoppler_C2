#include <QtTest>
#include "core/ThreatAssessor.h"
#include "core/TrackManager.h"

using namespace CounterUAS;

class TestThreatAssessor : public QObject {
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testDefendedAssets();
    void testThreatRules();
    void testThreatAssessment();
    void testAlerts();
    
private:
    TrackManager* m_trackManager;
    ThreatAssessor* m_assessor;
};

void TestThreatAssessor::initTestCase() {
    m_trackManager = new TrackManager(this);
    m_assessor = new ThreatAssessor(m_trackManager, this);
}

void TestThreatAssessor::cleanupTestCase() {
    delete m_assessor;
    delete m_trackManager;
}

void TestThreatAssessor::testDefendedAssets() {
    DefendedAsset asset;
    asset.id = "TEST-01";
    asset.name = "Test Asset";
    asset.position.latitude = 34.0522;
    asset.position.longitude = -118.2437;
    asset.position.altitude = 100.0;
    asset.criticalRadiusM = 500.0;
    asset.warningRadiusM = 1500.0;
    asset.priorityLevel = 5;
    
    m_assessor->addDefendedAsset(asset);
    
    QCOMPARE(m_assessor->defendedAssets().size(), 1);
    QCOMPARE(m_assessor->defendedAssets().first().id, QString("TEST-01"));
    
    m_assessor->removeDefendedAsset("TEST-01");
    QCOMPARE(m_assessor->defendedAssets().size(), 0);
}

void TestThreatAssessor::testThreatRules() {
    m_assessor->clearRules();
    m_assessor->loadDefaultRules();
    
    QVERIFY(m_assessor->rules().size() > 0);
    
    ThreatRule customRule;
    customRule.id = "CUSTOM-01";
    customRule.name = "Custom Rule";
    customRule.maxProximityM = 100.0;
    customRule.setThreatLevel = 5;
    
    m_assessor->addRule(customRule);
    
    // Find the rule
    bool found = false;
    for (const auto& rule : m_assessor->rules()) {
        if (rule.id == "CUSTOM-01") {
            found = true;
            break;
        }
    }
    QVERIFY(found);
}

void TestThreatAssessor::testThreatAssessment() {
    m_assessor->clearDefendedAssets();
    m_assessor->loadDefaultRules();
    
    // Add defended asset
    DefendedAsset asset;
    asset.id = "BASE-01";
    asset.position.latitude = 34.0522;
    asset.position.longitude = -118.2437;
    asset.criticalRadiusM = 500.0;
    asset.warningRadiusM = 1500.0;
    m_assessor->addDefendedAsset(asset);
    
    // Create track near asset
    GeoPosition pos;
    pos.latitude = 34.0525;  // ~300m from asset
    pos.longitude = -118.2437;
    pos.altitude = 100.0;
    
    QString trackId = m_trackManager->createTrack(pos, DetectionSource::Radar);
    
    // Assess track
    m_assessor->assessTrack(trackId);
    
    Track* track = m_trackManager->track(trackId);
    QVERIFY(track != nullptr);
    
    // Track should have elevated threat level due to proximity
    QVERIFY(track->threatLevel() >= 1);
}

void TestThreatAssessor::testAlerts() {
    m_assessor->clearAlerts();
    QCOMPARE(m_assessor->alerts().size(), 0);
    
    // Unacknowledged alerts should be empty
    QCOMPARE(m_assessor->unacknowledgedAlerts().size(), 0);
}

QTEST_MAIN(TestThreatAssessor)
#include "test_threat_assessor.moc"
