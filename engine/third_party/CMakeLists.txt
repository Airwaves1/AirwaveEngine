cmake_minimum_required(VERSION 3.8)

add_library(third_party INTERFACE)

add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
set(SPDLOG_FMT_EXTERNAL ON)

# glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(glfw)

# glad
add_subdirectory(glad)

# glm
add_subdirectory(glm)
option(GLM_ENABLE_EXPERIMENTAL ON)
option(GLM_ENABLE_CXX_20 "Enable C++ 20 features" ON)


# imgui
add_subdirectory(imgui)

# nlohmann_json
add_subdirectory(nlohmann_json)

# tiny_gltf
add_subdirectory(tiny_gltf)

#tiny_obj
add_subdirectory(tiny_obj)

# bullet
add_subdirectory(bullet3)
option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_UNIT_TESTS OFF)
option(BUILD_BULLET3 OFF)
option(BUILD_PYBULLET OFF)
option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)


target_link_libraries(third_party INTERFACE glfw)
target_link_libraries(third_party INTERFACE glad)
target_link_libraries(third_party INTERFACE glm)
target_link_libraries(third_party INTERFACE ImGui)
target_link_libraries(third_party INTERFACE nlohmann_json)
target_link_libraries(third_party INTERFACE tiny_gltf)
target_link_libraries(third_party INTERFACE tiny_obj)
target_link_libraries(third_party INTERFACE BulletDynamics)