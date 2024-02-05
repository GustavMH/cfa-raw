#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ImageSDK::ImageSdkCpp" for configuration "Release"
set_property(TARGET ImageSDK::ImageSdkCpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(ImageSDK::ImageSdkCpp PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libImageSdkCpp.so.3.0.19"
  IMPORTED_SONAME_RELEASE "libImageSdkCpp.so.3.0.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS ImageSDK::ImageSdkCpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_ImageSDK::ImageSdkCpp "${_IMPORT_PREFIX}/lib/libImageSdkCpp.so.3.0.19" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
