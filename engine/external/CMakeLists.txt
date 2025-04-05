cmake_minimum_required(VERSION 3.10)

add_library(external INTERFACE)

add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
set(SPDLOG_FMT_EXTERNAL ON)

# glfw
add_subdirectory(glfw)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

# glad
add_subdirectory(glad)

# glm
option(GLM_ENABLE_EXPERIMENTAL ON)
option(GLM_ENABLE_CXX_20 "Enable C++ 20 features" ON)
add_subdirectory(glm)

# imgui
add_subdirectory(imgui)

# imguizmo
add_subdirectory(imguizmo)

# nlohmann_json
add_subdirectory(nlohmann_json)

# tiny_gltf
add_subdirectory(tiny_gltf)


target_link_libraries(external INTERFACE glfw)
target_link_libraries(external INTERFACE glad)
target_link_libraries(external INTERFACE glm)
target_link_libraries(external INTERFACE ImGui)
target_link_libraries(external INTERFACE ImGuizmo)
target_link_libraries(external INTERFACE nlohmann_json)
target_link_libraries(external INTERFACE tiny_gltf)
