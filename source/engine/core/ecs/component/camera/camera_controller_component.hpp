#pragma once

#include "core/camera/controller/trackball_controller.hpp"
#include "core/camera/controller/fps_controller.hpp"
#include "core/camera/camera.hpp"

namespace Airwave
{

struct TrackballControllerComponent
{
    TrackballControllerComponent(std::shared_ptr<Camera> camera,
                                 const glm::vec3 &target = glm::vec3(0.0f, 0.0f, 0.0f))
        : controller(std::make_shared<TrackballController>(camera, target))
    {
    }

    std::shared_ptr<TrackballController> controller;
};

struct FPSCameraControllerComponent
{
    FPSCameraControllerComponent(std::shared_ptr<Camera> camera)
        : controller(std::make_shared<FPSCameraController>(camera))
    {
    }

    std::shared_ptr<FPSCameraController> controller;
};

} // namespace Airwave
