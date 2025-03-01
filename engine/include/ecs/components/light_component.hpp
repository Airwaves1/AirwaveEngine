#pragma once

#include "ecs/aw_component.hpp"
#include "core/common.hpp"
#include <glm/glm.hpp>
#include "rendering/framebuffer.hpp"

namespace Airwave
{
class LightComponent : public AwComponent
{
  public:
    LightComponent()  = default;
    ~LightComponent() = default;

    float intensity = 1.0f;
    glm::vec3 color = glm::vec3(1.0f);

    LightType type = LightType::Point;

    std::shared_ptr<Framebuffer> depth_framebuffer;
    glm::vec2 shadowMapSize = glm::vec2(1024.0f, 1024.0f);

    bool castShadow = true;
    float shadowBias = 0.005f;
    float shadowRadius = 1.0f;
    float shadowStrength = 0.5f;
    float lightSize = 0.3f;

};
} // namespace Airwave
