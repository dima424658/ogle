# FMOD_ROOT
# FMOD_FOUND
# FMOD_LIBRARIES
# FMOD_INCLUDE_DIR

set(_FMOD_INCLUDE_SEARCH_DIRS
"${FMOD_ROOT}/api/core"
"${FMOD_ROOT}"
"${CMAKE_SOURCE_DIR}/dependencies/fmod"
"C:/Program Files (x86)/FMOD Studio API Windows/api/core"
"C:/Program Files/FMOD Studio API Windows/api/core")

find_path(FMOD_INCLUDE_DIR "fmod.hpp"
		  PATH_SUFFIXES include inc
		  PATHS ${_FMOD_INCLUDE_SEARCH_DIRS})


if("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "x86_64" OR "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "AMD64")
	set(_FMOD_LIBRARIES_SEARCH_DIRS
	"${FMOD_ROOT}/api/core/lib/x86_64"
	"${FMOD_ROOT}/lib/x86_64"
	"${FMOD_ROOT}/api/core/lib/x64"
	"${FMOD_ROOT}/lib/x64"
	"${CMAKE_SOURCE_DIR}/dependencies/fmod/lib/x64"
	"${CMAKE_SOURCE_DIR}/dependencies/fmod/lib/x86_64"
	"C:/Program Files (x86)/FMOD Studio API Windows/api/lib/x64"
	"C:/Program Files/FMOD Studio API Windows/api/core/lib/x86_64"
	"C:/Program Files (x86)/FMOD Studio API Windows/api/lib/x64"
	"C:/Program Files/FMOD Studio API Windows/api/core/lib/x86_64")
elseif()
	set(_FMOD_LIBRARIES_SEARCH_DIRS
	"${FMOD_ROOT}/api/core/lib/x86"
	"${FMOD_ROOT}/lib/x86"
	"${CMAKE_SOURCE_DIR}/dependencies/fmod/lib/x86"
	"C:/Program Files/FMOD Studio API Windows/api/core/lib/x86"
	"C:/Program Files/FMOD Studio API Windows/api/core/lib/x86")
endif()

find_library(_FMOD_LIBRARY_TEMP NAMES fmod NAMES_PER_DIR PATHS ${_FMOD_LIBRARIES_SEARCH_DIRS})
find_library(_FMODL_LIBRARY_TEMP NAMES fmodL NAMES_PER_DIR PATHS ${_FMOD_LIBRARIES_SEARCH_DIRS})
set(FMOD_LIBRARIES ${_FMOD_LIBRARY_TEMP} ${_FMODL_LIBRARY_TEMP} CACHE FILEPATH "Path to fmod libraries")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FMOD REQUIRED_VARS FMOD_LIBRARIES FMOD_INCLUDE_DIR)