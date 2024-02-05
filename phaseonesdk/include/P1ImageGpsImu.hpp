#ifndef _P1_IMAGE_SDK_GPS_IMU_HPP_
#define _P1_IMAGE_SDK_GPS_IMU_HPP_

#include "P1ImageRational.hpp"
#include <cstdint>

namespace P1
{
namespace ImageSdk
{
    /** @brief Definition of possible reference points for the Yaw axis */
    enum class ImuYawRef
    {
        /** The Yaw reference is not known */
        Unknown = 0,
        /** Yaw is relative to *True North*. (In constract to *Magnetic North*) */
        TrueNorth = 1
    };

    /**
     * @brief A Date & Time representation used by the GPS to provide sub-millisecond precision
     *
     * Note that the *date* part might not be present. (The presence depends on the GPS module used)
     * See @ref hasDate to check you date fields contains something meaningful.
     * 
     * The *timezone* is always GMT/UTC. However, the clock used (or time scale) might not be UTC.
     *
     * GPS uses its own timescale (or clock) called *GPS Time*. It branched out from UTC in 1980,
     * and has since lept ahead of UTC. The reason is that *GPS Time* does not account for the leap
     * seconds added to UTC (since 1980).
     *
     * The field @ref isGpsTime tells you if the timestamp is in *GPS Time* and not *UTC*. By January
     * 1st, 2017, the differnce between GPS Time and UTC is: GPS - UTC = +18 seconds.
     */
    struct EXPORT_API GpsDateTimeComponents
    {
        bool hasDate;
        uint32_t year;
        uint32_t month;
        uint32_t day;
        int hours;
        int minutes;
        double seconds;
        bool isGpsTime;

        std::string ToString() const;
    };

    /**
    * @brief A GPS data bundle, that defines an auxillary GPS's position on earth, 
    * along with a date-time stamp
    * 
    * Coordinates are stored in degrees and minutes (of arc) with decimals. 
    * (No seconds of arc components.)
    * 
    * Conveniece methods @ref LatitudeAsDecimal() and @ref LongitudeAsDecimal(),
    * returns the decimal degrees representation. These are calculated using the formular:
    * @code
    * decimalDegrees = degrees + minutes / 60
    * @endcode
    * here are no convenience converters for convertion to a degree, minute & second representation.
    * 
    * However, the seconds components can be calculated from the minutes:
    * @code
    * seconds = (minutes - floor(minutes)) * 60
    * @endcode
    * 
    * The GPS timestamp might not contain a date component. This depends on the GPS device used, 
    * when the IIQ file was created. The member @ref GpsDateTimeComponents.HasDate
    * defines if date components (year, month, day) are present.
    */
    struct EXPORT_API GpsData
    {
        /** @brief The GPS time stamp, that might include a date */
        GpsDateTimeComponents gpsDateTimeStamp;

        /** @brief North-South point in (integer) degrees from -90 (South Pole) or +90 (North Pole) */
        int latitudeDegrees;

        /** @brief *Minute of arc* part of the latitude coordinate (North-South) */
        double latitudeMinutes;

        /** @brief Return *degrees only* latitude coordinate. (Degrees with decimals) */
        double LatitudeAsDecimal() const;


        /** @brief East-West point in (integer) degrees from -180 (West) to +180 (East), where 0 is near Greenwich, England */
        int longitudeDegrees;

        /** @brief *Minute of arc* part of the longitude coordinate (East-West) */
        double longitudeMinutes;
        
        /** @brief Return *degrees only* longitude coordinate. (Degrees with decimals) */
        double LongitudeAsDecimal() const;


        /** @brief Height of camera in meters, above sea level. Negative values means below sea level. */
        Rational altitude;

        std::string ToString() const;
    };

    /**
    * @brief An IMU data bundle, that defines the orientation of an auxillary IMU in space
    * 
    * If the camera has been been fitted with an IMU, this is the IMU orientation snapshot 
    * when the image was taken.
    * 
    * The orientation is defined by the principal axes yaw, pitch and roll expressed 
    * in degrees.
    */
    struct EXPORT_API ImuData
    {
        /** @brief Rotation on the vertical axis, where positive direction is clock-wise. */
        Rational yaw;
        
        /** @brief Rotation on the tranversal axis, (wingtip to wingtip), positive direction raises the aircraft's nose. */
        Rational pitch;
        
        /** @brief Rotation on the longitudinal axis (aircraft's nose to tail), positive direction lifts the left wing, and lowers the right. */
        Rational roll;
        
        /** @brief The definition of the Yaw reference point */
        ImuYawRef yawRef;

        std::string ToString() const;
    };


} // ImageSdk
} // P1

#endif // _P1_IMAGE_SDK_GPS_IMU_HPP_
