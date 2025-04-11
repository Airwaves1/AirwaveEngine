#pragma once

#include <glm/glm.hpp>
#include "function/ecs/system.hpp"
#include "function/ecs/world.hpp"
#include "function/ecs/component/transform_component.hpp"

namespace Airwave
{
class TransformSystem : public System
{
  public:
    TransformSystem()           = default;
    ~TransformSystem() override = default;

    virtual void onUpdate(std::weak_ptr<World> world, float delta_time) override
    {
        TransformComponent::updateWorldMatrix();
    }
};

} // namespace Airwave
