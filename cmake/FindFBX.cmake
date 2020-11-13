# FBX_ROOT
# FBX_FOUND
# FBX_LIBRARY
# FBX_STATIC_LIBRARY
# FBX_INCLUDE_DIR

set(_FBX_SEARCH_DIRS
"${FBX_ROOT}/include"
"${CMAKE_SOURCE_DIR}/dependencies/fbxsdk/include"
"C:/Program Files (x86)/Autodesk/FBX/FBX SDK/*/include"
"C:/Program Files/Autodesk/FBX/FBX SDK/*/include")

find_path(FBX_INCLUDE_DIR "fbxsdk.h"
		  PATHS ${_FBX_SEARCH_DIRS})

if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "x86_64" OR "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "AMD64")
    set(_FBX_LIBRARY_SEARCH_DIRS
    "${FBX_ROOT}/lib/*/x64/release"
    "${CMAKE_SOURCE_DIR}/dependencies/fbxsdk/lib/*/x64/release"
    "C:/Program Files (x86)/Autodesk/FBX/FBX SDK/*/lib/*/x64/release"
    "C:/Program Files/Autodesk/FBX/FBX SDK/*/lib/*/x64/release")
elseif()
    set(_FBX_LIBRARY_SEARCH_DIRS
    "${FBX_ROOT}/lib/*/x86/release"
    "${CMAKE_SOURCE_DIR}/dependencies/fbxsdk/lib/*/x86/release"
    "C:/Program Files (x86)/Autodesk/FBX/FBX SDK/*/lib/*/x86/release"
    "C:/Program Files/Autodesk/FBX/FBX SDK/*/lib/*/x86/release")
endif()

find_path(FBX_STATIC_LIBRARY "libfbxsdk.a"
		  PATHS ${_FBX_LIBRARY_SEARCH_DIRS})

find_library(FBX_LIBRARY NAMES fbxsdk
PATHS ${_FBX_LIBRARY_SEARCH_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FBX REQUIRED_VARS FBX_LIBRARY FBX_INCLUDE_DIR)