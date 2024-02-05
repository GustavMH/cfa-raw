#ifndef C_P1IMAGE_COMMON_LINKAGE_H
#define C_P1IMAGE_COMMON_LINKAGE_H

#ifndef EXPORT_API
#	if defined(_MSC_VER) // MSVC uses special DLL export attributes
#		ifdef P1MAKEDLL
#			define EXPORT_API __declspec(dllexport)
#		else // not exporting
#			if defined(_P1_SDK_STATIC)
#				define EXPORT_API 
#			else
#				define EXPORT_API __declspec(dllimport) // users import symbol from extern DLL
#			endif
#		endif
#	else // GCC + Clang
#		define EXPORT_API
#	endif
#endif // already defined


// MSVC compiler warns when DLLs expose STL types
// this will suppress these warnings
#ifndef MSVC_ALLOW_PUBLIC_STL
#   if defined(_MSC_VER)
#       define MSVC_ALLOW_PUBLIC_STL(X) _Pragma("warning (push)") \
    _Pragma("warning (disable:4251)") \
    X \
    _Pragma("warning (pop)")
#   else
#       define MSVC_ALLOW_PUBLIC_STL(X) X
#   endif
#endif

#endif // C_P1IMAGE_COMMON_LINKAGE_H
