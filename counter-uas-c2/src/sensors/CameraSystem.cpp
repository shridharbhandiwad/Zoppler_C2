#include "sensors/CameraSystem.h"
#include "utils/Logger.h"
#include <QtMath>

namespace CounterUAS {

CameraSystem::CameraSystem(const QString& sensorId, QObject* parent)
    : SensorInterface(sensorId, parent)
    , m_slewTimer(new QTimer(this))
{
    m_slewTimer->setInterval(50);  // Update slew every 50ms
    QObject::connect(m_slewTimer, &QTimer::timeout, this, &CameraSystem::updateSlewProgress);
}

CameraSystem::~CameraSystem() {
    disconnect();
}

void CameraSystem::setConfig(const CameraSystemConfig& config) {
    m_config = config;
    m_ptzState = config.initialPTZ;
    m_position = config.mountPosition;
    m_name = config.cameraName;
}

bool CameraSystem::connect() {
    if (m_connected) return true;
    
    setStatus(SensorStatus::Initializing);
    
    // In a real implementation, this would connect to the camera stream
    // For now, we mark as connected for simulation purposes
    
    m_connected = true;
    setStatus(SensorStatus::Online);
    emit connectedChanged(true);
    
    Logger::instance().info("CameraSystem",
                           QString("%1 connected to %2")
                               .arg(m_sensorId)
                               .arg(m_config.streamUrl));
    
    return true;
}

void CameraSystem::disconnect() {
    if (!m_connected) return;
    
    m_slewTimer->stop();
    m_connected = false;
    setStatus(SensorStatus::Offline);
    emit connectedChanged(false);
    
    Logger::instance().info("CameraSystem", m_sensorId + " disconnected");
}

bool CameraSystem::isConnected() const {
    return m_connected;
}

void CameraSystem::setPan(double degrees) {
    if (!m_config.hasPTZ) return;
    
    degrees = qBound(m_config.panMin, degrees, m_config.panMax);
    m_targetPTZ.pan = degrees;
    executePTZCommand();
}

void CameraSystem::setTilt(double degrees) {
    if (!m_config.hasPTZ) return;
    
    degrees = qBound(m_config.tiltMin, degrees, m_config.tiltMax);
    m_targetPTZ.tilt = degrees;
    executePTZCommand();
}

void CameraSystem::setZoom(double level) {
    if (!m_config.hasPTZ) return;
    
    level = qBound(m_config.zoomMin, level, m_config.zoomMax);
    m_targetPTZ.zoom = level;
    
    // Update FOV based on zoom
    double baseFOV = 60.0;  // Assume 60 degree base FOV
    m_targetPTZ.hfov = baseFOV / level;
    m_targetPTZ.vfov = (baseFOV * 0.75) / level;  // 4:3 aspect
    
    executePTZCommand();
}

void CameraSystem::setPTZ(double pan, double tilt, double zoom) {
    if (!m_config.hasPTZ) return;
    
    m_targetPTZ.pan = qBound(m_config.panMin, pan, m_config.panMax);
    m_targetPTZ.tilt = qBound(m_config.tiltMin, tilt, m_config.tiltMax);
    m_targetPTZ.zoom = qBound(m_config.zoomMin, zoom, m_config.zoomMax);
    
    double baseFOV = 60.0;
    m_targetPTZ.hfov = baseFOV / m_targetPTZ.zoom;
    m_targetPTZ.vfov = (baseFOV * 0.75) / m_targetPTZ.zoom;
    
    executePTZCommand();
}

void CameraSystem::goToPreset(int presetNumber) {
    if (m_presets.contains(presetNumber)) {
        m_targetPTZ = m_presets[presetNumber];
        executePTZCommand();
        
        Logger::instance().info("CameraSystem",
                               QString("%1 going to preset %2")
                                   .arg(m_sensorId)
                                   .arg(presetNumber));
    }
}

void CameraSystem::savePreset(int presetNumber) {
    m_presets[presetNumber] = m_ptzState;
    
    Logger::instance().info("CameraSystem",
                           QString("%1 saved preset %2: pan=%3, tilt=%4, zoom=%5")
                               .arg(m_sensorId)
                               .arg(presetNumber)
                               .arg(m_ptzState.pan)
                               .arg(m_ptzState.tilt)
                               .arg(m_ptzState.zoom));
}

void CameraSystem::slewToPosition(const GeoPosition& target) {
    if (!m_config.hasPTZ) {
        emit slewComplete(target);
        return;
    }
    
    auto [pan, tilt] = calculatePanTilt(target);
    
    m_slewTarget = target;
    m_slewInProgress = true;
    m_slewComplete = false;
    
    m_targetPTZ.pan = pan;
    m_targetPTZ.tilt = tilt;
    
    // Auto-zoom based on distance
    double distance = 0.0;  // Calculate distance to target
    // Simplified: set zoom based on approximate distance
    Track tempTrack("temp");
    tempTrack.setPosition(m_position);
    distance = tempTrack.distanceTo(target);
    
    if (distance > 1500) {
        m_targetPTZ.zoom = m_config.zoomMax;
    } else if (distance > 500) {
        m_targetPTZ.zoom = m_config.zoomMax / 2;
    } else {
        m_targetPTZ.zoom = m_config.zoomMin;
    }
    
    executePTZCommand();
    m_slewTimer->start();
    
    Logger::instance().info("CameraSystem",
                           QString("%1 slewing to position: lat=%2, lon=%3, alt=%4")
                               .arg(m_sensorId)
                               .arg(target.latitude, 0, 'f', 6)
                               .arg(target.longitude, 0, 'f', 6)
                               .arg(target.altitude, 0, 'f', 1));
}

void CameraSystem::slewToTrack(const QString& trackId) {
    Q_UNUSED(trackId)
    // This would be connected to the track manager
    // For now, it's a placeholder
}

void CameraSystem::reportDetection(const CameraDetection& detection) {
    if (detection.confidence < m_config.detectionConfidenceThreshold) {
        return;
    }
    
    // Convert bounding box to estimated position
    GeoPosition estimatedPos = calculateTargetPosition(detection.boundingBox);
    
    // Create sensor detection
    SensorDetection sensorDet;
    sensorDet.sensorId = m_sensorId;
    sensorDet.position = estimatedPos;
    sensorDet.confidence = detection.confidence;
    sensorDet.timestamp = detection.timestamp;
    sensorDet.sourceType = DetectionSource::Camera;
    sensorDet.metadata["objectClass"] = detection.objectClass;
    sensorDet.metadata["frameNumber"] = static_cast<qint64>(detection.frameNumber);
    sensorDet.metadata["bboxX"] = detection.boundingBox.x();
    sensorDet.metadata["bboxY"] = detection.boundingBox.y();
    sensorDet.metadata["bboxW"] = detection.boundingBox.width();
    sensorDet.metadata["bboxH"] = detection.boundingBox.height();
    
    recordDetection();
    
    emit cameraDetection(detection);
    emit this->detection(sensorDet);
}

void CameraSystem::setRecording(bool recording) {
    if (m_recording != recording) {
        m_recording = recording;
        emit recordingChanged(recording);
        
        Logger::instance().info("CameraSystem",
                               QString("%1 recording %2")
                                   .arg(m_sensorId)
                                   .arg(recording ? "started" : "stopped"));
    }
}

void CameraSystem::processData() {
    // In a real implementation, this would process incoming video frames
    // For simulation, we just update frame number
    m_frameNumber++;
}

void CameraSystem::onFrameReceived(const QImage& frame) {
    m_currentFrame = frame;
    m_frameNumber++;
    
    emit frameReady(frame, m_frameNumber);
    
    // Run detection if enabled
    if (m_config.enableDetection && (m_frameNumber % m_config.detectionFrameSkip == 0)) {
        // Detection would be run here
        // For now, this is handled externally
    }
}

void CameraSystem::updateSlewProgress() {
    if (!m_slewInProgress) {
        m_slewTimer->stop();
        return;
    }
    
    // Simulate slew progress
    const double slewSpeed = 30.0;  // degrees per second
    const double dt = 0.05;  // 50ms timer
    
    double panDiff = m_targetPTZ.pan - m_ptzState.pan;
    double tiltDiff = m_targetPTZ.tilt - m_ptzState.tilt;
    double zoomDiff = m_targetPTZ.zoom - m_ptzState.zoom;
    
    // Normalize pan difference for wraparound
    if (panDiff > 180.0) panDiff -= 360.0;
    if (panDiff < -180.0) panDiff += 360.0;
    
    double maxMove = slewSpeed * dt;
    
    if (std::abs(panDiff) < maxMove && 
        std::abs(tiltDiff) < maxMove &&
        std::abs(zoomDiff) < 0.5) {
        // Slew complete
        m_ptzState = m_targetPTZ;
        m_slewInProgress = false;
        m_slewComplete = true;
        m_slewTimer->stop();
        
        emit ptzChanged(m_ptzState);
        emit slewComplete(m_slewTarget);
        
        Logger::instance().info("CameraSystem",
                               QString("%1 slew complete").arg(m_sensorId));
    } else {
        // Continue slewing
        if (std::abs(panDiff) > maxMove) {
            m_ptzState.pan += (panDiff > 0 ? maxMove : -maxMove);
        } else {
            m_ptzState.pan = m_targetPTZ.pan;
        }
        
        if (std::abs(tiltDiff) > maxMove) {
            m_ptzState.tilt += (tiltDiff > 0 ? maxMove : -maxMove);
        } else {
            m_ptzState.tilt = m_targetPTZ.tilt;
        }
        
        double zoomSpeed = 2.0 * dt;
        if (std::abs(zoomDiff) > zoomSpeed) {
            m_ptzState.zoom += (zoomDiff > 0 ? zoomSpeed : -zoomSpeed);
        } else {
            m_ptzState.zoom = m_targetPTZ.zoom;
        }
        
        emit ptzChanged(m_ptzState);
    }
}

GeoPosition CameraSystem::calculateTargetPosition(const QRectF& bbox) {
    // Estimate target position based on bounding box in image
    // This is a simplified model assuming flat terrain
    
    double bboxCenterX = bbox.x() + bbox.width() / 2.0;
    double bboxCenterY = bbox.y() + bbox.height() / 2.0;
    
    // Convert image coordinates to angles
    double offsetX = (bboxCenterX - 0.5) * m_ptzState.hfov;
    double offsetY = (0.5 - bboxCenterY) * m_ptzState.vfov;
    
    double targetPan = m_ptzState.pan + offsetX;
    double targetTilt = m_ptzState.tilt + offsetY;
    
    // Estimate range based on bounding box size
    // Assume typical drone size of 0.5m
    double assumedTargetSize = 0.5;  // meters
    double pixelSize = bbox.width();  // normalized
    double angularSize = pixelSize * m_ptzState.hfov;
    double estimatedRange = assumedTargetSize / (2.0 * std::tan(qDegreesToRadians(angularSize / 2.0)));
    estimatedRange = qBound(50.0, estimatedRange, maxRange());
    
    // Convert to position
    double azRad = qDegreesToRadians(targetPan);
    double elRad = qDegreesToRadians(targetTilt);
    
    double horizontalRange = estimatedRange * std::cos(elRad);
    double northOffset = horizontalRange * std::cos(azRad);
    double eastOffset = horizontalRange * std::sin(azRad);
    double altOffset = estimatedRange * std::sin(elRad);
    
    GeoPosition result;
    result.latitude = m_position.latitude + (northOffset / 111000.0);
    result.longitude = m_position.longitude + 
                       (eastOffset / (111000.0 * std::cos(qDegreesToRadians(m_position.latitude))));
    result.altitude = m_position.altitude + altOffset;
    
    return result;
}

QPair<double, double> CameraSystem::calculatePanTilt(const GeoPosition& target) {
    // Calculate pan and tilt to point at target
    
    double dLat = target.latitude - m_position.latitude;
    double dLon = target.longitude - m_position.longitude;
    double dAlt = target.altitude - m_position.altitude;
    
    double northOffset = dLat * 111000.0;
    double eastOffset = dLon * 111000.0 * std::cos(qDegreesToRadians(m_position.latitude));
    
    double horizontalDist = std::sqrt(northOffset * northOffset + eastOffset * eastOffset);
    
    double pan = qRadiansToDegrees(std::atan2(eastOffset, northOffset));
    double tilt = qRadiansToDegrees(std::atan2(dAlt, horizontalDist));
    
    // Normalize pan to camera range
    while (pan < m_config.panMin) pan += 360.0;
    while (pan > m_config.panMax) pan -= 360.0;
    
    tilt = qBound(m_config.tiltMin, tilt, m_config.tiltMax);
    
    return {pan, tilt};
}

void CameraSystem::executePTZCommand() {
    // In a real implementation, this would send PTZ commands to the camera
    // For simulation, we start the slew timer
    m_slewInProgress = true;
    m_slewComplete = false;
    m_slewTimer->start();
}

} // namespace CounterUAS
