#pragma once
#include "core/common.hpp"
#include "core/log.hpp"
#include "core/application.hpp"
#include "ecs/aw_scene.hpp"
#include "ecs/aw_system.hpp"
#include "ecs/components/singleton_components/renderer_component.hpp"
#include "ecs/components/camera_component.hpp"
#include "rendering/renderer.hpp"
#include "rendering/postprocess_effect.hpp"

namespace Airwave
{
class PostProcessSystem : public AwSystem
{
  public:
    PostProcessSystem()  = default;
    ~PostProcessSystem() = default;

  protected:
    void onUpdate(float deltaTime) override
    {
        auto renderer       = m_scene->getApplication()->getRenderer();
        auto &reg           = m_scene->getRegistry();
        auto admin_entity   = m_scene->getAdminEntity();
        auto &renderer_comp = reg.get<RendererComponent>(admin_entity);
    }
};

} // namespace Airwave
