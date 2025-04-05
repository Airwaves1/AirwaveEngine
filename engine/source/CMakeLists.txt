cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE CORE_SOURCE_FILES core/*.cpp core/*.hpp)
file(GLOB_RECURSE FUNCTION_SOURCE_FILES function/*.cpp function/*.hpp)

add_library(engine STATIC
    engine.cpp
    ${CORE_SOURCE_FILES}
    ${FUNCTION_SOURCE_FILES}
)

target_link_libraries(engine PUBLIC platform)
add_subdirectory(platform)
