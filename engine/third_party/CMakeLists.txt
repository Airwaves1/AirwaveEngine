cmake_minimum_required(VERSION 3.8)

add_library(external INTERFACE)

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

# stb_image
add_subdirectory(stb_image)

# imgui
add_subdirectory(imgui)

# nlohmann_json
add_subdirectory(nlohmann_json)

# tiny_gltf
add_subdirectory(tiny_gltf)

#tiny_obj
add_subdirectory(tiny_obj)

target_link_libraries(external INTERFACE glfw)
target_link_libraries(external INTERFACE glad)
target_link_libraries(external INTERFACE glm)
target_link_libraries(external INTERFACE ImGui)
target_link_libraries(external INTERFACE stb_image)
target_link_libraries(external INTERFACE nlohmann_json)
target_link_libraries(external INTERFACE tiny_gltf)
target_link_libraries(external INTERFACE tiny_obj)