
#ifndef _P1_IMAGESDK_RAW_IMAGE_
#define _P1_IMAGESDK_RAW_IMAGE_

#include <C_P1ImageCommonTypes.h>
#include <C_P1ImageCommonLinkage.h>

#include "P1ImageRect.hpp"
#include "P1ImageConvertConfig.hpp"
#include "P1ImageDecodeConfig.hpp"
#include "P1ImageBitmapImage.hpp"
#include "P1ImageImageTag.hpp"
#include "P1ImageGpsImu.hpp"
#include "P1ImageLensDistortion.hpp"

#include <string>


namespace P1
{
namespace ImageSdk
{
    #if LIBTIFF_ENABLED
    class TiffWriter;
    #endif

    #if STITCHING_ENABLED
    class Stitching;
    #endif

    class RawImagePimpl; // forward decl

	/**
     * A handle to a Phase One IIQ image file, either on the file system or in a
     * memory buffer
     */
	class EXPORT_API RawImage
	{
	public:

		// Constructors & assignments

		/** @brief Constructs an empty and invalid RawImage object */
		RawImage();

		/** @brief Construct a RawImage from a file path */
		RawImage(std::string iiqPath);

#ifdef _MSC_VER
		/** @brief Construct a RawImage from a file path using UTF-16 based wchar_t - Windows only! */
		RawImage(std::wstring iiqPath);
#endif

		/** @brief Construct a RawImage from a memory pointer, to a complete in-memory IIQ file */
		RawImage(std::shared_ptr<const uint8_t> buffer, uint32_t bufferByteSize);

		RawImage(const RawImage& other);
		RawImage& operator= (const RawImage& other);

		// Acccessors

		/** @brief Get the image width, in pixels, of the exposed sensor area */
		uint32_t Width() const;

		/** @brief Get the image height, in pixels, of the exposed sensor area */
		uint32_t Height() const;

		/** @brief Get full width of the raw sensor output in pixels (including un-exposed area) */
		uint32_t RawSensorWidth() const;

		/** @brief Get the full height of the raw sensor output in pixels (including un-exposed area) */
		uint32_t RawSensorHeight() const;

		/** @brief Get the image dimensions, in pixels, of the exposed sensor area */
		Size GetDimensions() const;

		// Meta data

		/** @brief Extract and return meta data from the image container file (IIQ) */
		ImageTag GetTag(TagId id) const;

		/** @brief Check if a certain meta data tag exists in the image file */
		bool TagExists(TagId id) const;

		/**
		 * @brief Extract any RGB bitmap preview / thumbnail from meta data
		 * 
		 * IIQ files might contain a preview of the full image. This method
		 * extracts that image.
		 * 
		 * @param alloc An *optional* memory allocator. Default is standard C++ `new`. 
		 */
		BitmapImage GetPreview(std::allocator<uint8_t> alloc = std::allocator<uint8_t>());

		// Processing

		/**
		 * @brief Extract the Bayer (i.e RAW) data from the IIQ container file.
		 * 
		 * This operation consist of a a decompression stage and a calibration stage.
		 * 
		 * @param config The configuration of the static calibration options to be applied the sensor output
		 * @param alloc An *optional* memory allocator. Default is standard C++ `new`. 
		 */
		SensorBayerOutput Decode(const DecodeConfig& config, std::allocator<uint8_t> alloc=std::allocator<uint8_t>()) const;

		/**
		 * @brief Extract and de-mosaic (RAW convert) the Bayer data into an RGB bitmap image.
		 * 
		 * Use the @ref ConvertConfig to control cropping and scaling.
		 * @param config A configuration object specifying optional crop and scaling
		 * @param alloc An *optional* memory allocator. Default is standard C++ `new`. 
         * @param waitWhenBusy If true the function call will be blocked until the SDK is idle and able to accept the task. If it is false, it will throw an SDKException if the SDK is busy
		 */

        BitmapImage Convert(const ConvertConfig& config,
			std::allocator<uint8_t> alloc=std::allocator<uint8_t>()
			,BitmapImage * maskImage = nullptr
            ,bool waitWhenBusy = true) const;


		/**
		* @brief Convenience method to get the XMP meta data
		* 
		* This method equivalent to using GetTag() to get the XmpPacket tag, and trimming the output
		* for extranous white space.
		* 
		* @param alloc An *optional* memory allocator. Default is standard C++ `new`. 
		*/
		std::string GetXmpPacket(std::allocator<char> alloc = std::allocator<char>()) const;

		//bool HasGpsData() const;

		/**
		* @brief Extract the GPS data embedded in the IIQ files XMP meta data section, if any
		* 
		* This is a convenience method. It is equal to using the GetXmpPacket method
		* and parsing the GPS data tags.
		* 
		* If no GPS data is embedded in the IIQ file, this method will throw!
		*/
		GpsData GetGpsData() const;


		/**
		* @brief Get the GPS Event identifier (ID), if it exists for the IIQ file
		*
		* This is a convenience method. It is equal to using the GetXmpPacket method
		* and parsing the GPSEventID tag.
		*
		* If the IIQ file does not contain a GPSEventId, this method will throw!
		*/
		int32_t GetGpsEventId() const;

		//bool HasImuData() const;
		
		/**
		* @brief Extract the IMU data embedded in the IIQ files XMP meta data section, if any
		* 
		* This is a convenience method. It is equal to using the GetXmpPacket method
		* and parsing the IMU data tags.
		* 
		* If no IMU data is embedded in the IIQ file, this method will throw!
		*/
		ImuData GetImuData() const;	
        
        /**
        * @brief Generates a white balance gain value from a Point defined on the View
        *
        * The recommended way to use this function is to present an Image on Screen for the user to select the Point to pick the WB from.
        * Using a pointing device the user could select the Point on that Image View. The coordinate system for that Point should match the Image View's coordinate system, assuming the origin is the top left corner of that image.
        * To be able to map that Point on the image data, the input ConvertConfig must have the exact same geometry defined (including Crop and Scaling). You can reuse the same ConvertConfig used for the Image as it is not going to be modified.
        * Average RGB gain is calculated from the area defined by sampleSize and used to calculate a white balance.
        * @param config: A ConvertConfig object containing the Geometry transformation the view was based upon.
        * @param point: The X,Y coordinates of the center of the sampled area based on the View Coordinate System
        * @param sampleSize: A value between 1 and 10 that determines the size of the area to average. 1 is a single pixel area, 10 is 10x10 (100) pixels with x,y in the center
        * @return The white balance calculated based on the input. Note the alpha value of the Color is undefined
        */
        Color PickWhiteBalance(const ConvertConfig config, const Point2D point, int sampleSize) const;
        
        
        Size GetCanvasSize(const ConvertConfig config) const;
        
        /**
        * @brief Returns the geometric aberration information of the camera
        * @return The Australis geometric aberration parameters embedded in the raw file
        */
        GeometricCorrection GetGeometricAberration() const;
        

	private:
#if LIBTIFF_ENABLED
        friend class TiffWriter;
#endif

#ifdef STITCHING_ENABLED
		friend class P1::ImageSdk::Stitching;
		friend class P1::ImageSdk::Stacking;
#endif
		MSVC_ALLOW_PUBLIC_STL(std::shared_ptr<RawImagePimpl> mPimpl);
	};

} // namespace ImageSdk
} // namespace P1

#endif // _P1_IMAGESDK_CAPTURE_IMAGE_
