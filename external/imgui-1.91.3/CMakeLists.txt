cmake_minimum_required(VERSION 3.5)
project(imgui)

set(IMGUI_SRC
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
    ${EXTERNAL_DIR}/glfw/include
    ${EXTERNAL_DIR}/glad/include
)

target_link_libraries(imgui PRIVATE glfw glad)