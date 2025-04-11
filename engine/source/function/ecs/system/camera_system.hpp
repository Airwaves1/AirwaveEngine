#pragma once

#include "function/ecs/system.hpp"
#include "function/ecs/world.hpp"
#include "function/ecs/component/camera_component.hpp"
#include "function/ecs/component/transform_component.hpp"

namespace Airwave
{
class CameraSystem : public System
{
  public:
    ~CameraSystem() override = default;

    void onUpdate(std::weak_ptr<World> world, float delta_time) override
    {
        auto w = world.lock();
        if (!w)
        {
            return;
        }

        const auto &registry = w->getRegistry();
        auto view            = registry.view<CameraComponent, TransformComponent>();

        for (auto entity : view)
        {
            auto &camera    = view.get<CameraComponent>(entity);
            auto &transform = view.get<TransformComponent>(entity);

            camera.viewMatrix = transform.getWorldMatrixInverse();
        }
    }
};

} // namespace Airwave
