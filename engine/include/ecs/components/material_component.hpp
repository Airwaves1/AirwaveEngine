#pragma once

#include "core/common.hpp"
#include "ecs/aw_component.hpp"
#include "rendering/opengl/gl_shader.hpp"
#include "rendering/opengl/gl_texture.hpp"
#include "resource/shader_lib.hpp"

namespace Airwave
{
class MaterialComponent : public AwComponent
{
  public:
    MaterialRenderParams materialRenderParams;

    glm::vec3 color = glm::vec3(0.0);

    std::shared_ptr<Shader> shader{nullptr};
    std::shared_ptr<Texture> diffuseMap{nullptr};

    MaterialComponent(MaterialType type = MaterialType::Basic) { setMaterialType(type); }

    void setMaterialType(MaterialType type)
    {
        if (m_type == type) return;

        switch (type)
        {
            case MaterialType::Basic:
                shader = SHADER_LIB.load("basic",
                                         PROJECT_ROOT_DIR "/assets/shaders/shader_lib/basic.vert",
                                         PROJECT_ROOT_DIR "/assets/shaders/shader_lib/basic.frag");
                break;
            case MaterialType::Phong:
                // setPhongMaterial();
                break;
            case MaterialType::PBR:
                // setPBRMaterial();
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
