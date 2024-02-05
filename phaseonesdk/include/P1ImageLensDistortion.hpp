#ifndef _P1_IMAGE_SDK_LENS_DISTORTION_HPP_
#define _P1_IMAGE_SDK_LENS_DISTORTION_HPP_

#include "P1ImageRational.hpp"
#include <cstdint>

namespace P1
{
namespace ImageSdk
{

    /** @brief The collection of geometric correction parameters, as they are stated in Australis Format in the Calibration Result form received with calibrated cameras.
     *
     */
    struct EXPORT_API GeometricCorrection
    {
        /** @brief Pixel size in mm*/
        double pixelSize = -1.0;
        /** @brief Lens focal lenght in mm*/
        double focalLength = -1.0;
        /** @brief Principal point offset x coordinate*/
        double xp = 0.0;
        /** @brief Principal point offset y coordinate*/
        double yp = 0.0;
        /** @brief First radial distortion coefficient*/
        double k1 = 0.0;
        /** @brief Second radial distortion coefficient*/
        double k2 = 0.0;
        /** @brief Third radial distortion coefficient*/
        double k3 = 0.0;
        /** @brief First radial-assymetric and tangential coefficient*/
        double p1 = 0.0;
        /** @brief Second radial-assymetric and tangential coefficient*/
        double p2 = 0.0;
        /** @brief Affinity and non-orthogonality coefficient for x coordinate*/
        double b1 = 0.0;
        /** @brief Affinity and non-orthogonality coefficient for y coordinate*/
        double b2 = 0.0;
        
        std::string ToString() const;
    };
} // ImageSdk
} // P1

#endif // _P1_IMAGE_SDK_LENS_DISTORTION_HPP_
