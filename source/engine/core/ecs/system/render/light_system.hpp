#pragma once

#include "ecs/system/aw_system.hpp"
#include "ecs/component/material/material_component.hpp"
#include "ecs/component/light/light_component.hpp"

namespace Airwave
{
class LightSystem : public AwSystem
{

  public:
    LightSystem() = default;

    virtual ~LightSystem() = default;

    virtual void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) override
    {
        m_lights.clear();
        auto lightView = scene->getRegistry()->view<LightComponent>();
        for (auto entity : lightView)
        {
            auto &light = lightView.get<LightComponent>(entity);

            m_lights.push_back(light);
        }
    }

    const std::vector<LightComponent> &getLights() const { return m_lights; }

  private:
    std::vector<LightComponent> m_lights;
};

} // namespace Airwave
