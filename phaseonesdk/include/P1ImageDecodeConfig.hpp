#ifndef _P1_IMAGE_SDK_DECODE_CONFIG_H_
#define _P1_IMAGE_SDK_DECODE_CONFIG_H_

#include <cstdint>
#include <memory>

#include "C_P1ImageCommonLinkage.h"
#include "P1ImageBitmapImage.hpp"

namespace P1
{
namespace ImageSdk
{
    /** @brief BlackClamp is the process of calibrating / correcting sensor black levels */
    enum class BlackClamp
    {
        /** @brief Apply black clamping to the pixel values */
        On,
        /** @brief Do not black clamp pixel values */
        Off,
        /** @brief Let the framework decide on whether to black clamp pixel values, or not */
        Default = On
    };

    /** @brief Digital Offset is a value added to all pixels, to avoid underflow of unsigned integers */
    enum class DigitalOffset
    {
        /** @brief Substract the the digital offset from the pixel values */
        Subtract,
        /** @brief Leaves the offset as-is */
        Keep,
        /** @brief Let the framework decide on whether digital offset are substracted from pixel values, or not */
        Default = Subtract,
    };

    class RawImage;
    class DecodeConfigPimpl;

    /**
     * @brief Class used to configure the *Decode* pipline stages
     */
    class EXPORT_API DecodeConfig
    {
        friend class RawImage;
    private:
        MSVC_ALLOW_PUBLIC_STL(std::unique_ptr<DecodeConfigPimpl> mPimpl);
    public:

        /** @brief A static config object, configured with default static calibration values */
        static const DecodeConfig Defaults;

        /** @brief Construct an empty configuration object */
        DecodeConfig();

        DecodeConfig(const DecodeConfig& other);
        DecodeConfig& operator=(const DecodeConfig& other);
        DecodeConfig& operator=(DecodeConfig&& other);

        ~DecodeConfig();

        // Accessors

        BlackClamp GetBlackClamp() const;
        DigitalOffset GetDigitalOffset() const;

        // Setters

        /** @brief Configure Black clamping and Digital offseting as part of the static calibration */
        DecodeConfig BlackLeveling(BlackClamp clamp = BlackClamp::Default, DigitalOffset offset = DigitalOffset::Default);

        /** @brief Apply this configuration to an actual @ref RawImage */
        SensorBayerOutput ApplyTo(const RawImage& image);
    };

}
}

#endif // _P1_IMAGE_SDK_DECODE_CONFIG_H_
