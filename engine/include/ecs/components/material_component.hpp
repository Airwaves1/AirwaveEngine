#pragma once

#include "core/common.hpp"
#include "ecs/aw_component.hpp"

#include "resource/resource_manager.hpp"

#include "resource/shader_resource.hpp"
#include "resource/texture_resource.hpp"

namespace Airwave
{
class MaterialComponent : public AwComponent
{
  public:
    MaterialRenderParams materialRenderParams;

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

    MaterialComponent(MaterialType type = MaterialType::Basic) { setMaterialType(type); }

    MaterialType getMaterialType() const { return m_type; }

    void setMaterialType(MaterialType type)
    {
        if (m_type == type) return;

        switch (type)
        {
            case MaterialType::Basic:
                shader = ResourceManager::GetInstance().get<ShaderResource>("basic");
                break;
            case MaterialType::PBR:
                shader  = ResourceManager::GetInstance().get<ShaderResource>("pbr");
                brdfLUT = ResourceManager::GetInstance().get<TextureResource>("brdf_lut");
                break;
            default:
                break;
        }

        m_type = type;
    }

  private:
    friend class RenderSystem;
    MaterialType m_type = MaterialType::None;
};
} // namespace Airwave
