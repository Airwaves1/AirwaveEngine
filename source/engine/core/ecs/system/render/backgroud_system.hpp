#pragma once

#include <glm/glm.hpp>
#include "ecs/system/aw_system.hpp"

#include "opengl/gl_vertex_array.hpp"
#include "opengl/gl_texture.hpp"
#include "opengl/gl_cube_texture.hpp"
#include "resource/shader_lib.hpp"
#include "utils/file_utils.hpp"

#include "ecs/component/camera/camera_component.hpp"

namespace Airwave
{

enum class BackgroundType
{
    Color,
    CubeTexture,
    Skybox
};

class BackgroundSystem : public AwSystem
{
  public:
    BackgroundSystem(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
        : m_type(BackgroundType::Color), m_color(color)
    {
        setBackgroud(color);
    }

    BackgroundSystem(const std::shared_ptr<CubeTexture> &cubeTexture)
        : m_type(BackgroundType::CubeTexture), m_cubeTexture(cubeTexture)
    {
        setBackgroud(cubeTexture);
    }

    BackgroundSystem(const std::shared_ptr<Texture> &skyboxTexture)
        : m_type(BackgroundType::Skybox), m_skyboxTexture(skyboxTexture)
    {
        setBackgroud(skyboxTexture);
    }

    virtual ~BackgroundSystem() {}

    virtual void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) {}

    void setBackgroud(const glm::vec4 &color)
    {
        m_type  = BackgroundType::Color;
        m_color = color;
    }

    void setBackgroud(const std::shared_ptr<CubeTexture> &cubeTexture)
    {
        m_type        = BackgroundType::CubeTexture;
        m_cubeTexture = cubeTexture;
    }

    void setBackgroud(const std::shared_ptr<Texture> &skyboxTexture)
    {
        m_type          = BackgroundType::Skybox;
        m_skyboxTexture = skyboxTexture;
    }

    void render(const std::shared_ptr<Camera>& camera)
    {
        if (m_vertexArray == nullptr)
        {
            m_vertexArray = std::make_shared<VertexArray>();

            std::vector<float> vertices = {
                -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
            std::vector<uint32_t> indices = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
                                             8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
                                             16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

            auto vertexBuffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
            {
                BufferLayout layout = {
                    {ShaderDataType::Float3, "a_position"},
                };
                vertexBuffer->setLayout(layout);
                m_vertexArray->addVertexBuffer(vertexBuffer);
            }
            auto indexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
            m_vertexArray->setIndexBuffer(indexBuffer);

            m_shader =
                SHADER_LIB.load("background", SHADER_DIR "cube_texture_background.vert",
                                SHADER_DIR "cube_texture_background.frag");
        }

        m_shader->bind();
        m_shader->uploadUniformMat4("u_viewMatrix", glm::mat4(glm::mat3(camera->getViewMatrix())));
        m_shader->uploadUniformMat4("u_projectionMatrix", camera->getProjectionMatrix());
        glDepthFunc(GL_LEQUAL);
        switch (m_type)
        {
            case BackgroundType::Color:
                glClearColor(m_color.r, m_color.g, m_color.b, m_color.a);
                glClear(GL_COLOR_BUFFER_BIT);
                break;
            case BackgroundType::CubeTexture:
                m_cubeTexture->bind();
                m_vertexArray->bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);
                break;
            case BackgroundType::Skybox:
                m_skyboxTexture->bind();
                m_vertexArray->bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);
                break;
            default:
                break;
        }
        glDepthFunc(GL_LESS);
    }

  private:
    BackgroundType m_type;
    glm::vec4 m_color;
    std::shared_ptr<CubeTexture> m_cubeTexture;
    std::shared_ptr<Texture> m_skyboxTexture;

    std::shared_ptr<VertexArray> m_vertexArray;
    std::shared_ptr<Shader> m_shader;
};

} // namespace Airwave
