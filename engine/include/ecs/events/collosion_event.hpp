#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "ecs/aw_component.hpp"

namespace Airwave
{
class CollisionEvent : public AwComponent
{
  public:
    entt::entity entity1;
    entt::entity entity2;

    glm::vec3 contactPoint;  // 碰撞点
    glm::vec3 contactNormal; // 碰撞法线

    float impulse; // 冲量

    CollisionEvent(entt::entity entity1, entt::entity entity2, glm::vec3 contactPoint, glm::vec3 contactNormal, float impulse)
        : entity1(entity1), entity2(entity2), contactPoint(contactPoint), contactNormal(contactNormal), impulse(impulse)
    {
    }
};

} // namespace Airwave
