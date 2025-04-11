#pragma once

#include "function/ecs/system.hpp"
#include "function/ecs/world.hpp"
#include "function/ecs/component/first_person_controller.hpp"
#include "function/ecs/component/transform_component.hpp"
#include "function/ecs/component/camera_component.hpp"

namespace Airwave
{
class MovementSystem : public System
{
  public:
    MovementSystem()           = default;
    ~MovementSystem() override = default;

    void onUpdate(std::weak_ptr<World> world, float delta_time) override
    {
        auto w = world.lock();
        if (!w)
        {
            return;
        }

        const auto &registry = w->getRegistry();
        auto view            = registry.view<FirstPersonControllerComponent, TransformComponent>();

        for (auto entity : view)
        {
            auto &controller = view.get<FirstPersonControllerComponent>(entity);
            auto &transform  = view.get<TransformComponent>(entity);

        }
    }
};

} // namespace Airwave
