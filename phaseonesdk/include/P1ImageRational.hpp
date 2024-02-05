#ifndef _P1_IMAGE_SDK_RATIONAL_HPP_
#define _P1_IMAGE_SDK_RATIONAL_HPP_

#include <cstdint>
#include <string>

#include "C_P1ImageCommonLinkage.h"

namespace P1
{
namespace ImageSdk
{
    struct EXPORT_API Rational
    {
        int32_t Numerator;
        int32_t Denominator;

        double DoubleValue() const;
        std::string ToString() const;
    };
} // ImageSdk
} // P1

#endif // _P1_IMAGE_SDK_RATIONAL_HPP_
