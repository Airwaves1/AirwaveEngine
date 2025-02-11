#pragma once

#include "core/common.hpp"
#include "ecs/aw_component.hpp"

#include "rendering/material.hpp"
#include "resource/resource_manager.hpp"
#include "resource/shader_resource.hpp"
#include "resource/texture_resource.hpp"

namespace Airwave
{
class MaterialComponent : public AwComponent
{
  public:
    MaterialRenderParams materialRenderParams;
    std::shared_ptr<Material> material;

    MaterialComponent(MaterialType type = MaterialType::Basic) { setMaterialType(type); }

    MaterialType getMaterialType() const { return m_type; }

    void setMaterialType(MaterialType type)
    {
        if (m_type == type) return;

        switch (type)
        {
            case MaterialType::Basic:
                material->shader = ResourceManager::GetInstance().get<ShaderResource>("basic");
                break;
            case MaterialType::PBR:
                material->shader  = ResourceManager::GetInstance().get<ShaderResource>("pbr");
                material->brdfLUT = ResourceManager::GetInstance().get<TextureResource>("brdf_lut");
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
