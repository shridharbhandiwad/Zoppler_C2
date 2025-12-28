#ifndef COORDINATEUTILS_H
#define COORDINATEUTILS_H

#include <QPointF>
#include "core/Track.h"

namespace CounterUAS {

/**
 * @brief Coordinate conversion utilities
 */
class CoordinateUtils {
public:
    // Distance calculations
    static double haversineDistance(double lat1, double lon1, double lat2, double lon2);
    static double haversineDistance(const GeoPosition& p1, const GeoPosition& p2);
    
    // Bearing calculations
    static double bearing(double lat1, double lon1, double lat2, double lon2);
    static double bearing(const GeoPosition& from, const GeoPosition& to);
    
    // Position offset
    static GeoPosition positionFromBearingDistance(const GeoPosition& origin,
                                                    double bearingDeg, double distanceM);
    
    // Coordinate conversions
    static QPointF geoToLocal(const GeoPosition& pos, const GeoPosition& origin);
    static GeoPosition localToGeo(const QPointF& local, const GeoPosition& origin);
    
    // Screen coordinate conversions (for map display)
    static QPointF geoToScreen(const GeoPosition& pos, const GeoPosition& center,
                               double zoom, const QSize& screenSize);
    static GeoPosition screenToGeo(const QPointF& screen, const GeoPosition& center,
                                   double zoom, const QSize& screenSize);
    
    // MGRS/UTM conversions
    static QString toMGRS(const GeoPosition& pos, int precision = 5);
    static GeoPosition fromMGRS(const QString& mgrs);
    
    // Degree/Minute/Second conversions
    static QString toDMS(double degrees, bool isLatitude);
    static double fromDMS(const QString& dms);
    
    // Constants
    static constexpr double EARTH_RADIUS_M = 6371000.0;
    static constexpr double DEG_TO_M_LAT = 111000.0;
    
    static double degToMeterLon(double latitude);
};

} // namespace CounterUAS

#endif // COORDINATEUTILS_H
