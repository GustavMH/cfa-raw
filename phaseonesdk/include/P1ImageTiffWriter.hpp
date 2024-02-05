#ifndef _P1_IMAGESDK_TIFFWRITER_HPP_
#define _P1_IMAGESDK_TIFFWRITER_HPP_

#include "C_P1ImageCommonTypes.h"
#include "P1ImageRawImage.hpp"

namespace P1
{
namespace ImageSdk
{

/**
 * @brief Writes a Tiff file to the disk synchronously
 *
 * @param filename The path to the output file, it may be a single character or wide character string.
 * @param bitmap The rendered bitmap
 * @param image The raw image's in memory representation
 * @param config Describes the TIFF metadata/preview
 * @throw SDKException The function will throw an exception in case of failure.
 */


class EXPORT_API TiffWriter
{
public:
    TiffWriter(std::string filename, P1::ImageSdk::BitmapImage& bitmap, P1::ImageSdk::RawImage& image, TiffConfig& config)  noexcept(false);
    
    TiffWriter(std::wstring filename, P1::ImageSdk::BitmapImage& bitmap, P1::ImageSdk::RawImage& image, TiffConfig& config)  noexcept(false);
};

} // ImageSdk
} // P1

#endif // _P1_IMAGESDK_TIFFWRITER_HPP_
