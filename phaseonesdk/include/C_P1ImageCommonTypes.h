////////////////////////////////////////////////////////////////////////////////////////////////////
/// Declares the Phase One Image SDK types
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef PHASE_ONE_IMAGE_SDK_TYPES_H
#define PHASE_ONE_IMAGE_SDK_TYPES_H

#include <stdint.h>
#include "C_P1ImageCommonLinkage.h"

namespace P1
{
namespace ImageSdk
{

//-----------------------------------------
// Message/TRACE
//-----------------------------------------
enum MessageLevel
{
	MSG_LEVEL_OFF = 0,
	MSG_LEVEL_ERROR,
	MSG_LEVEL_WARNING,
	MSG_LEVEL_INFO_HIGHLIGHT,
	MSG_LEVEL_INFO,
	MSG_LEVEL_VERBOSE,
	MSG_LEVEL_DEBUG,
	MSG_LEVEL_MAX
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// structures
////////////////////////////////////////////////////////////////////////////////////////////////////

enum BayerDataRepFormat
{
	kBayerFormat_Unknown = 0,
	kBayerFormat_Bayer16
};

enum BitmapColorSpace
{
	kBitmapColorSpace_Undefined = 0,
	kBitmapColorSpace_sRGB				// sRGB (IEC61966-2.1)
};

struct EXPORT_API Version
{
//    C++11 forbids default member initializers, if initalizer list
//    is to be used
	uint32_t major;
	uint32_t minor;
	uint32_t build;
};

struct EXPORT_API Point2D
{
    double x;
    double y;
};

/** @brief Defines how to handle non-rectangular or non-upright canvas */
typedef enum CanvasClip
{
    /** @brief Canvas has the full RAW image
     *  @note This option will consider all input pixels, but may output blank pixels.
     */
    CanvasClipNone = 1,
    /** @brief Clip warped edges
     *  @note This option will only consider the maximum interior bounding box, and all pixels will be valid.
     */
    CanvasClipWarpLoose = 2,
} CanvasClip;


/** @brief The supported GeoTiff tags
 *  @details Please see Chapter 7. Requirements and Annex E in the GeoTiff specification at https://www.ogc.org/standards/geotiff for details of the supported keys
 */
enum GeoTiffKey {
    /** @brief Defines the model coordinate reference system
     *  @details Please see the requirements for this tag at http://docs.opengeospatial.org/is/19-008r4/19-008r4.html#_requirements_class_gtmodeltypegeokey
     */
    GTModelType = 0x400,
    
    /** @brief Defines is a raster is an area or a point
     *  @details Please see the requirements for this tag at http://docs.opengeospatial.org/is/19-008r4/19-008r4.html#_requirements_class_gtrastertypegeokey
     */
    GTRasterType = 0x0401,
    
    /** @brief Optional generic Citation of the GeoTiff file
     *  @details Please see the requirements for this tag at http://docs.opengeospatial.org/is/19-008r4/19-008r4.html#_requirements_class_citation_geokeys
     */
    GTCitation = 0x0402,

    /** @brief Describes the raster space and model space relation together with the GeoTiffModelTiepointTag tag
     *  @details Please see how coordinate transformation works in GeoTIFF specification at http://docs.opengeospatial.org/is/19-008r4/19-008r4.html#_coordinate_transformations and the requirements for this tag at http://docs.opengeospatial.org/is/19-008r4/19-008r4.html#_requirements_class_modelpixelscaletag
     *  @note This tag is mutually exclusive with the *GeoTiffModelTransformationTag*.
     */
    GeoTiffModelPixelScaleTag = 33550,
    
    /** @brief Describes the raster space and model space relation along with the GeoTiffModelPixelScaleTag tag
     *  @details Please see the requirements for this tag at http://docs.opengeospatial.org/is/19-008r4/19-008r4.html#_requirements_class_modeltiepointtag
     *  @note This tag is mutually exclusive with the *GeoTiffModelTransformationTag*.
     */
    GeoTiffModelTiepointTag = 33922,
    
    /** @brief Describes the raster space and model space relation with an affine transformation
     *  @details Please see the requirements for this tag at http://docs.opengeospatial.org/is/19-008r4/19-008r4.html#_requirements_class_modeltransformationtag
     *  @note This tag is mutually exclusive with the *GeoTiffModelPixelScaleTag*.
     */
    GeoTiffModelTransformationTag = 34264,
};

/** @brief The different value types supported by GeoTIFF specification */
enum GeoTiffValueType {
    GeoTiffValueTypeShort = 3,
    GeoTiffValueTypeDouble = 12,
    GeoTiffValueTypeAscii = 2,
};

/** @brief The description of a GeoTIFF field */
typedef struct EXPORT_API GeoTiffField {
    /** @brief The key of the GeoTIFF Field */
    GeoTiffKey key;
    /** @brief The value type of the GeoTIFF Field */
    GeoTiffValueType valueType;
    /** @brief In case the value type is GeoTiffValueTypeShort the value should be stored in this property */
    unsigned short value; // in case the valuetype is GeoTiffValueTypeShort and it the field contains a single value, this must be used over the data
    /** @brief In case the value type is NOT GeoTiffValueTypeShort the value should be stored in this field */
    void* data;
    /** @brief In case the value type is NOT GeoTiffValueTypeShort the count of value entries should be stored in this field */
    unsigned int count;
} GeoTiffField;

enum TiffTileSize
{
    noTiling,       /**< Dont use tiling when writing tiff file - image is stored as one large image */
    tileSize512,    /**< divide and store the image as tiles with the size of 512x512 pixels */
    tileSize1024,   /**< divide and store the image as tiles with the size of 1024x1024 pixels */
    tileSize2048    /**< divide and store the image as tiles with the size of 2048x2048 pixels */
};

/** @brief Generic export configuration */
struct EXPORT_API ImageExportCommonConfig
{
    /** @brief If the export should copy the EXIF tags (except GPS) from the source image*/
    bool  includeExif = true;
    /** @brief If the export should copy the GPS tags from the source image*/
    bool  includeGps = true;
    /** @brief If the export should copy the XMP block from the source image*/
    bool  includeXmp = true;
    /** @brief If the exported image should include a minumum IPTC block (date and time) */
    bool  includeIptc = true;
    /** @brief If the exported image should have a preview generated instead in inherit the one in the iiq file. This can be usefull if conversions, stitching or stacking is done on the image*/
    bool  generateNewPreview = false;

    /** @brief The byte size of the XMP block*/
    int   xmpSize = 16384;
    /** @brief The ICC color profile data */
    char* iccProfileData = nullptr; // TODO: Will have problem with copy assignment
    /** @brief The ICC color profile data size in bytes*/
    int   iccProfileSize = 0;

    /** @brief The ::GeoTIFF fields as an array */
    GeoTiffField* geoTiffFieldList = nullptr;
    /** @brief The count GeoTIFF fields */
    unsigned int geoTiffFieldCount = 0;
};


/**
 * @brief Configures the TIFF conversion
 */
struct EXPORT_API TiffConfig
{
    /** @brief The generic configuration*/
    ImageExportCommonConfig commonConfig;
    /** @brief To write metadata for multihead images (stitched or stacked images)*/
    bool  writeMetaDataForMultiHeadImage = false;
    /** @brief Tile size of the output TIFF file. */
    TiffTileSize tileSize = noTiling;
};

} // ImageSdk
} // P1

///< PHASE_ONE_IMAGE_SDK_TYPES_H
#endif
/// @}
