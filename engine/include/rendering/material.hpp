#pragma once

#include <glad/glad.h>
#include "resource/texture_resource.hpp"
#include "resource/shader_resource.hpp"

namespace Airwave
{
class Material
{
  public:
    float roughness = 1.0f;
    float metallic  = 1.0f;
    float ao        = 1.0f;

    glm::vec3 color = glm::vec3(0.0);

    std::shared_ptr<ShaderResource> shader;

    std::shared_ptr<TextureResource> albedoMap;
    std::shared_ptr<TextureResource> normalMap;
    std::shared_ptr<TextureResource> metallicMap;
    std::shared_ptr<TextureResource> roughnessMap;
    std::shared_ptr<TextureResource> aoMap;

    std::shared_ptr<TextureResource> irradianceMap;
    std::shared_ptr<TextureResource> prefilterMap;
    std::shared_ptr<TextureResource> brdfLUT;
};

} // namespace Airwave
