#ifndef _P1_IMAGESDK_VERSION_HPP_
#define _P1_IMAGESDK_VERSION_HPP_

#include <string>

#include "C_P1ImageCommonTypes.h"
#include "C_P1ImageCommonLinkage.h"


namespace P1
{
namespace ImageSdk
{

    EXPORT_API Version GetSdkVersion();

    EXPORT_API std::string GetSdkBuildDate();

    EXPORT_API std::string GetSdkBuildHash();

    EXPORT_API std::string GetSdkBuildCompiler();
}
}


#endif //_P1_IMAGESDK_VERSION_HPP_