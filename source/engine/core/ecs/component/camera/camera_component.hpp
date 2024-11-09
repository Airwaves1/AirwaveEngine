#pragma once

#include <memory>
#include "core/camera/camera.hpp"

namespace Airwave
{
struct CameraComponent
{
    std::shared_ptr<Camera> camera;

    CameraComponent() = default;

    CameraComponent(CameraType type, float fov, float aspectRatio, float nearPlane, float farPlane)
        : camera(std::make_shared<Camera>(type, fov, aspectRatio, nearPlane, farPlane))
    {
    }
};
} // namespace Airwave
