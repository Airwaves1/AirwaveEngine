#pragma once
#include <entt/entt.hpp>
#include "ecs/aw_system.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/aw_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/hierarchy_component.hpp"

namespace Airwave
{
class TransformSystem : public AwSystem
{
  public:
    TransformSystem()  = default;
    ~TransformSystem() = default;

    void onUpdate(float deltaTime) override
    {
        auto &registry = m_scene->getRegistry();
        auto view      = registry.view<TransformComponent>();

        for (auto entity : view)
        {
            auto &transform = view.get<TransformComponent>(entity);

            if (transform.m_dirty)
            {
                transform.m_localMatrix = transform.calculateTransformMatrix();
                transform.m_dirty       = false;
            }

            if (registry.all_of<HierarchyComponent>(entity))
            {
                auto &hierarchy = registry.get<HierarchyComponent>(entity);
                auto parent     = hierarchy.getParent();
                if(parent!= entt::null && registry.all_of<TransformComponent>(parent))
                {
                    auto &parentTransform = registry.get<TransformComponent>(parent);
                    transform.m_worldMatrix =
                        parentTransform.m_worldMatrix * transform.m_localMatrix;
                }
                else
                {
                    transform.m_worldMatrix = transform.m_localMatrix;
                }
            }
        }
    }


  private:
};
} // namespace Airwave
