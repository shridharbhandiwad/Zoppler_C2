#include "core/Track.h"
#include <QtMath>
#include <QJsonArray>
#include <QMutexLocker>

namespace CounterUAS {

// GeoPosition implementation
QJsonObject GeoPosition::toJson() const {
    QJsonObject obj;
    obj["latitude"] = latitude;
    obj["longitude"] = longitude;
    obj["altitude"] = altitude;
    return obj;
}

GeoPosition GeoPosition::fromJson(const QJsonObject& json) {
    GeoPosition pos;
    pos.latitude = json["latitude"].toDouble();
    pos.longitude = json["longitude"].toDouble();
    pos.altitude = json["altitude"].toDouble();
    return pos;
}

// VelocityVector implementation
double VelocityVector::speed() const {
    return std::sqrt(north * north + east * east + down * down);
}

double VelocityVector::heading() const {
    double hdg = qRadiansToDegrees(std::atan2(east, north));
    if (hdg < 0) hdg += 360.0;
    return hdg;
}

double VelocityVector::climbRate() const {
    return -down;  // Positive is climbing
}

QJsonObject VelocityVector::toJson() const {
    QJsonObject obj;
    obj["north"] = north;
    obj["east"] = east;
    obj["down"] = down;
    return obj;
}

VelocityVector VelocityVector::fromJson(const QJsonObject& json) {
    VelocityVector vel;
    vel.north = json["north"].toDouble();
    vel.east = json["east"].toDouble();
    vel.down = json["down"].toDouble();
    return vel;
}

// Track implementation
Track::Track(const QString& id, QObject* parent)
    : QObject(parent)
    , m_trackId(id)
    , m_createdTime(QDateTime::currentDateTimeUtc())
    , m_lastUpdateTime(m_createdTime)
{
}

Track::Track(const Track& other)
    : QObject(other.parent())
    , m_trackId(other.m_trackId)
    , m_position(other.m_position)
    , m_velocity(other.m_velocity)
    , m_classification(other.m_classification)
    , m_state(other.m_state)
    , m_threatLevel(other.m_threatLevel)
    , m_detectionSources(other.m_detectionSources)
    , m_createdTime(other.m_createdTime)
    , m_lastUpdateTime(other.m_lastUpdateTime)
    , m_associatedCameraId(other.m_associatedCameraId)
    , m_visuallyTracked(other.m_visuallyTracked)
    , m_boundingBox(other.m_boundingBox)
    , m_classificationConfidence(other.m_classificationConfidence)
    , m_engaged(other.m_engaged)
    , m_trackQuality(other.m_trackQuality)
    , m_coastCount(other.m_coastCount)
    , m_positionHistory(other.m_positionHistory)
{
}

GeoPosition Track::position() const {
    QMutexLocker locker(&m_mutex);
    return m_position;
}

void Track::setPosition(const GeoPosition& pos) {
    {
        QMutexLocker locker(&m_mutex);
        m_position = pos;
        m_lastUpdateTime = QDateTime::currentDateTimeUtc();
    }
    emit positionChanged();
    emit updated();
}

GeoPosition Track::predictedPosition(qint64 deltaMs) const {
    QMutexLocker locker(&m_mutex);
    
    double dt = deltaMs / 1000.0;  // Convert to seconds
    
    // Convert velocity to position change
    // Approximate conversion: 1 degree latitude = 111km
    // 1 degree longitude = 111km * cos(latitude)
    const double metersPerDegreeLat = 111000.0;
    double metersPerDegreeLon = 111000.0 * std::cos(qDegreesToRadians(m_position.latitude));
    
    GeoPosition predicted = m_position;
    predicted.latitude += (m_velocity.north * dt) / metersPerDegreeLat;
    predicted.longitude += (m_velocity.east * dt) / metersPerDegreeLon;
    predicted.altitude -= m_velocity.down * dt;
    
    return predicted;
}

VelocityVector Track::velocity() const {
    QMutexLocker locker(&m_mutex);
    return m_velocity;
}

void Track::setVelocity(const VelocityVector& vel) {
    {
        QMutexLocker locker(&m_mutex);
        m_velocity = vel;
        m_lastUpdateTime = QDateTime::currentDateTimeUtc();
    }
    emit velocityChanged();
    emit updated();
}

void Track::setClassification(TrackClassification cls) {
    if (m_classification != cls) {
        m_classification = cls;
        m_lastUpdateTime = QDateTime::currentDateTimeUtc();
        emit classificationChanged();
        emit updated();
    }
}

QString Track::classificationString() const {
    switch (m_classification) {
        case TrackClassification::Unknown: return "UNKNOWN";
        case TrackClassification::Friendly: return "FRIENDLY";
        case TrackClassification::Hostile: return "HOSTILE";
        case TrackClassification::Pending: return "PENDING";
        case TrackClassification::Neutral: return "NEUTRAL";
        default: return "UNKNOWN";
    }
}

void Track::setThreatLevel(int level) {
    level = qBound(1, level, 5);
    if (m_threatLevel != level) {
        m_threatLevel = level;
        m_lastUpdateTime = QDateTime::currentDateTimeUtc();
        emit threatLevelChanged(level);
        emit updated();
    }
}

void Track::setState(TrackState state) {
    if (m_state != state) {
        m_state = state;
        m_lastUpdateTime = QDateTime::currentDateTimeUtc();
        emit stateChanged(state);
        emit updated();
    }
}

QString Track::stateString() const {
    switch (m_state) {
        case TrackState::Initiated: return "INITIATED";
        case TrackState::Active: return "ACTIVE";
        case TrackState::Coasting: return "COASTING";
        case TrackState::Dropped: return "DROPPED";
        default: return "UNKNOWN";
    }
}

void Track::addDetectionSource(DetectionSource source) {
    if (!m_detectionSources.contains(source)) {
        m_detectionSources.append(source);
    }
}

void Track::clearDetectionSources() {
    m_detectionSources.clear();
}

bool Track::hasSource(DetectionSource source) const {
    return m_detectionSources.contains(source);
}

qint64 Track::trackAge() const {
    return m_createdTime.msecsTo(QDateTime::currentDateTimeUtc());
}

qint64 Track::timeSinceUpdate() const {
    return m_lastUpdateTime.msecsTo(QDateTime::currentDateTimeUtc());
}

void Track::setAssociatedCameraId(const QString& cameraId) {
    m_associatedCameraId = cameraId;
    m_lastUpdateTime = QDateTime::currentDateTimeUtc();
    emit updated();
}

void Track::setVisuallyTracked(bool tracked) {
    m_visuallyTracked = tracked;
    m_lastUpdateTime = QDateTime::currentDateTimeUtc();
    emit updated();
}

void Track::setBoundingBox(const BoundingBox& box) {
    m_boundingBox = box;
    emit boundingBoxChanged();
}

void Track::setClassificationConfidence(double conf) {
    m_classificationConfidence = qBound(0.0, conf, 1.0);
    m_lastUpdateTime = QDateTime::currentDateTimeUtc();
    emit updated();
}

void Track::setEngaged(bool engaged) {
    m_engaged = engaged;
    m_lastUpdateTime = QDateTime::currentDateTimeUtc();
    emit updated();
}

void Track::setTrackQuality(double quality) {
    m_trackQuality = qBound(0.0, quality, 1.0);
    m_lastUpdateTime = QDateTime::currentDateTimeUtc();
}

void Track::incrementCoastCount() {
    m_coastCount++;
}

void Track::resetCoastCount() {
    m_coastCount = 0;
}

void Track::addPositionHistory(const GeoPosition& pos, qint64 timestamp) {
    QMutexLocker locker(&m_mutex);
    m_positionHistory.append(qMakePair(pos, timestamp));
    while (m_positionHistory.size() > MAX_HISTORY_SIZE) {
        m_positionHistory.removeFirst();
    }
}

QList<QPair<GeoPosition, qint64>> Track::positionHistory() const {
    QMutexLocker locker(&m_mutex);
    return m_positionHistory;
}

void Track::clearHistory() {
    QMutexLocker locker(&m_mutex);
    m_positionHistory.clear();
}

QJsonObject Track::toJson() const {
    QMutexLocker locker(&m_mutex);
    
    QJsonObject obj;
    obj["trackId"] = m_trackId;
    obj["position"] = m_position.toJson();
    obj["velocity"] = m_velocity.toJson();
    obj["classification"] = static_cast<int>(m_classification);
    obj["state"] = static_cast<int>(m_state);
    obj["threatLevel"] = m_threatLevel;
    obj["createdTime"] = m_createdTime.toString(Qt::ISODateWithMs);
    obj["lastUpdateTime"] = m_lastUpdateTime.toString(Qt::ISODateWithMs);
    obj["associatedCameraId"] = m_associatedCameraId;
    obj["visuallyTracked"] = m_visuallyTracked;
    obj["classificationConfidence"] = m_classificationConfidence;
    obj["engaged"] = m_engaged;
    obj["trackQuality"] = m_trackQuality;
    
    QJsonArray sources;
    for (auto source : m_detectionSources) {
        sources.append(static_cast<int>(source));
    }
    obj["detectionSources"] = sources;
    
    return obj;
}

Track* Track::fromJson(const QJsonObject& json, QObject* parent) {
    Track* track = new Track(json["trackId"].toString(), parent);
    track->m_position = GeoPosition::fromJson(json["position"].toObject());
    track->m_velocity = VelocityVector::fromJson(json["velocity"].toObject());
    track->m_classification = static_cast<TrackClassification>(json["classification"].toInt());
    track->m_state = static_cast<TrackState>(json["state"].toInt());
    track->m_threatLevel = json["threatLevel"].toInt();
    track->m_createdTime = QDateTime::fromString(json["createdTime"].toString(), Qt::ISODateWithMs);
    track->m_lastUpdateTime = QDateTime::fromString(json["lastUpdateTime"].toString(), Qt::ISODateWithMs);
    track->m_associatedCameraId = json["associatedCameraId"].toString();
    track->m_visuallyTracked = json["visuallyTracked"].toBool();
    track->m_classificationConfidence = json["classificationConfidence"].toDouble();
    track->m_engaged = json["engaged"].toBool();
    track->m_trackQuality = json["trackQuality"].toDouble();
    
    QJsonArray sources = json["detectionSources"].toArray();
    for (const auto& src : sources) {
        track->m_detectionSources.append(static_cast<DetectionSource>(src.toInt()));
    }
    
    return track;
}

double Track::distanceTo(const GeoPosition& pos) const {
    QMutexLocker locker(&m_mutex);
    
    // Haversine formula
    const double R = 6371000.0; // Earth radius in meters
    
    double lat1 = qDegreesToRadians(m_position.latitude);
    double lat2 = qDegreesToRadians(pos.latitude);
    double dLat = lat2 - lat1;
    double dLon = qDegreesToRadians(pos.longitude - m_position.longitude);
    
    double a = std::sin(dLat/2) * std::sin(dLat/2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon/2) * std::sin(dLon/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    
    double horizontalDist = R * c;
    double verticalDist = pos.altitude - m_position.altitude;
    
    return std::sqrt(horizontalDist * horizontalDist + verticalDist * verticalDist);
}

double Track::distanceTo(const Track& other) const {
    return distanceTo(other.position());
}

double Track::bearingTo(const GeoPosition& pos) const {
    QMutexLocker locker(&m_mutex);
    
    double lat1 = qDegreesToRadians(m_position.latitude);
    double lat2 = qDegreesToRadians(pos.latitude);
    double dLon = qDegreesToRadians(pos.longitude - m_position.longitude);
    
    double y = std::sin(dLon) * std::cos(lat2);
    double x = std::cos(lat1) * std::sin(lat2) -
               std::sin(lat1) * std::cos(lat2) * std::cos(dLon);
    
    double bearing = qRadiansToDegrees(std::atan2(y, x));
    if (bearing < 0) bearing += 360.0;
    
    return bearing;
}

void Track::updateFrom(const Track& other) {
    QMutexLocker locker(&m_mutex);
    
    m_position = other.m_position;
    m_velocity = other.m_velocity;
    
    // Only update classification if confidence is higher
    if (other.m_classificationConfidence > m_classificationConfidence) {
        m_classification = other.m_classification;
        m_classificationConfidence = other.m_classificationConfidence;
    }
    
    // Merge detection sources
    for (auto source : other.m_detectionSources) {
        if (!m_detectionSources.contains(source)) {
            m_detectionSources.append(source);
        }
    }
    
    m_lastUpdateTime = QDateTime::currentDateTimeUtc();
    m_coastCount = 0;
    
    emit positionChanged();
    emit velocityChanged();
    emit updated();
}

} // namespace CounterUAS
