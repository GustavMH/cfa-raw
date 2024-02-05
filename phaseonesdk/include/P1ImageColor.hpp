#ifndef _P1_IMAGE_SDK_COLOR_H_
#define _P1_IMAGE_SDK_COLOR_H_

#include <cstdint>
#include <memory>
#include <string>

#include "C_P1ImageCommonLinkage.h"

namespace P1
{
namespace ImageSdk
{
    union EXPORT_API Color
    {
        float vector[4];
        struct {
            float red;
            float green;
            float blue;
            float alpha;
        };
        
        Color();
        Color(float r, float g, float b, float a = 1.0f);
        
    };
} //namespace ImageSdk
} //namespace P1


#endif //_P1_IMAGE_SDK_COLOR_H_
