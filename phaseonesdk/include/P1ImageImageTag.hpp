#ifndef _P1_IMAGE_SDK_IMAGE_TAG_H_
#define _P1_IMAGE_SDK_IMAGE_TAG_H_

#include "P1ImageTagId.hpp"
#include "P1ImageRational.hpp"

#include <memory>
#include <string>
#include <sstream>

namespace P1
{
namespace ImageSdk
{

    enum class EXPORT_API TiffTagType
    {
        ifdBYTE = 1,
        ifdASCII = 2,
        ifdSHORT = 3,
        ifdLONG = 4,
        ifdRATIONAL = 5,
        ifdSBYTE = 6,
        ifdUNDEFINED = 7,
        ifdSSHORT = 8,
        ifdSLONG = 9,
        ifdSRATIONAL = 10,
        ifdFLOAT = 11,
        ifdDOUBLE = 12,
        ifdIFD = 13
    };

    class RawImage; // forward decl
    class ImageTagPimpl; // forward decl
    class EXPORT_API ImageTag
    {
        friend class RawImage;
    private:
        MSVC_ALLOW_PUBLIC_STL(std::shared_ptr<ImageTagPimpl> mPimpl);

    public:
        TagId Id() const;
        TiffTagType Type() const;
        uint32_t Count() const;
        uint32_t Bytes() const;
        uint32_t Value() const;

        std::shared_ptr<uint8_t> Data(std::allocator<uint8_t> alloc = std::allocator<uint8_t>()) const;

        std::string StringValue() const;
        Rational RationalValue() const;
        double DoubleValue() const;

        std::string ToString() const;

    };

}
}


#endif // _P1_IMAGE_SDK_IMAGE_TAG_H_