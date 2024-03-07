cmake_minimum_required(VERSION 3.5)

project(AirwaveEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

# 打包assets资源文件，并拷贝到可执行文件的目录下
file(GLOB ASSETS "./assets")
file(COPY ${ASSETS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(PROJECT_SOURCES
        src/main.cpp
        src/Utils/logger.h
        src/Utils/logger.inl
        src/Utils/logger.cpp
        src/Utils/utils.h
        src/Application/mainwindow.h        src/Application/mainwindow.cpp
        src/Application/openglworker.h      src/Application/openglworker.cpp

        src/OpenGL/openglwidget.h src/OpenGL/openglwidget.cpp
        src/OpenGL/shader.h src/OpenGL/shader.cpp
        src/OpenGL/stb_image.h
        src/OpenGL/mytexture.h src/OpenGL/mytexture.cpp
        src/OpenGL/camera.h src/OpenGL/camera.cpp
        src/OpenGL/mesh.h src/OpenGL/mesh.cpp
        src/OpenGL/model.h src/OpenGL/model.cpp
)

# 包含头文件目录
include_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)
include_directories(
    SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 包含库文件目录
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AirwaveEngine
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/Application/editwidget.h src/Application/editwidget.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AirwaveEngine APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AirwaveEngine SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AirwaveEngine
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(AirwaveEngine PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(AirwaveEngine PRIVATE Qt6::OpenGLWidgets)
target_link_libraries(AirwaveEngine PRIVATE libassimp.dll.a)

# 将DLL复制到可执行文件目录
add_custom_command(
    TARGET AirwaveEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/bin/libassimp-5.dll"
        $<TARGET_FILE_DIR:AirwaveEngine>
    )


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AirwaveEngine)
endif()
set_target_properties(AirwaveEngine PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AirwaveEngine
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AirwaveEngine)
endif()
