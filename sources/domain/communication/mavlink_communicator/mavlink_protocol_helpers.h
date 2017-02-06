#ifndef MAVLINK_PROTOCOL_HELPERS_H
#define MAVLINK_PROTOCOL_HELPERS_H

// Qt
#include <QGeoCoordinate>

namespace domain
{
    int32_t inline hzToUs(float frequency)
    {
        return 1000000 / frequency;
    }

    inline int32_t encodeLatLon(double value)
    {
        return value * 1e7;
    }

    inline int32_t encodeAltitude(double value)
    {
        return value * 1000;
    }

    inline uint16_t encodeCourse(float value)
    {
        return value * 100;
    }

    inline double decodeLatLon(int32_t value)
    {
        return double(value) / 1e7;
    }

    inline float decodeAltitude(int32_t value)
    {
        return float(value) / 1000;
    }

    inline float decodeCourse(uint16_t value)
    {
        return float(value) / 100;
    }

    inline float decodeVoltage(uint16_t value)
    {
        return float(value) / 1000;
    }

    inline float decodeCurrent(int16_t value)
    {
        return float(value) / 10000;
    }

    inline QGeoCoordinate decodeCoordinate(int32_t lat, int32_t lon, int32_t alt)
    {
        if (lat == 0 && lon == 0) return QGeoCoordinate();

        return QGeoCoordinate(decodeLatLon(lat), decodeLatLon(lon),
                              decodeAltitude(alt));
    }
}

#endif // MAVLINK_PROTOCOL_HELPERS_H
