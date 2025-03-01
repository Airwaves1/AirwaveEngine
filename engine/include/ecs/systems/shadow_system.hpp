#pragma once

#include "ecs/aw_scene.hpp"
#include "ecs/aw_system.hpp"

#include "ecs/components/camera_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/singleton_components/renderer_component.hpp"
#include "ecs/components/singleton_components/lights_manager_component.hpp"
#include "ecs/components/light_component.hpp"

namespace Airwave
{
class ShadowSystem : public AwSystem
{
  public:
    ShadowSystem()  = default;
    ~ShadowSystem() = default;

  protected:
    void onUpdate(float deltaTime) override
    {
        auto &reg = m_scene->getRegistry();
        auto admin_entity = m_scene->getAdminEntity();
        auto &renderer_comp = reg.get<RendererComponent>(admin_entity);
        auto &lightsManager_comp = reg.get<LightsManagerComponent>(admin_entity);

        auto view = reg.view<LightComponent, TransformComponent, CameraComponent>();

        for(auto entity : view)
        {
            auto &light = view.get<LightComponent>(entity);
            auto &transform = view.get<TransformComponent>(entity);
            auto &camera = view.get<CameraComponent>(entity);

            if(light.type == LightType::Directional)
            {
                // 生成阴影贴图
                renderer_comp.shadowMap->bind();
                glClear(GL_DEPTH_BUFFER_BIT);
                renderScene(camera);
                renderer_comp.shadowMap->unbind();
            }
        }
    }
};

} // namespace Airwave
