#ifndef _P1_IMAGE_SDK_EXCEPTIONS_
#define _P1_IMAGE_SDK_EXCEPTIONS_

#include <stdexcept>
#include <string>

#include "C_P1ImageCommonLinkage.h"
#include <C_P1ImageCommonErrorCodes.h>

namespace P1
{
namespace ImageSdk
{

    class SdkException : public std::runtime_error
    {
    public:

        ErrorCode mCode;
        std::string mFilePath;
        int mLineNo;

        EXPORT_API SdkException(ErrorCode code, std::string message, std::string filePath = std::string(), int lineNo = -1);
    };

}
}

#endif //_P1_IMAGE_SDK_EXCEPTIONS_
