#pragma once

#include <vector>
#include "ecs/aw_component.hpp"
#include "ecs/components/light_component.hpp"
#include "ecs/aw_entity.hpp"

namespace Airwave
{
class LightsManagerComponent : public AwComponent
{
  public:
    std::vector<AwEntity*> lights;
};

} // namespace Airwave
