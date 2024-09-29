cmake_minimum_required(VERSION 3.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../stb_image/stb_image)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../nlohmann_json/nlohmann_json)

add_library(tiny_obj "tiny_obj_loader.cpp")