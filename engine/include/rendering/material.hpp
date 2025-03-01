#pragma once

#include <glad/glad.h>

#include "utils/shader_utils.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

#include "resource/resource_manager.hpp"
#include "resource/texture_resource.hpp"
#include "resource/shader_resource.hpp"

#include "core/common.hpp"
namespace Airwave                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
{
class Material
{
  public:
    ShaderKeyWord computeShaderKeyWord()
    {
        ShaderKeyWord key = ShaderKeyWord::NONE;

        if (m_type == MaterialType::PBR) key = key | ShaderKeyWord::USE_PBR;
        if (albedoMap) key = key | ShaderKeyWord::USE_ALBEDO_MAP;
        if (normalMap) key = key | ShaderKeyWord::USE_NORMAL_MAP;
        if (metallicRoughnessMap) key = key | ShaderKeyWord::USE_METALLIC_ROUGHNESS_MAP;
        if (aoMap) key = key | ShaderKeyWord::USE_AO_MAP;
        if (emissiveMap) key = key | ShaderKeyWord::USE_EMISSIVE_MAP;
        if (irradianceMap && prefilterMap && brdfLUT) key = key | ShaderKeyWord::USE_IBL;
        if (hasTanget) key = key | ShaderKeyWord::USE_TANGENT;
        
        if (key != m_shaderKeyWord)
        {
            m_shaderKeyWord = key;
            shader          = ShaderUtils::GetInstance().get_new_variant_shader(shader, key);
        }

        return key;
    }

    bool hasTanget = false;

    float roughness = 1.0f;
    float metallic  = 1.0f;
    float ao        = 1.0f;

    glm::vec3 color    = glm::vec3(0.0);
    glm::vec3 emissive = glm::vec3(0.0);

    std::shared_ptr<Shader> shader;

    std::shared_ptr<Texture> albedoMap;
    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> metallicMap;
    std::shared_ptr<Texture> roughnessMap;
    std::shared_ptr<Texture> metallicRoughnessMap;
    std::shared_ptr<Texture> aoMap;
    std::shared_ptr<Texture> emissiveMap;

    glm::vec2 uvScale = glm::vec2(1.0f, 1.0f);

    std::shared_ptr<Texture> irradianceMap;
    std::shared_ptr<Texture> prefilterMap;
    std::shared_ptr<Texture> brdfLUT;

    Material(MaterialType type = MaterialType::PBR) { setMaterialType(type); }

    MaterialType getMaterialType() const { return m_type; }

    void setMaterialType(MaterialType type)
    {
        if (m_type == type) return;

        switch (type)
        {
            case MaterialType::Basic:
                shader = RES.load<ShaderResource>("shaders/shader_lib/basic.glsl")->getShader();
                break;
            case MaterialType::PBR:
                shader  = RES.load<ShaderResource>("shaders/shader_lib/pbr.glsl")->getShader();
                brdfLUT = RES.load<TextureResource>("brdf_lut")->getTexture();
                break;
            default:
                break;
        }

        m_type = type;
    }

  private:
    MaterialType m_type           = MaterialType::None;
    ShaderKeyWord m_shaderKeyWord = ShaderKeyWord::NONE;
};

} // namespace Airwave
