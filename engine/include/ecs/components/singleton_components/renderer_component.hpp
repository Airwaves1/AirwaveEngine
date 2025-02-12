#pragma once

#include "ecs/aw_component.hpp"
#include "rendering/opengl/gl_texture.hpp"
#include "rendering/opengl/gl_shader.hpp"

#include "resource/resource_manager.hpp"
#include "resource/resource.hpp"
#include "resource/texture_resource.hpp"
#include "resource/shader_resource.hpp"

#include "rendering/opengl/gl_vertex_array.hpp"
#include "rendering/opengl/gl_vertex_buffer.hpp"

#include "utils/shapes_vao.hpp"

#include "ecs/components/mesh_component.hpp"

#include "rendering/primitive.hpp"

namespace Airwave
{
class RendererComponent : public AwComponent
{
  public:
    std::shared_ptr<ShaderResource> backgroundShader;
    std::shared_ptr<ShaderResource> basicShader;

    std::shared_ptr<TextureResource> emptyMap;
    std::shared_ptr<TextureResource> defaultNormal;

    std::shared_ptr<TextureResource> irradianceMap;
    std::shared_ptr<TextureResource> prefilterMap;
    std::shared_ptr<TextureResource> brdfLUT;

    std::shared_ptr<TextureResource> envMap;
    std::shared_ptr<TextureResource> backgroundMap;

    RendererComponent()
    {
        backgroundShader = RES.get_shared<ShaderResource>(SHADER_PATH + "shader_lib/background.glsl");
        basicShader      = RES.get_shared<ShaderResource>(SHADER_PATH + "shader_lib/basic.glsl");

        emptyMap      = RES.get_shared<TextureResource>("empty_texture");
        defaultNormal = RES.get_shared<TextureResource>("default_normal");

        cube = GeometryUtils::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);
    }

  private:
    std::shared_ptr<Primitive> cube;

    friend class RenderSystem;
};

} // namespace Airwave
