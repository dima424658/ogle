add_library(sound
${CMAKE_CURRENT_SOURCE_DIR}/device.cpp
${CMAKE_CURRENT_SOURCE_DIR}/sound2d.cpp
${CMAKE_CURRENT_SOURCE_DIR}/sound3d.cpp
${CMAKE_CURRENT_SOURCE_DIR}/soundlistener.cpp)

target_link_libraries(sound ${FMOD_LIBRARY})