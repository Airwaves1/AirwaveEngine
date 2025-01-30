#pragma once

#include "ecs/aw_component.hpp"
#include "ecs/aw_entity.hpp"

namespace Airwave
{
class HierarchyComponent : public AwComponent
{

  public:
    AwEntity *getParent() const { return parent; }
    const std::vector<AwEntity *> &getChildren() const { return children; }

  private:
    AwEntity *parent{nullptr};
    std::vector<AwEntity *> children;

    friend class AwScene;
};

} // namespace Airwave
