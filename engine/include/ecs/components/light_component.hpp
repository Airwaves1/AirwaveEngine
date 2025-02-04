#pragma once

#include "ecs/aw_component.hpp"
#include "core/common.hpp"
#include <glm/glm.hpp>

namespace Airwave
{
class LightComponent : public AwComponent
{
  public:
    LightComponent()  = default;
    ~LightComponent() = default;

    float intensity = 1.0f;
    glm::vec3 color = glm::vec3(1.0f);

    LightType type = LightType::Directional;
};
} // namespace Airwave
