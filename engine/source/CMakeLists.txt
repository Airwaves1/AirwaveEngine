cmake_minimum_required(VERSION 3.10)

add_library(engine STATIC
    engine.cpp
)

target_link_libraries(engine PUBLIC core)

add_subdirectory(core)
add_subdirectory(function)
add_subdirectory(platform)
