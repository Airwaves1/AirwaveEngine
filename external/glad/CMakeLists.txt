cmake_minimum_required(VERSION 3.8)


add_library(glad "src/glad.c")

target_include_directories(glad PUBLIC "include")

target_link_libraries(glad PUBLIC opengl32)

