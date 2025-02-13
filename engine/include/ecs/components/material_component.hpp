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

    MaterialComponent(MaterialType type = MaterialType::PBR)
    {
        material = std::make_shared<Material>(type);
    }


  private:
    friend class RenderSystem;

};
} // namespace Airwave
