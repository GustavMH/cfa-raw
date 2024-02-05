#ifndef _P1_IMAGE_SDK_RECT_H_
#define _P1_IMAGE_SDK_RECT_H_

#include <cstdint>
#include <memory>
#include <string>

#include "C_P1ImageCommonLinkage.h"

namespace P1
{
namespace ImageSdk
{
/**
 * @brief A point on any 2D Cartesian coordinate system defined by its x and y axis values
 */
struct EXPORT_API Point
{
    /** @brief The horizontal distance from the origin of that coordinate system */
    int32_t x;
    
    /** @brief The vertical distance from the origin of that coordinate system */
    int32_t y;
    
    std::string ToString() const;
};

/**
 * @brief A size on any Cartesian coordinate system defined by its width and height
 */
struct EXPORT_API Size
{
    /** @brief The horizontal length */
    int32_t width;
    
    /** @brief The vertical length */
    int32_t height;
    
    std::string ToString() const;
};

/**
 * @brief A rectangular area defined on any 2D Cartesian coordinate system defined by its top left corner and size.
 */
struct EXPORT_API Rect : public Point, public Size
{
    Rect();
    Rect(int32_t x, int32_t y, int32_t w, int32_t h);
    
    std::string ToString() const;
};

}
}


#endif //_P1_IMAGE_SDK_RECT_H_
