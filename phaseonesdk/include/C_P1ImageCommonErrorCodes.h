#ifndef _P1_IMAGE_SDK_COMMON_ERROR_CODES_H
#define _P1_IMAGE_SDK_COMMON_ERROR_CODES_H

namespace P1
{
namespace ImageSdk
{

//-----------------------------------------
	enum [[nodiscard]] ErrorCode
	{
		kNoError = 0,
		kSuccess = kNoError,

		P1ImageSDKReturn_UnknownError,

		P1ImageSDKReturn_GenericError,
		kErrorUnknown = P1ImageSDKReturn_UnknownError,

		P1ImageSDKReturn_ParametersError,
		kErrorParameterIsNull,

		kErrorInvalidParameter,

		P1ImageSDKReturn_FileOpenError,
		kErrorFileOpen,

		P1ImageSDKReturn_RamFileOpenError,
		P1ImageSDKReturn_BufferSizeError,
		kErrorBufferSize = P1ImageSDKReturn_BufferSizeError,

		P1ImageSDKReturn_ReadImageError,
		kErrorImageRead = P1ImageSDKReturn_ReadImageError,

		P1ImageSDKReturn_WriteImageError,
		kErrorImageWrite = P1ImageSDKReturn_WriteImageError,

		kErrorSdkStateInvalid,
		kErrorAssertionFailed,
		kErrorImageProcessing,

		kErrorTagNotFound,
		kErrorTagTypeInvalid,

		kErrorThumbnailReadFailed,

		kErrorAbiBoundaryConsistency,

		kErrorNotInitialized,

		kErrorTagXmpParsing,

		kErrorConfigurationModelPersistance,

		kErrorStitchingProblem,
		kErrorIllegalImageFileForStitching,
		kErrorImageIsNotTheExpected,
		kErrorCalibrationProblem,
		kErrorStackingProblem,
		kErrorCannotCreateStitcher,
	    kErrorStitcherObjectNotValid,
		kErrorStackerObjectNotValid,

        kErrorImageExport,

		kErrorIllegalValue,
		kErrorOutOfMemory,
        
        kErrorBusy,

		kErrorDllFileNotFound,
};

} } // P1::CameraSdk

#endif // _P1_IMAGE_SDK_COMMON_ERROR_CODES_H
