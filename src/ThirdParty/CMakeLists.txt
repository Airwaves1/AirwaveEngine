cmake_minimum_required (VERSION 3.8)

add_library(ThirdParty glad/src/glad.c)

#glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory(glfw)

target_link_libraries(ThirdParty glfw)
