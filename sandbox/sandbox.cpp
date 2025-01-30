#include "sandbox.hpp"
#include <glad/glad.h>

#include "resource/texture_lib.hpp"

#include "controller/trackball_camera_system.hpp"
#include "controller/trackball_controller_component.hpp"
namespace Airwave
{
void Sandbox::onConfigurate(Airwave::ApplicationConfig &config)
{
    config.width  = 2100;
    config.height = 1350;
    config.title  = "Airwave Editor";
}

void Sandbox::onInit()
{
    m_editor = std::make_unique<Airwave::Editor>(this);

    m_scene->addSystem<TrackballCameraSystem>();

    auto main_camera_entity = m_scene->createDefaultEntity("main_camera");
    main_camera_entity->addComponent<CameraComponent>();
    main_camera_entity->addComponent<TrackballController>();
    auto &camera_transform = main_camera_entity->getComponent<TransformComponent>();
    camera_transform.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    auto cube_entity = m_scene->createDefaultEntity("cube");
    auto &cubeMesh   = cube_entity->addComponent<MeshComponent>();
    auto &cubeMat    = cube_entity->addComponent<MaterialComponent>(MaterialType::Basic);

    std::vector<AwVertex> cubeVertices;
    std::vector<uint32_t> cubeIndices;
    GeometryUtils::CreateCube(cubeVertices, cubeIndices, 1.0f, 1.0f, 1.0f, 1, 1, 1);
    cubeMesh.setData(cubeVertices, cubeIndices);

    TextureSpecification textureSpec;
    cubeMat.diffuseMap =
        TEXTURE_LIB.load(PROJECT_ROOT_DIR "/assets/textures/R-C.jpeg", textureSpec);
}

void Sandbox::onDestory() {}

void Sandbox::onUpdate(float deltaTime) { m_editor->onUpdate(deltaTime); }

void Sandbox::onRender()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Sandbox::onImGuiRender()
{
    m_editor->beginNewFrame();
    m_editor->beginDockSpace();
    m_editor->endDockSpace();
    m_editor->renderPanels();
    m_editor->render();
    m_editor->dealMultiViewport();
    m_editor->endNewFrame();
}

} // namespace Airwave
