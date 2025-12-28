#include "utils/CoordinateUtils.h"
#include <QtMath>
#include <QSize>

namespace CounterUAS {

double CoordinateUtils::haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = qDegreesToRadians(lat2 - lat1);
    double dLon = qDegreesToRadians(lon2 - lon1);
    
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(qDegreesToRadians(lat1)) * std::cos(qDegreesToRadians(lat2)) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    
    return EARTH_RADIUS_M * c;
}

double CoordinateUtils::haversineDistance(const GeoPosition& p1, const GeoPosition& p2) {
    double horizontalDist = haversineDistance(p1.latitude, p1.longitude, 
                                               p2.latitude, p2.longitude);
    double verticalDist = p2.altitude - p1.altitude;
    return std::sqrt(horizontalDist * horizontalDist + verticalDist * verticalDist);
}

double CoordinateUtils::bearing(double lat1, double lon1, double lat2, double lon2) {
    double dLon = qDegreesToRadians(lon2 - lon1);
    double lat1Rad = qDegreesToRadians(lat1);
    double lat2Rad = qDegreesToRadians(lat2);
    
    double y = std::sin(dLon) * std::cos(lat2Rad);
    double x = std::cos(lat1Rad) * std::sin(lat2Rad) -
               std::sin(lat1Rad) * std::cos(lat2Rad) * std::cos(dLon);
    
    double bearing = qRadiansToDegrees(std::atan2(y, x));
    return std::fmod(bearing + 360.0, 360.0);
}

double CoordinateUtils::bearing(const GeoPosition& from, const GeoPosition& to) {
    return bearing(from.latitude, from.longitude, to.latitude, to.longitude);
}

GeoPosition CoordinateUtils::positionFromBearingDistance(const GeoPosition& origin,
                                                          double bearingDeg, double distanceM) {
    double bearingRad = qDegreesToRadians(bearingDeg);
    double angularDist = distanceM / EARTH_RADIUS_M;
    double lat1 = qDegreesToRadians(origin.latitude);
    double lon1 = qDegreesToRadians(origin.longitude);
    
    double lat2 = std::asin(std::sin(lat1) * std::cos(angularDist) +
                           std::cos(lat1) * std::sin(angularDist) * std::cos(bearingRad));
    
    double lon2 = lon1 + std::atan2(std::sin(bearingRad) * std::sin(angularDist) * std::cos(lat1),
                                    std::cos(angularDist) - std::sin(lat1) * std::sin(lat2));
    
    GeoPosition result;
    result.latitude = qRadiansToDegrees(lat2);
    result.longitude = qRadiansToDegrees(lon2);
    result.altitude = origin.altitude;
    
    return result;
}

QPointF CoordinateUtils::geoToLocal(const GeoPosition& pos, const GeoPosition& origin) {
    double x = (pos.longitude - origin.longitude) * degToMeterLon(origin.latitude);
    double y = (pos.latitude - origin.latitude) * DEG_TO_M_LAT;
    return QPointF(x, y);
}

GeoPosition CoordinateUtils::localToGeo(const QPointF& local, const GeoPosition& origin) {
    GeoPosition result;
    result.latitude = origin.latitude + local.y() / DEG_TO_M_LAT;
    result.longitude = origin.longitude + local.x() / degToMeterLon(origin.latitude);
    result.altitude = origin.altitude;
    return result;
}

QPointF CoordinateUtils::geoToScreen(const GeoPosition& pos, const GeoPosition& center,
                                      double zoom, const QSize& screenSize) {
    QPointF local = geoToLocal(pos, center);
    
    double scale = zoom * 10.0;  // Pixels per meter at zoom level
    
    double x = screenSize.width() / 2.0 + local.x() * scale;
    double y = screenSize.height() / 2.0 - local.y() * scale;  // Y inverted
    
    return QPointF(x, y);
}

GeoPosition CoordinateUtils::screenToGeo(const QPointF& screen, const GeoPosition& center,
                                          double zoom, const QSize& screenSize) {
    double scale = zoom * 10.0;
    
    double localX = (screen.x() - screenSize.width() / 2.0) / scale;
    double localY = -(screen.y() - screenSize.height() / 2.0) / scale;  // Y inverted
    
    return localToGeo(QPointF(localX, localY), center);
}

QString CoordinateUtils::toMGRS(const GeoPosition& pos, int precision) {
    // Simplified MGRS conversion
    // In a real implementation, use a proper MGRS library
    Q_UNUSED(precision)
    return QString("%1N %2E").arg(pos.latitude, 0, 'f', 4).arg(pos.longitude, 0, 'f', 4);
}

GeoPosition CoordinateUtils::fromMGRS(const QString& mgrs) {
    // Simplified parsing
    Q_UNUSED(mgrs)
    return GeoPosition();
}

QString CoordinateUtils::toDMS(double degrees, bool isLatitude) {
    char direction = (degrees >= 0) ? (isLatitude ? 'N' : 'E') : (isLatitude ? 'S' : 'W');
    degrees = std::abs(degrees);
    
    int d = static_cast<int>(degrees);
    double minFloat = (degrees - d) * 60.0;
    int m = static_cast<int>(minFloat);
    double s = (minFloat - m) * 60.0;
    
    return QString("%1°%2'%3\"%4").arg(d).arg(m).arg(s, 0, 'f', 2).arg(direction);
}

double CoordinateUtils::fromDMS(const QString& dms) {
    // Parse DMS string
    Q_UNUSED(dms)
    return 0.0;
}

double CoordinateUtils::degToMeterLon(double latitude) {
    return DEG_TO_M_LAT * std::cos(qDegreesToRadians(latitude));
}

} // namespace CounterUAS
