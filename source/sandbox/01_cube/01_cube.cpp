#include <iostream>
#include <memory>

#include "aw_engine.hpp"
#include <imgui.h>

#include <glad/glad.h>

class Sandbox : public Airwave::Application
{
  public:
    void onInit() override
    {
        m_eventObserver = std::make_shared<Airwave::EventObserver>();
        m_eventObserver->onEvent<Airwave::MouseMovedEvent>(
            [](const Airwave::MouseMovedEvent &event)
            {
                // std::cout << "MouseMovedEvent: " << event.getX() << ", " << event.getY() <<
                // std::endl;
            });

        // 顶点数据, 四边形
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
        };

        // 索引数据
        std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

        m_vertexArray = std::make_shared<Airwave::VertexArray>();
        {
            m_vertexArray->bind();
            auto vertexBuffer = std::make_shared<Airwave::VertexBuffer>(
                vertices.data(), vertices.size() * sizeof(float));
            vertexBuffer->setLayout({{Airwave::ShaderDataType::Float3, "aPos"},
                                     {Airwave::ShaderDataType::Float2, "aTexCoord"}});
            m_vertexArray->addVertexBuffer(vertexBuffer);
            auto indexBuffer =
                std::make_shared<Airwave::IndexBuffer>(indices.data(), indices.size());
            m_vertexArray->setIndexBuffer(indexBuffer);
            m_vertexArray->unbind();
        }

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 aPos;
            layout(location = 1) in vec2 aTexCoord;

            out vec2 TexCoord;

            void main()
            {
                gl_Position = vec4(aPos, 1.0);
                TexCoord = aTexCoord;
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            out vec4 FragColor;

            in vec2 TexCoord;

            // uniform vec3 u_Color;

            uniform sampler2D u_Texture;

            void main()
            {   
                // FragColor = vec4(u_Color, 1.0);
                FragColor = vec4(texture(u_Texture, TexCoord).rgb,1.0);
            }
        )";

        Airwave::TextureSpecification spec;
        spec.format         = Airwave::TextureFormat::RGBA;
        spec.internalFormat = Airwave::TextureInternalFormat::RGBA8;

        m_texture = std::make_shared<Airwave::Texture>(TEXTURE_DIR "container2.png", spec, true);
        m_shader  = std::make_shared<Airwave::Shader>(vertexSrc, fragmentSrc, false);
        m_shader->bind();
        m_shader->uploadUniformInt("u_Texture", 0.0);

        m_scene     = std::make_shared<Airwave::Scene>(Airwave::SceneSpecification("Sandbox"));
        auto entity = m_scene->createEntity("Cube");
        auto system = m_scene->addSystem(std::make_shared<Airwave::CameraSystem>(), "CameraSystem");
        auto renderSystem = m_scene->addSystem(std::make_shared<Airwave::ForwardRenderSystem>(), "ForwardRenderSystem");
    }

    void onRender() override
    {
        m_scene->updateSystems(0.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        m_shader->bind();
        m_texture->bind(0);
        m_vertexArray->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Triangle");
        ImGui::Text("Hello, Triangle!");
        ImGui::End();
    }

  private:
    std::shared_ptr<Airwave::EventObserver> m_eventObserver;

    std::shared_ptr<Airwave::VertexArray> m_vertexArray;
    std::shared_ptr<Airwave::Shader> m_shader;
    std::shared_ptr<Airwave::Texture> m_texture;

    std::shared_ptr<Airwave::Scene> m_scene;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }