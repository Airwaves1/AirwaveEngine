cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE engine_source CONFIGURE_DEPENDS "src/*.cpp")

add_subdirectory(third_party)

add_library(engine STATIC ${engine_source})
target_link_libraries(engine PUBLIC third_party)