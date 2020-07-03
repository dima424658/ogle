add_library(graphics
${CMAKE_CURRENT_SOURCE_DIR}/device.cpp
${CMAKE_CURRENT_SOURCE_DIR}/deferred.cpp
${CMAKE_CURRENT_SOURCE_DIR}/mesh.cpp
${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
${CMAKE_CURRENT_SOURCE_DIR}/texture.cpp
${CMAKE_CURRENT_SOURCE_DIR}/material.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ui/editor.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ui.cpp
${CMAKE_CURRENT_SOURCE_DIR}/light.cpp
)

find_package(GLM REQUIRED)
if(GLM_FOUND)
	include_directories(${GLM_INCLUDE_DIRS})
else()
	message(SEND_ERROR "Failed to find GLM.")
	return()
endif()

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIR})
    target_link_libraries(graphics ${SDL2_LIBRARY})
else()
    message(SEND_ERROR "Failed to find SDL2.")
	return()
endif()

find_package(SDL2_image REQUIRED)
if(SDL2_IMAGE_FOUND)
	include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(graphics ${SDL2_IMAGE_LIBRARIES})
else()
    message(SEND_ERROR "Failed to find SDL2 image.")
	return()
endif()

target_link_libraries(graphics glad imgui)