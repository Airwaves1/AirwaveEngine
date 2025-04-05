cmake_minimum_required(VERSION 3.10)
project(Sandbox)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.cpp" "*.hpp")

# 生成可执行文件
add_executable(sandbox ${SOURCE_FILES})

# 链接 engine 库
target_link_libraries(sandbox PUBLIC engine)