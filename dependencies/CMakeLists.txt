add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_link_libraries(glad ${CMAKE_DL_LIBS})

set(IMGUI_DIR {CMAKE_CURRENT_SOURCE_DIR}/imgui FILEPATH)

add_library(imgui
${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp)
