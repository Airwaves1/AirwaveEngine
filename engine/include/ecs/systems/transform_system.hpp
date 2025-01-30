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
            auto awEntity = m_scene->getEntity(entity);
            if (!m_scene->isValideEntity(awEntity))
            {
                continue;
            }
            auto &transform = awEntity->getComponent<TransformComponent>();

            if (transform.m_dirty)
            {
                transform.m_localMatrix = transform.calculateTransformMatrix();
                transform.m_dirty       = false;
            }

            if (awEntity->hasComponent<HierarchyComponent>())
            {
                auto &hierarchy = awEntity->getComponent<HierarchyComponent>();
                auto parent     = hierarchy.getParent();
                if (parent && parent->hasComponent<TransformComponent>())
                {
                    auto &parentTransform = parent->getComponent<TransformComponent>();
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
