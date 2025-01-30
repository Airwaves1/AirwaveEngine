cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE editor_source CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(editor STATIC ${editor_source})
