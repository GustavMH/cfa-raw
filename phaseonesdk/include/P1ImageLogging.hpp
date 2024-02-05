#ifndef _P1_IMAGESDK_LOGGING_
#define _P1_IMAGESDK_LOGGING_

#include <functional>

#include "C_P1ImageCommonLinkage.h"

namespace P1
{
namespace ImageSdk
{
    enum class LogMessageLevel
    {
        Error,
        Warning,
        Info,
        Verbose
    };

    EXPORT_API void SetSdkLoggingHandler(std::function<void(const char*, LogMessageLevel)> logHandler);
    EXPORT_API void ClearSdkLoggingHandler();


    
}
}

#endif // _P1_IMAGESDK_LOGGING_