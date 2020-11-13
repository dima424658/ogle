add_library(platform
${CMAKE_CURRENT_SOURCE_DIR}/info.cpp
${CMAKE_CURRENT_SOURCE_DIR}/linux.cpp)

find_package(GTK2 REQUIRED)
if(GTK2_FOUND)
	target_include_directories(platform PUBLIC ${GTK2_INCLUDE_DIRS})
    target_link_libraries(platform PUBLIC ${GTK2_LIBRARIES})
else()
    message(SEND_ERROR "Failed to find GTK2.")
	return()
endif()
