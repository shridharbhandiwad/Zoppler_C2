#include <QtTest>
#include "video/VideoStreamManager.h"
#include "video/FileVideoSource.h"
#include "video/VideoOverlayRenderer.h"

using namespace CounterUAS;

class TestVideoPipeline : public QObject {
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testStreamManager();
    void testVideoOverlay();
    void testCameraDefinition();
    
private:
    VideoStreamManager* m_manager;
};

void TestVideoPipeline::initTestCase() {
    m_manager = new VideoStreamManager(this);
}

void TestVideoPipeline::cleanupTestCase() {
    delete m_manager;
}

void TestVideoPipeline::testStreamManager() {
    QCOMPARE(m_manager->activeStreamCount(), 0);
    
    CameraDefinition camera;
    camera.cameraId = "TEST-CAM-01";
    camera.name = "Test Camera";
    camera.streamUrl = "file:///dev/null";
    camera.sourceType = "FILE";
    camera.position.latitude = 34.0522;
    camera.position.longitude = -118.2437;
    
    QString id = m_manager->addStream(camera);
    QCOMPARE(id, QString("TEST-CAM-01"));
    QCOMPARE(m_manager->activeStreamCount(), 1);
    
    QVERIFY(m_manager->stream("TEST-CAM-01") != nullptr);
    
    m_manager->removeStream("TEST-CAM-01");
    QCOMPARE(m_manager->activeStreamCount(), 0);
}

void TestVideoPipeline::testVideoOverlay() {
    VideoOverlayRenderer renderer;
    
    // Test overlay style
    OverlayStyle style;
    style.showTrackIds = true;
    style.showTimestamp = true;
    renderer.setStyle(style);
    
    QVERIFY(renderer.style().showTrackIds);
    
    // Test track overlay
    TrackOverlay track;
    track.trackId = "TRK-0001";
    track.boundingBox = {100, 100, 50, 50, "CAM-01", 0};
    track.classification = TrackClassification::Hostile;
    track.threatLevel = 4;
    
    renderer.addTrackOverlay(track);
    renderer.setSelectedTrack("TRK-0001");
    
    QCOMPARE(renderer.selectedTrack(), QString("TRK-0001"));
    
    // Test rendering (creates a test frame)
    QImage frame(640, 480, QImage::Format_RGB888);
    frame.fill(Qt::black);
    
    QImage result = renderer.renderOverlay(frame);
    QVERIFY(!result.isNull());
    QCOMPARE(result.size(), frame.size());
}

void TestVideoPipeline::testCameraDefinition() {
    CameraDefinition camera;
    camera.cameraId = "CAM-001";
    camera.name = "Test Camera";
    camera.streamUrl = "rtsp://localhost/stream";
    camera.sourceType = "RTSP";
    camera.hasPTZ = true;
    camera.ptzProtocol = "ONVIF";
    
    QCOMPARE(camera.cameraId, QString("CAM-001"));
    QVERIFY(camera.hasPTZ);
}

QTEST_MAIN(TestVideoPipeline)
#include "test_video_pipeline.moc"
