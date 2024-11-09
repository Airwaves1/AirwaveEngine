#pragma once

#include "ecs/system/aw_system.hpp"
#include "ecs/component/transform/transform_component.hpp"
#include "ecs/scene.hpp"
#include <entt/entt.hpp>

namespace Airwave
{
class TransformSystem : public AwSystem
{
  public:
    TransformSystem()  = default;
    ~TransformSystem() = default;

    virtual void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) override
    {
        auto registry = scene->getRegistry();

        // 遍历所有变换组件
        auto view = registry->view<TransformComponent>();
        for (auto entity : view)
        {
            auto &transform = view.get<TransformComponent>(entity);

            // 如果有父实体，获取父实体的变换
            if (auto parent = transform.getParent())
            {
                // 更新当前实体的变换矩阵
                auto &parentTransform = parent->getComponent<TransformComponent>();

                glm::mat4 parentMatrix      = parentTransform.getTransformMatrix();
                transform.m_transformMatrix = parentMatrix * transform.calculateTransformMatrix();
            }
            else
            {
                // 如果没有父实体，直接使用自身的变换
                transform.m_transformMatrix = transform.calculateTransformMatrix();
            }

            // 清除脏标志
            transform.m_dirty = false;
        }
    }
};

} // namespace Airwave
