#ifndef _P1_IMAGE_SDK_BITMAP_IMAGE_H_
#define _P1_IMAGE_SDK_BITMAP_IMAGE_H_

#include <cstdint>
#include <memory>
#include "C_P1ImageCommonLinkage.h"

#include "P1ImageRect.hpp"

namespace P1
{
namespace ImageSdk
{
    class BitmapImagePimpl; // forward decl

    /** @brief Supported RGB bitmap formats */
    enum class BitmapFormat
    {
        Unknown = 0,       
        Rgb24=1,         /**< Each pixel is 3 bytes, 8-8-8 RGB */
        Rgb48=2,         /**< Each pixel is 6 bytes, 16-16-16 RGB */
        Argb32=3,        /**< Each pixel is 4 bytes, 8-8-8-8 RGB */
        Grey8=4,         /**< Each pixel is 1 byte, 8 Greytone */
        Grey16=5,        /**< Each pixel is 2 bytes, 16 Greytone */
        Argb64=6         /**< Each pixel is 8 bytes, 16-16-16-16 RGB */
    };

    /** @brief Supported Bayer data formats */
    enum class BayerFormat
    {
        Unknown = 0,

        Bayer16         /**< Each Bayer pixel is 16-bit, little endian */
    };

    class EXPORT_API BitmapBase
    {
        friend class RawImage;
#if STITCHING_ENABLED
        friend class Stitching;
        friend class Stacking;
#endif
    protected:
        MSVC_ALLOW_PUBLIC_STL(std::shared_ptr<BitmapImagePimpl> mPimpl);
    public:

        /** @brief Size of the data array, in bytes */
        uint32_t ByteSize() const;

        /** @brief 1-dimensional array of pixel data, in row-major order */
        MSVC_ALLOW_PUBLIC_STL(std::shared_ptr<uint8_t> Data());
    };

    /**
     * @brief In-memory representation of an RGB bitmap image
     * 
     */
    class EXPORT_API BitmapImage : public BitmapBase
    {
        friend class RawImage;
#if STITCHING_ENABLED
        friend class Stitching;
        friend class Stacking;
#endif
    public:

        /** @brief The pixel format of the @ref Data array */
        BitmapFormat Format() const;

        /** @brief Image Width in pixels */
        uint32_t Width() const;

        /** @brief Image height in pixels */
        uint32_t Height() const;

        /** @brief If this image is cropped from a larger source image, 
         * this is the crop in source coordinates */
        Rect SourceCrop() const;
    };

    /**
     * @brief In-memory representation of the image sensor Bayer output data
     */
    class EXPORT_API SensorBayerOutput : public BitmapBase
    {
        friend class RawImage;
    public:

        /** @brief The pixel format of the @ref Data array */
        BayerFormat Format() const;

        /** @brief The area of the sensor in pixels, that is exposed to light */
        Rect ActiveArea() const;

        /** @brief Full sensor width in pixels, including borders not exposed to light */
        uint32_t FullWidth() const;

        /** @brief Full sensor height in pixels, including borders not exposed to light */
        uint32_t FullHeight() const;
    };

}
}

#endif //_P1_IMAGE_SDK_BITMAP_IMAGE_H_
