#pragma once

#include "core/common.hpp"
#include "ecs/aw_component.hpp"

#include "rendering/material.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

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

    MaterialComponent(MaterialType type = MaterialType::Basic)
    {
        material = std::make_shared<Material>();
        setMaterialType(type);
    }

    MaterialType getMaterialType() const { return m_type; }

    void setMaterialType(MaterialType type)
    {
        if (m_type == type) return;

        switch (type)
        {
            case MaterialType::Basic:
                material->shader = RES.load<ShaderResource>("shader/shader_lib/basic.glsl")->getShader();
                break;
            case MaterialType::PBR:
                material->shader  = RES.load<ShaderResource>("shader/shader_lib/pbr.glsl")->getShader();
                material->brdfLUT = RES.load<TextureResource>("brdf_lut")->getTexture();
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
