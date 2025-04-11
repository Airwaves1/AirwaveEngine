#pragma once

#include <glad/glad.h>
#include "function/render/shader/shader.hpp"
#include "function/render/texture/texture.hpp"

namespace Airwave
{
struct MeshComponent
{
    constexpr static const char *getComponentName() { return "MeshComponent"; }

    MeshComponent()
    {
        // 立方体的顶点数据
        std::vector<float> vertices = {
            // 位置              // 纹理坐标    // 法线
            -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f, // 前面
            0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 前面
            0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f, // 前面
            -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, // 前面
            -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, 0.0f, // 后面
            0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // 后面
            0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, // 后面
            -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  -1.0f, 1.0f, 1.0f, 0.0f, // 后面
            -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 左面
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 左面
            -0.5f, 0.5f,  -0.5f, -1.0f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 左面
            -0.5f, 0.5f,  0.5f,  -1.0f, 1.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 左面
            0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右面
            0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 右面
            0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 右面
            0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 右面
            -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 上面
            0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 上面
            0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 上面
            -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 上面
            -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, 0.0f, // 底面
            0.5f,  -0.5f, 0.5f,  1.0f,  -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, // 底面
            0.5f,  -0.5f, -0.5f, 1.0f,  -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 底面
            -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, 0.0f  // 底面
        };

        // 立方体的索引数据
        std::vector<unsigned int> indices = {
            0,  1,  2,  2,  3,  0,  // 前面
            4,  5,  6,  6,  7,  4,  // 后面
            8,  9,  10, 10, 11, 8,  // 左面
            12, 13, 14, 14, 15, 12, // 上面
            16, 17, 18, 18, 19, 16, // 底面
            20, 21, 22, 22, 23, 20  // 右面
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glBindVertexArray(vao);

        // 绑定VBO和EBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // 设置位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // 设置纹理坐标属性
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 设置法线属性
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // 解绑VBO和VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        index_count = static_cast<uint64_t>(indices.size()); // 设置索引数量
    }

    // nlohmann::json to_json() const { return nlohmann::json{{"tag", tag}, {"name", name}, {"is_active", is_active}}; }
    // void from_json(const nlohmann::json &j)
    // {
    //     tag       = j.value("tag", "none");
    //     name      = j.value("name", "entity");
    //     is_active = j.value("is_active", true);
    // }

    nlohmann::json to_json() const
    {
        std::string shader_path  = shader ? shader->getResourcePath() : "";
        std::string texture_path = texture ? texture->getResourcePath() : "";

        return nlohmann::json{{"shader", shader_path}, {"texture", texture_path}};
    }

    void from_json(const nlohmann::json &j)
    {
        std::string shader_path  = j.value("shader", "");
        std::string texture_path = j.value("texture", "");

        if (!shader_path.empty())
        {
            shader = std::make_shared<Shader>(shader_path);
            shader->load();
        }

        if (!texture_path.empty())
        {
            Airwave::TextureConfig texture_config;
            texture = std::make_shared<Airwave::Texture>(texture_config, texture_path);
            texture->load();
        }
    }

    GLuint vao = 0; // 顶点数组对象
    GLuint vbo = 0; // 顶点缓冲对象
    GLuint ebo = 0; // 索引缓冲对象

    std::shared_ptr<Shader> shader;   // 着色器对象
    std::shared_ptr<Texture> texture; // 纹理对象

    uint64_t index_count = 0; // 索引数量
};

} // namespace Airwave
