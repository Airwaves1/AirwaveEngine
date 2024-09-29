#include <iostream>
#include "aw_engine.hpp"
#include <imgui.h>

#include <glad/glad.h>

class Sandbox : public Airwave::Application
{
  public:
    void onInit() override
    {
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, // bottom left
            0.5f,  -0.5f, 0.0f, // bottom right
            0.0f,  0.5f,  0.0f  // top
        };

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 aPos;

            void main()
            {
                gl_Position = vec4(aPos, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            out vec4 FragColor;

            uniform vec3 u_Color;

            void main()
            {   
                FragColor = vec4(u_Color, 1.0);
            }
        )";
        m_shader = std::make_shared<Airwave::Shader>(vertexSrc, fragmentSrc, false);
        m_shader->bind();
        m_shader->uploadUniformFloat3("u_Color", glm::vec3(0.4f, 0.7f, 0.9f));
    }

    void onRender() override
    {
        glClear(GL_COLOR_BUFFER_BIT);

        m_shader->bind();

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void onImGuiRender() override
    {
        ImGui::Begin("Triangle");
        ImGui::Text("Hello, Triangle!");
        ImGui::End();
    }

  private:
    uint32_t m_vao, m_vbo;
    std::shared_ptr<Airwave::Shader> m_shader;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }