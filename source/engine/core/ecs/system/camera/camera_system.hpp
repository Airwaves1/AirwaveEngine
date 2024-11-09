#pragma once

#include "ecs/system/aw_system.hpp"
#include "ecs/scene.hpp"
#include "utils/log.hpp"
#include "camera/camera.hpp"
#include "camera/controller/trackball_controller.hpp"
#include "ecs/component/camera/camera_component.hpp"
#include "ecs/component/camera/camera_controller_component.hpp"

namespace Airwave
{
class CameraSystem : public AwSystem
{
  public:
    CameraSystem() = default;

    virtual ~CameraSystem() override {}

    virtual void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) override
    {
        auto controller = scene->getRegistry()->view<FPSCameraControllerComponent>();
        for (auto entity : controller)
        {
            auto &controllerComp = scene->getRegistry()->get<FPSCameraControllerComponent>(entity);
            controllerComp.controller->update(deltaTime);
        }
    }

  private:
};

} // namespace Airwave
