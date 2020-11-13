cmake_minimum_required(VERSION 3.7)

project(ogle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_definitions(-Wall -g -D_DEBUG)

include_directories(${CMAKE_SOURCE_DIR}/dependencies/rapidjson/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/angelscript/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/fmod/inc)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/imgui)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(GLM_ROOT_DIR ${CMAKE_SOURCE_DIR}/dependencies/glm CACHE PATH "GLM source directory")
find_package(GLM REQUIRED)
if(GLM_FOUND)
	include_directories(${GLM_INCLUDE_DIRS})
else()
	message(SEND_ERROR "Failed to find GLM.")
	return()
endif()

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/main.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/platform)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/system)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/sound)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/script)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/graphics)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/scene)

target_link_libraries(${PROJECT_NAME} sound)
target_link_libraries(${PROJECT_NAME} script)
target_link_libraries(${PROJECT_NAME} system)
target_link_libraries(${PROJECT_NAME} graphics)
target_link_libraries(${PROJECT_NAME} platform)
target_link_libraries(${PROJECT_NAME} scene)