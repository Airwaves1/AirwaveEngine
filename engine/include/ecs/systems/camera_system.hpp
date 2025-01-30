#pragma once

#include "ecs/aw_system.hpp"
#include "ecs/components/camera_component.hpp"

namespace Airwave
{
class CameraSystem : public AwSystem
{
  public:
    CameraSystem()  = default;
    ~CameraSystem() = default;

  protected:
    void onUpdate(float deltaTime) override
    {
        auto &reg = m_scene->getRegistry();
        reg.view<CameraComponent, TransformComponent>().each(
            [](auto &camera, auto &transform)
            {
                if (camera.enable)
                {
                    camera.m_worldInverseMatrix = glm::inverse(transform.getWorldMatrix());
                    camera.update();
                }
            });
    }
};
} // namespace Airwave