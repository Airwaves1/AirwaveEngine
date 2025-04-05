#include "sandbox.hpp"
#include <glad/glad.h>

void Sandbox::onInit()
{
    // auto world = Airwave::GlobalContext.world;
    // world->loadScene("test");

    // world->traverseHierarchy(
    //     world->getadminEntity(),
    //     [](entt::entity entity)
    //     {
    //         auto &tag = Airwave::GlobalContext.world->getComponent<Airwave::TagComponent>(entity);
    //         LOG_DEBUG("Entity Name: {}, Tag: {}", tag.name, tag.tag);
    //     },
    //     false);

    // world->createDefaultEntity("e1", "player", true);
    // world->createDefaultEntity("e2", "enemy", true);
    // world->createDefaultEntity("e3", "enemy", true);
    // world->createDefaultEntity("e4", "enemy", true);
    // world->createDefaultEntity("e5", "enemy", true);
    // world->createDefaultEntity("e6", "enemy", true);
    // world->createDefaultEntity("e7", "enemy", true);

    // world->setParent(world->getEntityByName("e2"), world->getEntityByName("e1"));
    // world->setParent(world->getEntityByName("e3"), world->getEntityByName("e1"));
    // world->setParent(world->getEntityByName("e4"), world->getEntityByName("e1"));

    // world->removeChild(world->getEntityByName("e1"), world->getEntityByName("e3"));

    std::vector<float> vertices = {
        -0.05f, -0.05f, 0.0f, 0.0f, 0.0f, 0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 0.05f, 0.05f, 0.0f, 1.0f, 1.0f, -0.05f, 0.05f, 0.0f, 0.0f, 1.0f,
    };
    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_shader = std::make_shared<Airwave::Shader>(Airwave::AssetsPath + "shader/shader_lib/basic.glsl");
    m_shader->load();

    Airwave::TextureConfig texture_config;
    m_texture = std::make_shared<Airwave::Texture>(texture_config, Airwave::AssetsPath + "texture/R-C.jpeg");
    m_texture->load();
}

void Sandbox::onUpdate(float delta_time)
{
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader->bind();
    m_texture->bind(0);
    m_shader->setUniform("u_texture_0", 0);
    m_shader->commitUniforms();
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Sandbox::onShutdown()
{
    // auto world = Airwave::GlobalContext.world;
    // world->saveScene("test");
}
