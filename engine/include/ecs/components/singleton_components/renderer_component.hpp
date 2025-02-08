#pragma once

#include "ecs/aw_component.hpp"
#include "rendering/opengl/gl_texture.hpp"
#include "rendering/opengl/gl_shader.hpp"
#include "resource/resource_manager.hpp"
#include "rendering/opengl/gl_vertex_array.hpp"
#include "rendering/opengl/gl_vertex_buffer.hpp"

#include "utils/shapes_vao.hpp"

#include "ecs/components/mesh_component.hpp"
namespace Airwave
{
class RendererComponent : public AwComponent
{
  public:
    std::shared_ptr<Texture> envMap;
    std::shared_ptr<Texture> backgroundMap;
    std::shared_ptr<Texture> irradianceMap;

    RendererComponent()
    {
        backgroundShader = ResourceManager::GetInstance().loadShader("background",
                                                                     PROJECT_ROOT_DIR "/assets/"
                                                                                      "shaders/"
                                                                                      "shader_lib/"
                                                                                      "vert/"
                                                                                      "background."
                                                                                      "vert",
                                                                     PROJECT_ROOT_DIR "/assets/"
                                                                                      "shaders/"
                                                                                      "shader_lib/"
                                                                                      "frag/"
                                                                                      "background."
                                                                                      "frag");

        std::vector<AwVertex> cubeVertices;
        std::vector<uint32_t> cubeIndices;
        GeometryUtils::CreateCube(cubeVertices, cubeIndices, 1.0f, 1.0f, 1.0f, 1, 1, 1);
        meshComp = std::make_shared<MeshComponent>(cubeVertices, cubeIndices);
    }

  private:
    std::shared_ptr<Shader> backgroundShader;

    std::shared_ptr<VertexArray> cubeVAO;

    std::shared_ptr<MeshComponent> meshComp;

    friend class RenderSystem;
};

} // namespace Airwave
