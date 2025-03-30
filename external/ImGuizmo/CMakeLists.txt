cmake_minimum_required(VERSION 3.8)

project(ImGuizmo)

add_library(ImGuizmo STATIC
    "ImGuizmo.cpp"
    "ImGuizmo.h"
)

# # 包括ImGui头文件目录
target_include_directories(ImGuizmo PUBLIC ${PROJECT_SOURCE_DIR}/ImGuizmo)
# target_include_directories(ImGui PUBLIC ${PROJECT_SOURCE_DIR}/imgui/backends)