#ifndef _P1_IMAGE_SDK_TAG_ID_H_
#define _P1_IMAGE_SDK_TAG_ID_H_

#include "C_P1ImageCommonLinkage.h"
#include <cstdint>

namespace P1
{
namespace ImageSdk
{
    enum class EXPORT_API TagCategory
    {
        Any = 0,
        Preview,
        FullImage,
        Thumbnail
    };

    enum class EXPORT_API TagType
    {
        Tiff,
        Exif,
        CaptureTag,
        Xmp
    };

    struct EXPORT_API TagId
    {
        TagType mType;
        uint32_t mId;
        TagCategory mCategory;

        // Tiff tags

        /** @brief The preview (thumbnail) image width in pixels, taken from TIFF meta data */
        static const TagId PreviewWidth;
        /** @brief The preview (thumbnail) image height in pixels, taken from TIFF meta data */
        static const TagId PreviewHeight;
        /** @brief A vector with the size of each component (color) */
        static const TagId PreviewBitsPerSample;

        /** @brief The scanner / camera manufacturer */
        static const TagId Make;
        /** @brief The scanner / camera model name or number */
        static const TagId Model;

        /** @brief The number of pixels per ResolutionUnit in the ImageWidth direction */
        static const TagId XResolution;
        /** @brief The number of pixels per ResolutionUnit in the ImageLength direction */
        static const TagId YResolution;
        
        /** @brief The unit of measurement for XResolution and YResolution */
        static const TagId ResolutionUnit;

        /** @brief XML packet (text blob) containing XMP metadata */
        static const TagId XmpPacket;
        


        // Exif tags

        /** @brief Exposure time, given in seconds */
        static const TagId ExposureTime;
        /** @brief The F number */
        static const TagId FNumber;
        /** @brief The class of the program used by the camera to set exposure when the picture is taken */
        static const TagId ExposureProgram;
        /** @brief Indicates the ISO Speed and ISO Latitude of the camera or input device as specified in ISO 12232 */
        static const TagId IsoSpeedRatings;
        /** @brief The date and time when the original image data was generated */
        static const TagId DateTime;
        /** @brief Shutter speed (In APEX) */
        static const TagId ShutterSpeedValue;
        /** @brief Shutter speed (In APEX) */
        static const TagId ApertureValue;
        /** @brief The exposure bias (In APEX) */
        static const TagId ExposureBias;
        /** @brief The kind of light source (Exif defined enum) */
        static const TagId LightSource;
        /** @brief The actual focal length of the lens, in mm */
        static const TagId FocalLength;
        /** @brief Indicates the white balance mode set when the image was shot (0 = Auto, 1 = Manual) */
        static const TagId WhiteBalance;
        /** @brief Indicates an identifier assigned uniquely to each image */
        static const TagId ImageUniqueId;
  
        static const TagId FocalPlaneXResolution;
        static const TagId FocalPlaneYResolution;
        static const TagId FocalPlaneResolutionUnit;

        /** @brief The serial number of the lens used when the capture was taken */
        static const TagId LensSerialNumber;

        /** @brief The minimum aperture of the lens used for the capture */
        static const TagId ApertureMin;
        /** @brief The maximum aperture of the lens used for the capture */
        static const TagId ApertureMax;
        /** @brief The minimum focallength of the lens used for the capture */
        static const TagId FocalLengthMin;
        /** @brief The maximum focallength of the lens used for the capture */
        static const TagId FocalLengthMax;

        /** @brief The number of captures that the camera had taken, when this actual capture was taken */
        static const TagId CaptureNumber;

        /** @brief A description of the camera and its firmware that took the capture */
        static const TagId CameraDescription;

        /** @brief The rating of the picture */
        static const TagId Rating;


        // P1 Tags
        /** @brief The Camera's serial number */
        static const TagId SerialNumber; // = new TagId(TagType.CaptureTag, 0x102);
        /** @brief The name of the lens used */
        static const TagId LensName; // = new TagId(TagType.CaptureTag, 0x0412);
        
        /** @brief The Camera's orientation when picture was taken */
        static const TagId Orientation;
                
        /** @brief The Orientation the camera was setup to use. The normal value is automode - then the orientation is automaticly detected */
        static const TagId OrientationMode;

        /** @brief The pixel size in both x and y direction in micrometer */
        static const TagId SensorPixelPitch;
    };

}
}


#endif // _P1_IMAGE_SDK_TAG_ID_H_