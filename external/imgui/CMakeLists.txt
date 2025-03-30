cmake_minimum_required(VERSION 3.8)

project(ImGui)

add_library(ImGui STATIC
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_widgets.cpp"
    "imgui_tables.cpp"
    "backends/imgui_impl_glfw.cpp"
    "backends/imgui_impl_opengl3.cpp"
)

# 包括ImGui头文件目录
target_include_directories(ImGui PUBLIC ${PROJECT_SOURCE_DIR}/imgui)
target_include_directories(ImGui PUBLIC ${PROJECT_SOURCE_DIR}/imgui/backends)