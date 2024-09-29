cmake_minimum_required(VERSION 3.8)

# 添加库
add_library(external INTERFACE)

target_compile_options(external INTERFACE -Wno-deprecated-declarations)

add_compile_options(-Wno-deprecated-declarations)

# glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(glfw)

# glad
add_subdirectory(glad)

# glm
add_subdirectory(glm)
option(GLM_ENABLE_CXX_20 "Enable C++ 20" ON)

# imgui
add_subdirectory(imgui)

# stb_image
add_subdirectory(stb_image)

# nlohmann_json
add_subdirectory(nlohmann_json)

# tiny_gltf
add_subdirectory(tiny_gltf)

#tiny_obj
add_subdirectory(tiny_obj)

# 查找 Vulkan
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "++ Vulkan found at ${Vulkan_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "++ Vulkan not found")
endif()

# 添加 Vulkan 头文件目录
target_include_directories(external INTERFACE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(external INTERFACE Vulkan::Vulkan)

# 链接 external
target_link_libraries(external INTERFACE external)


target_link_libraries(external INTERFACE glfw)
target_link_libraries(external INTERFACE glad)
target_link_libraries(external INTERFACE glm)
target_link_libraries(external INTERFACE ImGui)
target_link_libraries(external INTERFACE stb_image)
target_link_libraries(external INTERFACE nlohmann_json)
target_link_libraries(external INTERFACE tiny_gltf)
target_link_libraries(external INTERFACE tiny_obj)