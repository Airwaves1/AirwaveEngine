cmake_minimum_required(VERSION 3.8)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE editor_source CONFIGURE_DEPENDS "src/editor/*.cpp")
