cmake_minimum_required(VERSION 3.8)

add_library(engine INTERFACE)

target_link_libraries(engine INTERFACE core)

add_subdirectory(core)
add_subdirectory(platform)