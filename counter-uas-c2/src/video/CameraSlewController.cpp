#include "video/CameraSlewController.h"
#include "core/TrackManager.h"
#include "video/VideoStreamManager.h"
#include "utils/Logger.h"

namespace CounterUAS {

CameraSlewController::CameraSlewController(QObject* parent)
    : QObject(parent)
    , m_updateTimer(new QTimer(this))
{
    m_updateTimer->setInterval(100);
    connect(m_updateTimer, &QTimer::timeout, this, &CameraSlewController::updateTracking);
}

void CameraSlewController::setTrackManager(TrackManager* manager) {
    if (m_trackManager) {
        disconnect(m_trackManager, nullptr, this, nullptr);
    }
    
    m_trackManager = manager;
    
    if (m_trackManager) {
        connect(m_trackManager, &TrackManager::trackUpdated,
                this, &CameraSlewController::onTrackUpdated);
        connect(m_trackManager, &TrackManager::trackDropped,
                this, &CameraSlewController::onTrackDropped);
    }
}

void CameraSlewController::setVideoStreamManager(VideoStreamManager* manager) {
    m_videoManager = manager;
}

void CameraSlewController::slewToTrack(const QString& cameraId, const QString& trackId) {
    if (!m_trackManager) return;
    
    Track* track = m_trackManager->track(trackId);
    if (!track) {
        Logger::instance().warning("CameraSlewController",
                                  "Track not found: " + trackId);
        return;
    }
    
    slewToPosition(cameraId, track->position());
}

void CameraSlewController::startAutoTracking(const QString& cameraId, const QString& trackId) {
    if (!m_trackManager) return;
    
    Track* track = m_trackManager->track(trackId);
    if (!track) return;
    
    m_cameraTrackMap[cameraId] = trackId;
    
    // Initial slew
    slewToTrack(cameraId, trackId);
    
    // Start update timer if not running
    if (!m_updateTimer->isActive()) {
        m_updateTimer->start();
    }
    
    Logger::instance().info("CameraSlewController",
                           QString("Started auto-tracking: camera %1 -> track %2")
                               .arg(cameraId)
                               .arg(trackId));
    
    emit trackingStarted(cameraId, trackId);
}

void CameraSlewController::stopAutoTracking(const QString& cameraId) {
    if (!m_cameraTrackMap.contains(cameraId)) return;
    
    m_cameraTrackMap.remove(cameraId);
    
    // Stop timer if no more tracking
    if (m_cameraTrackMap.isEmpty()) {
        m_updateTimer->stop();
    }
    
    Logger::instance().info("CameraSlewController",
                           "Stopped auto-tracking: camera " + cameraId);
    
    emit trackingStopped(cameraId);
}

void CameraSlewController::slewToPosition(const QString& cameraId, const GeoPosition& target) {
    if (m_videoManager) {
        m_videoManager->slewCamera(cameraId, target);
    }
    
    emit slewStarted(cameraId, target);
}

void CameraSlewController::slewNearestCamera(const GeoPosition& target) {
    if (m_videoManager) {
        m_videoManager->slewNearestCamera(target);
    }
}

bool CameraSlewController::isTracking(const QString& cameraId) const {
    return m_cameraTrackMap.contains(cameraId);
}

QString CameraSlewController::trackedTrack(const QString& cameraId) const {
    return m_cameraTrackMap.value(cameraId);
}

void CameraSlewController::onTrackUpdated(const QString& trackId) {
    // Update slew for any cameras tracking this track
    for (auto it = m_cameraTrackMap.begin(); it != m_cameraTrackMap.end(); ++it) {
        if (it.value() == trackId) {
            Track* track = m_trackManager->track(trackId);
            if (track) {
                slewToPosition(it.key(), track->position());
            }
        }
    }
}

void CameraSlewController::onTrackDropped(const QString& trackId) {
    // Stop tracking for any cameras following this track
    QStringList camerasToStop;
    
    for (auto it = m_cameraTrackMap.begin(); it != m_cameraTrackMap.end(); ++it) {
        if (it.value() == trackId) {
            camerasToStop.append(it.key());
            emit trackLost(it.key(), trackId);
        }
    }
    
    for (const QString& cameraId : camerasToStop) {
        stopAutoTracking(cameraId);
    }
}

void CameraSlewController::updateTracking() {
    if (m_cameraTrackMap.isEmpty() || !m_trackManager) {
        m_updateTimer->stop();
        return;
    }
    
    // Periodic update for all tracked cameras
    for (auto it = m_cameraTrackMap.begin(); it != m_cameraTrackMap.end(); ++it) {
        Track* track = m_trackManager->track(it.value());
        if (track && track->state() != TrackState::Dropped) {
            // Use predicted position for smoother tracking
            GeoPosition predicted = track->predictedPosition(100);  // 100ms ahead
            slewToPosition(it.key(), predicted);
        }
    }
}

} // namespace CounterUAS
