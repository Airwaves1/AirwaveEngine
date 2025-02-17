#pragma once
#include "ecs/aw_component.hpp"
#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace Airwave
{
class TriggerEvent : public AwComponent
{
  public:
    entt::entity entity1;
    entt::entity entity2;
    glm::vec3 contactPoint;
    glm::vec3 contactNormal;

    TriggerEvent(entt::entity entity1, entt::entity entity2, glm::vec3 contactPoint, glm::vec3 contactNormal)
        : entity1(entity1), entity2(entity2), contactPoint(contactPoint), contactNormal(contactNormal)
    {
    }
};

} // namespace Airwave
