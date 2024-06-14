cmake_minimum_required (VERSION 3.8)

set(PROJECT_NAME "LearnOpenGL")
set(CMAKE_CXX_STANDARD 17)
project(${PROJECT_NAME}) 


#设置输出路径
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/bin)


add_definitions(-DASSETS_DEFINE_ROOT_DIR="${CMAKE_SOURCE_DIR}/assets/")


 
if(WIN32)
    message("++ Platform: Windows")
    add_definitions(-DPLATFORM_WIN32)
elseif(UNIX)
    if(APPLE)
        message("++ Platform: Macos")
        add_definitions(-DPLATFORM_MACOS)
    else()
        message("++ Platform: Linux")
        add_definitions(-DPLATFORM_LINUX)
    endif()
else()
    message("@@ Platform: Unsupported")
endif()

#预定义图形API
add_definitions(-DGRAPHICS_API_OPENGL) 
#add_definitions(-DGRAPHICS_API_VULKAN)



#添加第三方库头文件搜索路径
include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/glfw/include
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/glad/include
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/spdlog/include
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/glm
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/stb_image
)

#库文件搜索路径
link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/glfw/lib
	${CMAKE_CURRENT_SOURCE_DIR}/src/ThirdParty/glad/lib
)

#添加头文件搜索路径
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src/Platform
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core
)

add_subdirectory(src/ThirdParty)
add_subdirectory(src/Core)
add_subdirectory(src/Platform)
add_subdirectory(src/Sandbox)
