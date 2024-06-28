cmake_minimum_required(VERSION 3.22)
set(CORE_SOURCE_FILE
    "Application/Application.cpp"
    "Application/EventManager.cpp"
    "Application/ControllerEvent.cpp"

    "Render/RenderContext.cpp"

    "Camera/Camera.cpp"
    "Camera/OrthographicCamera.cpp"
    "Camera/PerspectiveCamera.cpp"

    "Controller/CameraController.cpp"
    "Controller/GameCameraController.cpp"
    "Controller/TrackBallCameraController.cpp"

    "Geometry/GeometryUtils.cpp"
    "Geometry/Geometry.cpp"

    "ECS/UUID.cpp"
    "ECS/Node.cpp"
    "ECS/Scene.cpp"
    "ECS/Entity.cpp"
)

add_library(Core ${CORE_SOURCE_FILE})

target_link_libraries(Core PUBLIC Platform)
