#pragma once

#include "ecs/aw_component.hpp"
#include "ecs/aw_entity.hpp"
#include <entt/entt.hpp>

namespace Airwave
{
class HierarchyComponent : public AwComponent
{

  public:
    entt::entity getParent() const { return parent; }
    const std::vector<entt::entity> &getChildren() const { return children; }

  private:
    entt::entity parent;
    std::vector<entt::entity> children;

    friend class AwScene;
};

} // namespace Airwave
