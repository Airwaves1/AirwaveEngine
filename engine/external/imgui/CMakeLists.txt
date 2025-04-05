cmake_minimum_required(VERSION 3.10)

project(ImGui)

# 添加 ImGui 源文件
add_library(ImGui STATIC
    "imgui.cpp"
    "imgui_demo.cpp"
    "imgui_draw.cpp"
    "imgui_widgets.cpp"
    "imgui_tables.cpp"
    "backends/imgui_impl_glfw.cpp"
    "backends/imgui_impl_opengl3.cpp"
)

# 包括 ImGui 头文件目录
target_include_directories(ImGui PUBLIC ${PROJECT_SOURCE_DIR}/imgui)
target_include_directories(ImGui PUBLIC ${PROJECT_SOURCE_DIR}/imgui/backends)

# 禁用所有编译警告
if(MSVC)
    target_compile_options(ImGui PRIVATE /w)  # 对于 MSVC 禁用所有警告
elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(ImGui PRIVATE -w)  # 对于 GCC 和 Clang 禁用所有警告
endif()
