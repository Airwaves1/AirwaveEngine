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

    // data
    std::vector<AwVertex> cubeVertices;
    std::vector<uint32_t> cubeIndices;
    std::vector<AwVertex> sphereVertices;
    std::vector<uint32_t> sphereIndices;
    GeometryUtils::CreateCube(cubeVertices, cubeIndices, 1.0f, 1.0f, 1.0f, 1, 1, 1);
    GeometryUtils::CreateSphere(sphereVertices, sphereIndices, 1.0f, 36, 18);

    // entity

    // camera
    auto main_camera_entity = m_scene->createDefaultEntity("main_camera");
    main_camera_entity->addComponent<CameraComponent>();
    main_camera_entity->addComponent<TrackballController>();
    auto &camera_transform = main_camera_entity->getComponent<TransformComponent>();
    camera_transform.setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    // cube
    auto cube_entity = m_scene->createDefaultEntity("cube");
    auto &cubeMesh   = cube_entity->addComponent<MeshComponent>();
    auto &cubeMat    = cube_entity->addComponent<MaterialComponent>(MaterialType::PBR);
    // cubeMat.setMaterialType(MaterialType::PBR);
    cubeMesh.setData(cubeVertices, cubeIndices);
    auto &cubeTransform = cube_entity->getComponent<TransformComponent>();
    cubeTransform.setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    cubeMat.color = glm::vec3(0.3, 0.5, 0.8);

    // sphere
    auto sphere_entity  = m_scene->createDefaultEntity("sphere");
    auto &sphereMesh    = sphere_entity->addComponent<MeshComponent>();
    auto &sphereMat     = sphere_entity->addComponent<MaterialComponent>(MaterialType::PBR);
    sphereMat.color     = glm::vec3(0.6, 0.4, 0.2);
    sphereMat.roughness = 0.3;
    sphereMat.metallic  = 0.7;
     // sphereMat.setMaterialType(MaterialType::PBR);
    sphereMesh.setData(sphereVertices, sphereIndices);
    auto &sphereTransform = sphere_entity->getComponent<TransformComponent>();
    sphereTransform.setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));

    // light
    auto light_entity    = m_scene->createDefaultEntity("direction light");
    auto &light          = light_entity->addComponent<LightComponent>();
    auto &lightTransform = light_entity->getComponent<TransformComponent>();
    light.color          = glm::vec3(0.8);
    lightTransform.setPosition(glm::vec3(2.0, 2.0, 2.0));

    // m_scene->setEntityParent(sphere_entity, cube_entity);
    // TextureSpecification textureSpec;
    // cubeMat.diffuseMap =
    //     TEXTURE_LIB.load(PROJECT_ROOT_DIR "/assets/textures/R-C.jpeg", textureSpec);
}

void Sandbox::onDestory() {}

void Sandbox::onUpdate(float deltaTime) { m_editor->onUpdate(deltaTime); }

void Sandbox::onRender() {}

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
