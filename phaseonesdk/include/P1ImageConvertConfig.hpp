#ifndef _P1_IMAGE_SDK_CONVERT_CONFIG_H_
#define _P1_IMAGE_SDK_CONVERT_CONFIG_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "C_P1ImageCommonLinkage.h"
#include "C_P1ImageCommonTypes.h"
#include "P1ImageBitmapImage.hpp"
#include "P1ImageClarity.hpp"
#include "P1ImageColor.hpp"
#include "P1ImageRect.hpp"
#include "P1ImageLensDistortion.hpp"

namespace P1
{
namespace ImageSdk
{

    /** @brief A structure defining a minimum, maximum and default values. To be used for numeric properties. */
    template <typename T>
    struct EXPORT_API ValueRange
    {
        /** @brief The minimum value */
        const T minValue;
        /** @brief The maximum value */
        const T maxValue;
        /** @brief The default value */
        const T defaultValue;
        
        /** @brief The constructor */
        ValueRange(T min,T max,T def);
        
        /** @brief Return a clamped value of the input */
        T TrimmedValue(T input) const;
        /** @brief Return true if the input matches the default value */
        bool IsDefaultValue(T input) const;
    };

    class ConvertConfigPimpl; // forward decl
    class RawImage; // forward decl

    /**
     * @brief Class that configures the *Convert* pipeline stages.
     * 
     * The convert config is where convertion from Bayer domain into the Rgb domain happens, you can configure cropping and scaling. The order is fixed.
     * Also you can enable and setup different kinds of image enhancements algorithms, known from other image editing tools. Also it is possible to enable
     * image corrections like lens distortion and light falloff.     
     */
    class EXPORT_API ConvertConfig
    {
        friend class RawImage;
    private:
        MSVC_ALLOW_PUBLIC_STL(std::unique_ptr<ConvertConfigPimpl> mPimpl);
    public:

        ConvertConfig();
        ConvertConfig(const ConvertConfig& other);

        ConvertConfig& operator=(const ConvertConfig& other);      

        ~ConvertConfig();

        // Accessors

        /** @brief Returns the current CanvasClip setting.*/
        CanvasClip GetCanvasClip() const;
        /** @brief Configure the output image canvas edge handling.
         *  @param canvasClip The new CanvasClip setting.
         *  @return This object itself (this), to allow chaining
         */
        ConvertConfig SetCanvasClip(CanvasClip canvasClip);
        
        /** @brief Get any configured crop */
        Rect GetCrop() const;

        bool HasCrop() const;
        

        bool HasOutputScaling() const;

        /** @brief Get any configured scaling factor, or a negative number, if none if set */
        double GetOutputScale() const;

        /** @brief Get any configured scaling by fixed output width, or a negative number, if none if set */
        int32_t GetOutputWidth() const;

        /** @brief Get any configured scaling by fixed output height, or a negative number, if none if set */
        int32_t GetOutputHeight() const;

        /** @brief The data format of the pixels */
        BitmapFormat GetOutputFormat() const;

        // Setters

        /** @brief Crop image to a given rectangle, in coordinates of the input image */
        ConvertConfig SetCrop(int32_t x, int32_t y, int32_t width, int32_t height);

        /** @brief Crop image to a given rectangle, in coordinates of the input image */
        ConvertConfig SetCrop(const Rect& cropRect);

        /** @brief Scale image relative to the full image size, with aspect ratio enforced */
        ConvertConfig SetOutputScale(double scaleFactor);

        /** @brief Set the scaling to match a given destination width, aspect ratio is still enforced */
        ConvertConfig SetOutputWidth(int32_t destinationWidth);

        /** @brief Set the scaling to match a given destination height, aspect ratio is still enforced */
        ConvertConfig SetOutputHeight(int32_t destinationHeight);

        /** @brief Set the pixel data format of the output image */
        ConvertConfig SetOutputFormat(BitmapFormat format);


        ConvertConfig SetNoiseModel(float param1, float param2);
        
        /** @brief Get the value of the contrast setting in the convert config. See also SetContrast function */
        float GetContrast() const;

        /** @brief Adjust the contrast of the image 
            @param contrast is a value between -1.0 and 1.0. Default is 0.0
        */
        ConvertConfig SetContrast(const float contrast);        

        /** @brief Get the value of the brightness setting in the convert config. See also SetBrightness function */
        float GetBrightness() const;

        /** @brief Adjust the brightness of the image
            @param brightness is a value between -1.0 and 1.0. Default is 0.0
        */
        ConvertConfig SetBrightness(const float brightness);

        /** @brief Get the value of the saturation setting in the convert config. See also SetSaturation function */
        float GetSaturation() const;
        /** @brief Adjust the saturation of the image
            @param saturation is a value between -1.0 and 1.0. Default is 0.0
        */        
        ConvertConfig SetSaturation(const float saturation);
     
        /** @brief Get the value of the exposure setting in the convert config. See also SetExposure function */
        float GetExposure() const;
        /** @brief Adjust the exposure of the image
            @param exposure is a value between -4.0 and 4.0. Default is 0.0
        */
        ConvertConfig SetExposure(const float exposure);
        
        /** @brief Get the value of the gain setting in the convert config. See also SetGain function */
        float GetGain() const;
        /** @brief Adjust the gain of the image - this is a variant of the exposure setting
            @param gain is a floating point value. Default is 1.0 
        */
        ConvertConfig SetGain(const float gain);
        
        
        /** @brief Get the black level setting in the convert config. See also SetBlackLevel functions*/
        Color GetBlackLevel();
        /** @brief Set the black level. This is a color that is regarded as black        
            @param r,g,b are values between 0.0 and 1.0. Default is 0.0
        */
        ConvertConfig SetBlackLevel(float r, float g, float b);
        /** @brief Set the black level. This is a color that is regarded as black
            @param color where the r,g,b color is set as a struct.
        */
        ConvertConfig SetBlackLevel(Color color);

        /** @brief Get the midtone level setting in the convert config. See also SetMidtoneLevel functions*/
        Color GetMidtoneLevel();
        /** @brief Set the midtone level. This is a color that is used as 50% level of the color values
        @param r,g,b are values between 0.0 and 1.0. Default is 0.0
        */
        ConvertConfig SetMidtoneLevel(float r, float g, float b);
        /** @brief Set the midtone level. This is a color that is used as 50% level of the color values
           @param color where the r,g,b color is set as a struct.
       */
        ConvertConfig SetMidtoneLevel(Color color); 

        /** @brief Get the white level setting in the convert config. See also SetWhiteLevel functions*/
        Color GetWhiteLevel();
        /** @brief Set the white level. This is a color that is regarded as white
         @param r,g,b are values between 0.0 and 1.0. Default is 0.0
        */
        ConvertConfig SetWhiteLevel(float r, float g, float b);
        /** @brief Set the white level. This is a color that is regarded as white
         @param color where the r,g,b color is set as a struct.
        */
        ConvertConfig SetWhiteLevel(Color color);

//        ConvertConfig SetColorBalanceShadow(float r, float g, float b);
//
//        ConvertConfig SetColorBalanceMidtone(float r, float g, float b);
//
//        ConvertConfig SetColorBalanceHighlight(float r, float g, float b);
        
        
        /** @brief Set the level used for black. This is a color that black color is mapped into (as in a level tool)
            @param r,g,b are values between 0.0 and 1.0. Default is 0.0
        */
        ConvertConfig SetBlackLevelTarget(float r, float g, float b);
        
        /** @brief Set the level used for white. This is a color that white color is mapped into (as in a level tool)
            @param r,g,b are values between 0.0 and 1.0. Default is 0.0
        */
        ConvertConfig SetWhiteLevelTarget(float r, float g, float b);
        
        /** @brief Get the value of the sharpening amount in the convert config. See also SetSharpening function */
        float GetSharpeningAmount();
        /** @brief Adjust the amount of sharpening of the image
            @param sharpeningAmount is a value between -10.0 and 10.0. Default is 0.0
        */    
        ConvertConfig SetSharpeningAmount(float sharpeningAmount);

        /** @brief Get the pixel radius used for the Noise Reduction algorithm to sample neighbouring pixels to reduce noise. */
        int GetNoiseReductionRadiusAmount();
        /** @brief Sets the pixel radius used for the Noise Reduction algorithm to reduce noise.
         *  @param noiseReductionRadius is a value between 0 and 32. Default is 0.
         */
        ConvertConfig SetNoiseReductionRadiusAmount(int noiseReductionRadius);
        
        /** @brief Get the setup of the clarity tool. See also SetClarity function for further explanation */
        Clarity GetClarity();
        /** @brief Setup the clarity tool. Its used as a tool to help control the local contrast in the image. A negative value will decrease local contrast and and positive increase it. 
          Its a combination of selecting a method and a factor (amount)
          @param clarityMethod is the method used for clarity tool. It can be set to: clarityMethodNeutral = 0, clarityMethodPunch=1, clarityMethodNatural=2. Default is clarityMethodNeutral=0
          @param clarityFactor is the value defining the factor used for the clarity tool. It is a value between -1.0 and 1.0. Default is 0.0 for disabled
        */
        ConvertConfig SetClarity(int clarityMethod, float clarityFactor);
        /** @brief Setup the clarity tool. It's used as a tool to help control the local contrast in the image. A negative value will decrease local contrast and and positive increase it.
            Its a combination of selecting a method and a factor (amount)
            @param clarity is a struct with two members: the method and the factor. See SetClarity(int clarityMethod, float clarityFactor)
        */
        ConvertConfig SetClarity(Clarity clarity);
        
        /** @brief Get the value of the shadow recovery setting in the convert config. See also SetShadowRecovery function */
        float GetShadowRecovery() const;
        /** @brief Set shadow recovery. Used to extract details only in the shadows of the image by making them brighter.
        * @param shadowRecovery is a value to control how much effect is wanted. The value can be between 0.0 and 1.0. Default is 0.0 (=disabled)
        */
        ConvertConfig SetShadowRecovery(const float shadowRecovery);
        
        /** @brief Get the value of the highight recovery setting in the convert config. See also SetHighlightRecovery function */
        float GetHighlightRecovery() const;
        /** @brief Set highlight recovery. Used to extract details only in the highlights of the image by reducing the brightness of highlight pixels.
        * @param shadowRecovery is a value to control how much effect is wanted. The value can be between 0.0 and 1.0. Default is 0.0 (=disabled)
        */
        ConvertConfig SetHighlightRecovery(const float highlightRecovery);
        
        /** @brief Request in which stage of the processing pipeline a histogram must be sampled (its mainly used for debugging). Default is to not sample a histogram
            @param stage is a value defining the stage in processing where histogram is sampled
        */ 
        ConvertConfig SetHistogramStage(const int stage);
        
        
        
        /** @brief Get a vector of discreete coordinate pairs (x,y) describing the red curve used. See also SetRedCurve.*/
        const std::vector<Point2D> GetRedCurve();
        /** @brief Set the curve for red values as known from the curves tool in most image editors. The curve is defined as a vector of discreete coordinate pairs (x,y) that is interpolated. Default is a linear mapping: (0.0,0.0)-(1.0,1.0).            
            @param points is a vector of discreete coordinate pairs (x,y) in the range (0.0,0.0)-(1.0,1.0).
        */
        ConvertConfig SetRedCurve(const std::vector<Point2D>& points);
        
        /** @brief Get a vector of discreete coordinate pairs (x,y) describing the green curve used. See also SetGreenCurve.*/
        const std::vector<Point2D> GetGreenCurve();
        /** @brief Set the curve for green values as known from the curves tool in most image editors. The curve is defined as a vector of discreete coordinate pairs (x,y) that is interpolated. Default is a linear mapping: (0.0,0.0)-(1.0,1.0).
         It is similar to the curves tool known from most image editors
            @param points is a vector of discreete coordinate pairs (x,y) in the range (0.0,0.0)-(1.0,1.0).
        */
        ConvertConfig SetGreenCurve(const std::vector<Point2D>& points);

        /** @brief Get a vector of discreete coordinate pairs (x,y) describing the blue curve used. See also SetBlueCurve.*/
        const std::vector<Point2D> GetBlueCurve();
        /** @brief Set the curve for blue values as known from the curves tool in most image editors. The curve is defined as a vector of discreete coordinate pairs (x,y) that is interpolated. Default is a linear mapping: (0.0,0.0)-(1.0,1.0).
         It is similar to the curves tool known from most image editors
          @param points is a vector of discreete coordinate pairs (x,y) in the range (0.0,0.0)-(1.0,1.0).
        */
        ConvertConfig SetBlueCurve(const std::vector<Point2D>& points);
        
        /** @brief Get a vector of discreete coordinate pairs (x,y) describing the luma curve used. See also SetLumaCurve.*/
        const std::vector<Point2D> GetLumaCurve();
        /** @brief Set the curve for luma values as known from the curves tool in most image editors. The curve is defined as a vector of discreete coordinate pairs (x,y) that is interpolated. Default is a linear mapping: (0.0,0.0)-(1.0,1.0).
         It is similar to the curves tool known from most image editors
          @param points is a vector of discreete coordinate pairs (x,y) in the range (0.0,0.0)-(1.0,1.0).
        */
        ConvertConfig SetLumaCurve(const std::vector<Point2D>& points);

        /** @brief Get a vector of discreete coordinate pairs (x,y) describing the rgb curve used. See also SetLumaCurve.*/
        const std::vector<Point2D> GetRgbCurve();
        /** @brief Set the curve for rgb values as known from the curves tool in most image editors. The curve is defined as a vector of discreete coordinate pairs (x,y) that is interpolated. Default is a linear mapping: (0.0,0.0)-(1.0,1.0).
         It is similar to the curves tool known from most image editors
          @param points is a vector of discreete coordinate pairs (x,y) in the range (0.0,0.0)-(1.0,1.0).
        */
        ConvertConfig SetRgbCurve(const std::vector<Point2D>& points);

        /** @brief Returns the White Balance as gain for each channels (RGB) */
        const Color GetWhiteBalanceGain() const;
        /** @brief Sets the White Balance as gain for each channels (RGB) 
        *   @param whiteBalanceGain is a Color with each field between 0.0f and 10.0f.
        */
        ConvertConfig SetWhiteBalanceGain(const Color whiteBalanceGain);
        
        /** @brief Returns a Boolean value indicating if the conversion will include Geometric Aberration correction */
        bool GetGeomerticCorrectionEnabled() const;
        /** @brief Sets the Enabled state of the Geometric Aberration correction
         *  @param enable true will enable Geometric Aberration correction, false will disable it.
         */
        ConvertConfig SetGeomerticCorrectionEnabled(const bool enable);

        /** @brief Custom Geometric Aberration correction parameters */
        const GeometricCorrection GetGeometricCorrection() const;
        /** @brief Sets Geometric Aberration correction parameters to be applied
         *  @param geometricCorrection the geometric correction to be used for the algorithm.
         *  @note If geometricCorrection is set to an invalid value, the the conversion will try to load the geometric correction data embedded in the source IIQ file
         */
        ConvertConfig SetGeometricCorrection(const GeometricCorrection geometricCorrection);

        /** @brief Returns a Boolean value indicating if the conversion will include Lens Light falloff correction for recognized lenses */
        bool GetLightFalloffCorrectionEnabled() const;
        /** @brief Sets the Enabled state of the Lens Light falloff correction
         *  @param enable true will enable Lens Light falloff correction, false will disable it.
         */
        ConvertConfig SetLightFalloffCorrectionEnabled(const bool enable);

        /** @brief Returns a Boolean value indicating if the conversion will include Chromatic Aberration (CA) correction */
        bool GetChromaticAberrationCorrectionEnabled() const;
        /** @brief Sets the Enabled state of the Chromatic Aberration correction
         *  @param enable true will enable Chromatic Aberration correction, false will disable it.
         */
        ConvertConfig SetChromaticAberrationCorrectionEnabled(const bool enable);

        /** @brief Threshold for under and over exposure mask */
        bool GetExposureWarningMaskEnabled() const;
        /** Control generation of exposure mask in the mask image. True means enable. Default is disabled. When doing the actual convertion of an image - a mask buffer must be provided. 
         If enabled the underexposed threshold and overexposed threshold should also be set (See SetExposureMaskThreadhold function)
        */
        ConvertConfig SetExposureWarningMaskEnabled(const bool enable);

        /** @brief Get the value of the overexposed setting used when exposure mask generation is enabled*/
        float GetMaskThresholdOverExposure();
        /** @brief Get the value of the underexposed setting used when exposure mask generation is enabled*/
        float GetMaskThresholdUnderExposure();
        /** @brief Set the value of the overexposed and underexposed settings used when exposure mask generation is enabled.
        @param underExposeThreshold is a value between 0.0 and 1.0 so it is independend of the bitdepth of the image 
        @param overExposeThreshold is a value between 0.0 and 1.0 so it is independend of the bitdepth of the image 
        */
        ConvertConfig SetExposureMaskThreadhold(float underExposeThreshold, float overExposeThreadhold);

        /** @brief Get the setting for clip warning mask enabled. See also SetClipWarningMaskEnabled and SetMaskThresholdClipWarningCount functions */
        bool GetClipWarningMaskEnabled() const;
        /** @brief enable the clip warning mask. 
        For this setting to  be used the mask image must be enabled and provided when processing the image 
        @param enable. can be true of false. false is the default value
        */
        ConvertConfig SetClipWarningMaskEnabled(bool enable);

        /** @brief Get the count of neighbour pixels that must be outside the threshold limits to be set in the clip warning mask */
        int GetMaskThresholdClipWarningCount();
        /** @brief Set the minimum required count of neighbour pixels that must be outside the threshold limits to be set in the clip warning mask. For this to take effect the mask image must be enabled .
          @param thresholdCount is a value between 1 and 8. 
        */
        ConvertConfig SetMaskThresholdClipWarningCount(int thresholdCount);

        /** @brief Get EnableLinearPath setting. See also description of SetEnableLinearPath */
        bool GetEnableLinearPath() const;
        /** @brief Set EnableLinearPath setting. This setting is used to disable most of the processing steps to make sure that the debayering of the image into rgb domain is done
        only using a simple linear mapping enabling only the minimum needed processing.
        @param enable. it can be set to true or false. False is the default value.
        */
        ConvertConfig SetEnableLinearPath(const bool enable);

        /** @brief The ValueRange of the Exposure value
         *      - Minimum: -4.0
         *      - Maximum: 4.0
         *      - Default: 0.0
         */
        static const ValueRange<float> ExposureRange;

        /** @brief The ValueRange of the Contrast value
         *      - Minimum: -1.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         */
        static const ValueRange<float> ContrastRange;

        /** @brief The ValueRange of the Brightness value
         *      - Minimum: -1.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         */
        static const ValueRange<float> BrightnessRange;

        /** @brief The ValueRange of the Saturation value
         *      - Minimum: -1.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         */
        static const ValueRange<float> SaturationRange;

        /** @brief The ValueRange of the HighlightRecovery value
         *      - Minimum: 0.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         */
        static const ValueRange<float> HighlightRecoveryRange;

        /** @brief The ValueRange of the ShadowRecovery value
         *      - Minimum: 0.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         */
        static const ValueRange<float> ShadowRecoveryRange;

        /** @brief The ValueRange of the BlackLevel value
         *      - Minimum: 0.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         */
        static const ValueRange<float> BlackLevelRange;

        /** @brief The ValueRange of the WhiteLevel value
         *      - Minimum: 0.0
         *      - Maximum: 1.0
         *      - Default: 1.0
         */
        static const ValueRange<float> WhiteLevelRange;

        /** @brief The ValueRange of the MidtoneLevel value
         *      - Minimum: -1.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         */
        static const ValueRange<float> MidtoneLevelRange;

        /** @brief The ValueRange of the X and Y field of each element of the curve type properties
         *      - Minimum: 0.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         *
         *  The properties this applies to are the following
         *      - RedCurve
         *      - BlueCurve
         *      - GreenCurve
         *      - LumaCurve
         *      - RGBCurve
         */
        static const ValueRange<double> CurveInOutRange;

        /** @brief The ValueRange of the Method field of the Clarity value.
         *      - Minimum: clarityMethodNeutral = 0
         *      - Maximum: clarityMethodNatural = 2
         *      - Default: clarityMethodNeutral = 0
         */
        static const ValueRange<int> ClarityMethodRange;

        /** @brief The ValueRange of the Factor field of the Clarity property
         *      - Minimum: -1.0
         *      - Maximum: 1.0
         *      - Default: 0.0
         */
        static const ValueRange<float> ClarityAmountRange;

        /** @brief The ValueRange of the NoiseReductionRadius property
         *      - Minimum: 0
         *      - Maximum: 32
         *      - Default: 0
         */
        static const ValueRange<int> NoiseReductionRadiusRange;

        /** @brief The ValueRange of the RGB channels of the White Balance Gain property
         *      - Minimum: 0
         *      - Maximum: 10.0
         *      - Default: 0.0, meaning to use the metadata embedded into the IIQ file.
         */
        static const ValueRange<float> WhiteBalanceGainRange;

        /** @brief The ValueRange of the focalLength field of the GeometricCorrection value
         *      - Minimum: 23.0
         *      - Maximum: 1000.0
         */
        static const ValueRange<float> GeometricCorrectionFocalLengthRange;

        /** @brief The ValueRange of the pixelSize field of the GeometricCorrection value.
         *      - Minimum: 0.001
         *      - Maximum: 0.05317
         *      - Default: -1.0, indicating that the GeometricCorrection is unset, and is not to be used.
         */
        static const ValueRange<float> GeometricCorrectionPixelPitchRange;

        /** @brief The ValueRange of the xP and yP fields of the GeometricCorrection value
         *      - Minimum: -30.0
         *      - Maximum: 30.0
         *      - Default: 0.0
         */
        static const ValueRange<float> GeometricCorrectionShiftRange;

        /** @brief The ValueRange of the k1, k2, k3, p1, p2, b1, b2 fields of the GeometricCorrection value
         *      - Minimum: -1.0e-4
         *      - Maximum: 1.0e-4
         *      - Default: 0.0
         */
        static const ValueRange<float> GeometricCorrectionPolinomRange;
        
        /** @brief Apply your current configuration to a @ref RawImage */
        BitmapImage ApplyTo(const RawImage& image, BitmapImage * mask = nullptr, bool waitForBusySDK = true);

        /** @brief Store the configuration into a formatted file
            @param filename: the full path of the output
            @throw Throws an SDKException in case of failure (eg. failed to write to file)
         */
        void SaveToFile(const char* filename) const;
        
        /** @brief Creates a configuration from a formatted file
            @param filename: the full path of the input
            @throw Throws an SDKException in case of failure (eg. failed to read from file, the file was malformed)
            @return A smart pointer to a newly created ConvertConfig object
         */
        static ConvertConfig LoadFromFile(const char* filename);


        /** @brief Converts the object into a JSON formatted string
            @throw Throws an SDKException in case of failure (eg. failed to write to file)
            @return A string which contains the JSON representation of the object
         */
        std::string Serialize() const;

        /** @brief Creates a configuration from a formatted JSON string
            @param serializedData: a formatted JSON string
            @throw Throws an SDKException in case of failure (eg. the JSON string was malformed)
            @return A smart pointer to a newly created ConvertConfig object
         */
        static ConvertConfig Deserialize(const std::string serializedData);
        
        /** @brief Returns a list of 'Y' values of the Curve defined by the input Points. This is applicable for the RGB/LumaCurve)
            @param points: the list of points defining the Curve, X and Y value must be in between 0 and 1
            @param bits: The resolution of the return value. Valid range: [0,12]
            @throw Throws an SDKException in case of failure (eg. input point list has less than two points)
            @return A vector of the Y values of the evenly distributed between the two end points of the range [0, 1)
         */
        static std::vector<float>DiscretizeCurve(const std::vector<Point2D>& points, unsigned int bits);
    };

}
}

#endif // _P1_IMAGE_SDK_CONVERT_CONFIG_H_
