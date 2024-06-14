cmake_minimum_required(VERSION 3.22)
set (CORE_SOURCE_FILE
    "Utils/Log.cpp"

    "Application/Application.cpp"
    "Application/EventManager.cpp"

    "Render/RenderContext.cpp"


    "Camera/Camera.cpp"
    "Camera/OrthographicCamera.cpp"
    "Camera/PerspectiveCamera.cpp"

    "Controller/CameraController.cpp"
    "Controller/GameCameraController.cpp"
    "Controller/TrackBallCameraController.cpp"

)


add_library(Core ${CORE_SOURCE_FILE})

target_link_libraries(Core PUBLIC Platform)