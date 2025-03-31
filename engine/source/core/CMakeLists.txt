cmake_minimum_required(VERSION 3.10)
file(GLOB_RECURSE SOURCE_FILES *.cpp)
add_library(core STATIC ${SOURCE_FILES})