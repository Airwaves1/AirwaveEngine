#pragma once

#include "ecs/aw_system.hpp"
#include "ecs/components/light_component.hpp"
#include "ecs/components/singleton_components/lights_manager_component.hpp"

namespace Airwave
{
class LightSystem : public AwSystem
{
  public:
    void onUpdate(float deltaTime) override
    {
        auto &reg        = m_scene->getRegistry();
        auto view        = reg.view<LightComponent, TransformComponent>();
        auto adminEntity = m_scene->getAdminEntity();

        auto &lightsManager = reg.get<LightsManagerComponent>(adminEntity);
        lightsManager.lights.clear();
        for (auto light_entity : view)
        {
            lightsManager.lights.push_back(light_entity);
        }
    }
};

} // namespace Airwave
