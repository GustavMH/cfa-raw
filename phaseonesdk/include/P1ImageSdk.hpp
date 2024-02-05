#ifndef _P1_IMAGESDK_SDK_
#define _P1_IMAGESDK_SDK_

#include "P1ImageLogging.hpp"
#include "P1ImageExceptions.hpp"

#include <C_P1ImageCommonLinkage.h>

namespace P1
{
namespace ImageSdk
{

    /**
    * @brief Throttle the CPU resources consumed by image conversion and decoding
    * 
    * This sets the number of thread the internal thread worker pool can utilize.
    * To have effect, this method must be called **before any** interaction with the SDK
    * 
    * By default the amount of thread is determined by the available CPU cores on the system.
    * 
    * @param numOfThreadPoolThreads The number of thread allowed in the internal thread pool
    */
    EXPORT_API void SetThreadPoolThreadCount(uint32_t numOfThreadPoolThreads);

    /**

    * @brief Set size of the memory used for processing images
    *
    * By default the system default memory size is used.
    *
    * @param maxMemorySizeInBytes The size of the memory used for image processing
    */
    EXPORT_API void SetProcessingMemoryUsage(uint64_t maxMemorySizeInBytes);
    
    /**

    * @brief Set the default location where the SDK looks for the SensorProfile files
    *
    * The default search location is the ./SensorProfiles, relative to the
    * current working directory.
    *
    * To have effect, this method must be called **before any** interaction with
    * the SDK.
    *
    * @param pathToProfiles The path (absolute or relative) to the folder
    * containing the profiles
    */
    EXPORT_API void SetSensorProfilesLocation(std::string pathToProfiles);

    /**
     * @brief Force initialization and allocation of internal resources (memory & threads)
     * 
     * This function is <em>optional</em>! You do not need to call this. It is called automatically
     * when you process the first image.
     * 
     * Use this function to prepare the the SDK and avoid the costly initialization when you need to 
     * process an image. You can also use this method to allocate the needed SDK resources (like memory)
     * up front.
     */
    EXPORT_API void Initialize();



    /**
     * @brief Force disposal of the internal resources (memory)
     * The intention of this function is to let the application easy the memory pressure while ImageSDK is not in use.
     * Once the ImageSDK is activated again, it will reallocate the internal resources.
     * This function must not be called while the ImageSDK is active!
     */
    EXPORT_API void Dispose();


}
}

#endif // _P1_IMAGESDK_SDK_
