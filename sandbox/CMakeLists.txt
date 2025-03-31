cmake_minimum_required(VERSION 3.10)
project(Sandbox)

#递归遍历当前文件夹下的所有子文件夹获取所有的源文件
file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(sandbox ${SOURCE_FILES})

target_link_libraries(sandbox PUBLIC engine)