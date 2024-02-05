#ifndef _P1_IMAGE_SDK_CLARITY_H_
#define _P1_IMAGE_SDK_CLARITY_H_

#include <cstdint>
#include <memory>
#include <string>

#include "C_P1ImageCommonLinkage.h"

namespace P1
{
namespace ImageSdk
{
    struct EXPORT_API Clarity
    {
        int method; //IC_ClarityMethod
        float factor;
        
        Clarity();
        Clarity(int method, float factor);
    };
} //namespace ImageSdk
} //namespace P1


#endif //_P1_IMAGE_SDK_CLARITY_H_
