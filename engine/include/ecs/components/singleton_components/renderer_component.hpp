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
namespace Airwave
{
class RendererComponent : public AwComponent
{
  public:
    std::shared_ptr<TextureResource> emptyMap;
    std::shared_ptr<TextureResource> defaultNormal;

    std::shared_ptr<TextureResource> irradianceMap;
    std::shared_ptr<TextureResource> prefilterMap;
    std::shared_ptr<TextureResource> brdfLUT;

    std::shared_ptr<TextureResource> envMap;
    std::shared_ptr<TextureResource> backgroundMap;

    std::shared_ptr<ShaderResource> backgroundShader;

    RendererComponent()
    {
        backgroundShader = RES.get<ShaderResource>("background");
        emptyMap         = RES.get<TextureResource>("empty");
        defaultNormal    = RES.get<TextureResource>("defaultNormal");

        std::vector<AwVertex> cubeVertices;
        std::vector<uint32_t> cubeIndices;
        GeometryUtils::CreateCube(cubeVertices, cubeIndices, 1.0f, 1.0f, 1.0f, 1, 1, 1);
        meshComp = std::make_shared<MeshComponent>(cubeVertices, cubeIndices);
    }

  private:
    std::shared_ptr<MeshComponent> meshComp;

    friend class RenderSystem;
};

} // namespace Airwave
