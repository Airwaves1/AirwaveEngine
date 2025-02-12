#pragma once

#include <glad/glad.h>

#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

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

    std::shared_ptr<Shader> shader;

    std::shared_ptr<Texture> albedoMap;
    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> metallicMap;
    std::shared_ptr<Texture> roughnessMap;
    std::shared_ptr<Texture> metallicRoughnessMap;
    std::shared_ptr<Texture> aoMap;
    std::shared_ptr<Texture> emissiveMap;

    std::shared_ptr<Texture> irradianceMap;
    std::shared_ptr<Texture> prefilterMap;
    std::shared_ptr<Texture> brdfLUT;
};

} // namespace Airwave
