cmake_minimum_required(VERSION 3.7)

project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_definitions(-pthread -Wall -g -D_DEBUG -m64)

include_directories(${CMAKE_SOURCE_DIR}/dependencies/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/angelscript/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/imgui)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/main.cpp)

set(CMAKE_THREAD_PREFER_PTHREAD)
find_package(Threads REQUIRED)
if(Threads_FOUND)
    target_compile_options(${PROJECT_NAME} PUBLIC "-pthread")
    link_libraries("${CMAKE_THREAD_LIBS_INIT}")
else()
    message(SEND_ERROR "Failed to find PThreads.")   
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/system)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/sound)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/script)
add_subdirectory(${CMAKE_SOURCE_DIR}/sources/graphics)

target_link_libraries(${PROJECT_NAME} sound)
target_link_libraries(${PROJECT_NAME} script)
target_link_libraries(${PROJECT_NAME} system)
target_link_libraries(${PROJECT_NAME} graphics)