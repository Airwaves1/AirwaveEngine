#include "sandbox.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "component/movement_system.hpp"

void Sandbox::onInit()
{
    // m_shader = std::make_shared<Airwave::Shader>(Airwave::AssetsPath + "shader/shader_lib/basic.glsl");
    // m_shader->load();

    // Airwave::TextureConfig texture_config;
    // m_texture = std::make_shared<Airwave::Texture>(texture_config, Airwave::AssetsPath + "texture/R-C.jpeg");
    // m_texture->load();

    auto world = Airwave::GlobalContext.world;
    world->registerSystem<Airwave::MovementSystem>(6);
    world->loadScene("test");

    // world->createDefaultEntity("main_camera", "camera");
    // world->addComponent<Airwave::CameraComponent>(world->getEntityByName("main_camera"));
    // world->addComponent<Airwave::FirstPersonControllerComponent>(world->getEntityByName("main_camera"));
    // auto &camera_trans = world->getComponent<Airwave::TransformComponent>(world->getEntityByName("main_camera"));
    // camera_trans.setPosition(glm::vec3(0.0f, 1.0f, 10.0f));

    // world->createDefaultEntity("box_0", "item");
    // auto &mesh_comp       = world->addComponent<Airwave::MeshComponent>(world->getEntityByName("box_0"));
    // mesh_comp.shader      = m_shader;
    // mesh_comp.texture     = m_texture;

    glEnable(GL_DEPTH_TEST);
}

void Sandbox::onUpdate(float delta_time) {}

void Sandbox::onShutdown() {
    auto world = Airwave::GlobalContext.world;
    world->saveScene("test");
}
