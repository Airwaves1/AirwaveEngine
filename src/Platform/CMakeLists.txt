cmake_minimum_required(VERSION 3.22)
set(PLATFORM_SOURCE_FILE

    "Utils/Log.cpp"

    "Windows/Window.cpp"
    "Windows/GLFWWindow.cpp"

    "GraphicContext.cpp"
    "OpenGL/OpenGLCommon.cpp"
    "OpenGL/OpenGLContext.cpp"
    "OpenGL/OpenGLShader.cpp"
    "OpenGL/OpenGLTexture.cpp"
)

add_library(Platform ${PLATFORM_SOURCE_FILE})

# 添加链接库
target_link_libraries(Platform ThirdParty)
