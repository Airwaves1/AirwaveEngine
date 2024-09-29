cmake_minimum_required(VERSION 3.8)

project(AirwaveEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -Wall")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 设置库文件的输出目录（动态库和静态库）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置静态库的输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置不同构建类型的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/release)

if(WIN32)
    message("++ Platform: Windows")
    add_definitions(-DAW_PLATFORM_WINDOWS)
elseif(APPLE)
    message("++ Platform: MacOS")
    add_definitions(-DAW_PLATFORM_MACOS)
elseif(UNIX)
    message("++ Platform: Linux")
    add_definitions(-DAW_PLATFORM_LINUX)
else()
    message("++ Platform: Unsupported")
endif()

# 添加宏定义 
add_definitions(-DPROJECT_ROOT="${CMAKE_SOURCE_DIR}")
set(PROJECT_ROOT "${CMAKE_SOURCE_DIR}")
add_definitions(-DASSETS_DIR="${CMAKE_SOURCE_DIR}/install/assets/")
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/install/assets/")
add_definitions(-DCONFIG_DIR="${CMAKE_SOURCE_DIR}/install/config/")
set(CONFIG_DIR "${CMAKE_SOURCE_DIR}/install/config/")
add_definitions(-DCMAKE_DIR="${CMAKE_SOURCE_DIR}/install/cmake/")
set(CMAKE_DIR "${CMAKE_SOURCE_DIR}/install/cmake/")
add_definitions(-DSCRIPTS_DIR="${CMAKE_SOURCE_DIR}/install/scripts/")
set(SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/install/scripts/")

message("++ Project root: ${CMAKE_SOURCE_DIR}")
message("++ Assets directory: ${CMAKE_SOURCE_DIR}/install/assets/")
message("++ Config directory: ${CMAKE_SOURCE_DIR}/install/config/")
message("++ CMake directory: ${CMAKE_SOURCE_DIR}/install/cmake/")
message("++ Scripts directory: ${CMAKE_SOURCE_DIR}/install/scripts/")

# 添加项目头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/source/engine)
include_directories(${CMAKE_SOURCE_DIR}/source/engine/platform)
include_directories(${CMAKE_SOURCE_DIR}/source/engine/core)

# 添加第三方库头文件
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/external/glm)
include_directories(${CMAKE_SOURCE_DIR}/external/spdlog)
include_directories(${CMAKE_SOURCE_DIR}/external/stb_image)
include_directories(${CMAKE_SOURCE_DIR}/external/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)
include_directories(${CMAKE_SOURCE_DIR}/external/nlohmann_json)
include_directories(${CMAKE_SOURCE_DIR}/external/tiny_gltf)
include_directories(${CMAKE_SOURCE_DIR}/external/tiny_obj)

# 添加子目录
add_subdirectory(external)
add_subdirectory(source/engine)
add_subdirectory(source/sandbox)


#添加图标
set(APP_ICON "${CMAKE_SOURCE_DIR}/install/assets/pictures/airwave_icon.png")
