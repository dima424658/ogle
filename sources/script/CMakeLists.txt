add_library(script
${CMAKE_CURRENT_SOURCE_DIR}/device.cpp
${CMAKE_CURRENT_SOURCE_DIR}/reference.cpp
${CMAKE_CURRENT_SOURCE_DIR}/script.cpp
${CMAKE_CURRENT_SOURCE_DIR}/types/functions.cpp
${CMAKE_CURRENT_SOURCE_DIR}/types/scriptstdstring.cpp
${CMAKE_CURRENT_SOURCE_DIR}/types/glm_as.cpp)

target_link_libraries(script ${CMAKE_SOURCE_DIR}/dependencies/angelscript/lib/libangelscript.a)