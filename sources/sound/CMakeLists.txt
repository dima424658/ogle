add_library(sound
${CMAKE_CURRENT_SOURCE_DIR}/device.cpp
${CMAKE_CURRENT_SOURCE_DIR}/sound2d.cpp
${CMAKE_CURRENT_SOURCE_DIR}/sound3d.cpp
${CMAKE_CURRENT_SOURCE_DIR}/soundlistener.cpp)

find_package(FMOD REQUIRED)
if(FMOD_FOUND)
	target_include_directories(sound PUBLIC ${FMOD_INCLUDE_DIR})
    target_link_libraries(sound ${FMOD_LIBRARIES})
else()
    message(SEND_ERROR "Failed to find FMOD.")
	return()
endif()