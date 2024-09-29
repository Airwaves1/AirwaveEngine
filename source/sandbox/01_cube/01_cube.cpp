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
        m_eventObserver->onEvent<Airwave::MouseMovedEvent>([](const Airwave::MouseMovedEvent &event) {
            // std::cout << "MouseMovedEvent: " << event.getX() << ", " << event.getY() << std::endl;
        });

        // 创建立方体数据
        // 顶点数据
        float cubeVertices[] = {
            // 位置           // 法线           // UV
            -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,  -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

            -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, -1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,
            1.0f,  1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,

            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
            -1.0f, 1.0f,  1.0f,  -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,

            1.0f,  -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -1.0f, 1.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, -1.0f, 1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

            -1.0f, -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
            1.0f,  -1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,
        };

        // 索引数据
        unsigned int cubeIndices[] = {
            0,  1,  2,  2,  3,  0,
            4,  5,  6,  6,  7,  4,
            8,  9,  10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20,
        };

        // 创建立方体
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

        // 位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // 法线属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // UV属性
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);



        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

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

        m_scene = std::make_shared<Airwave::Scene>(Airwave::SceneSpecification("Sandbox"));
        auto entity = m_scene->createEntity("Cube");
        auto system = m_scene->addSystem(std::make_shared<Airwave::CameraSystem>(), "CameraSystem");
    }

    void onRender() override
    {
        m_scene->updateSystems(0.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        m_shader->bind();
        m_texture->bind(0);
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Triangle");
        ImGui::Text("Hello, Triangle!");
        ImGui::End();
    }

  private:
    std::shared_ptr<Airwave::EventObserver> m_eventObserver;
    uint32_t m_vao, m_vbo, m_ibo;
    std::shared_ptr<Airwave::Shader> m_shader;
    std::shared_ptr<Airwave::Texture> m_texture;

    std::shared_ptr<Airwave::Scene> m_scene;

    
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }