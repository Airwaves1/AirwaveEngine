#include "sandbox.hpp"
#include <glad/glad.h>

#include "resource/texture_lib.hpp"

#include "controller/trackball_camera_system.hpp"
#include "controller/trackball_controller_component.hpp"
namespace Airwave
{
void Sandbox::onConfigurate(Airwave::ApplicationConfig &config)
{
    config.width  = 1920;
    config.height = 1080;
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
    GeometryUtils::CreateSphere(sphereVertices, sphereIndices, 1.0f, 36, 32);

    // entity

    // camera
    auto main_camera_entity = m_scene->createDefaultEntity("main_camera");
    main_camera_entity->addComponent<CameraComponent>();
    main_camera_entity->addComponent<TrackballController>();
    auto &camera_transform = main_camera_entity->getComponent<TransformComponent>();
    camera_transform.setPosition(glm::vec3(0.0f, 0.0f, 30.0f));

    // // lights
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            auto light_entity    = m_scene->createDefaultEntity("light_" + std::to_string(i) + "_" +
                                                                std::to_string(j));
            auto &light_comp     = light_entity->addComponent<LightComponent>();
            light_comp.intensity = 300.0f;
            light_comp.color     = glm::vec3(1.0f);
            auto &light_transform = light_entity->getComponent<TransformComponent>();
            light_transform.setPosition(glm::vec3(i * 10.0f - 5.0f, j * 10.0f - 5.0f, 10.0f));
        }
    }

    // sphere container
    auto sphere_container_entity = m_scene->createDefaultEntity("sphere_container");

    // spheres 7 x 7
    TextureSpecification spec;
    spec.internalFormat = TextureInternalFormat::SRGB;
    auto albedoMap = ResourceManager::GetInstance().loadTexture(PROJECT_ROOT_DIR "/assets/textures/"
                                                                                 "rustediron1-alt2-"
                                                                                 "Unreal-Engine/"
                                                                                 "rustediron2_"
                                                                                 "basecolor.png",
                                                                spec);

    spec.internalFormat = TextureInternalFormat::RGB;
    auto normalMap = ResourceManager::GetInstance().loadTexture(PROJECT_ROOT_DIR "/assets/textures/"
                                                                                 "rustediron1-alt2-"
                                                                                 "Unreal-Engine/"
                                                                                 "rustediron2_"
                                                                                 "normal.png",

                                                                spec);

    auto metallicMap = ResourceManager::GetInstance().loadTexture(PROJECT_ROOT_DIR "/assets/"
                                                                                   "textures/"
                                                                                   "rustediron1-"
                                                                                   "alt2-"
                                                                                   "Unreal-Engine/"
                                                                                   "rustediron2_"
                                                                                   "metallic.png",
                                                                  spec);

    auto roughnessMap = ResourceManager::GetInstance().loadTexture(PROJECT_ROOT_DIR "/assets/"
                                                                                    "textures/"
                                                                                    "rustediron1-"
                                                                                    "alt2-"
                                                                                    "Unreal-Engine/"
                                                                                    "rustediron2_"
                                                                                    "roughness.png",
                                                                   spec);

    spec.isHDR          = true;
    spec.generateMipmap = false;
    auto envMap = ResourceManager::GetInstance().loadTexture(PROJECT_ROOT_DIR "/assets/textures/"
                                                                              "hdr/"
                                                                              "newport_loft.hdr",
                                                             spec);

    // HDR to Cubemap
    auto cubemap = TextureUtils::equirectangularToCubemap(m_renderer.get(), envMap, 1024, true);
    auto irradiance_map = TextureUtils::irradianceConvolution(m_renderer.get(), cubemap, 32);

    auto adminEntity            = m_scene->getAdminEntity();
    auto &renderer_comp         = adminEntity->getComponent<RendererComponent>();
    renderer_comp.backgroundMap = irradiance_map;
    renderer_comp.envMap        = irradiance_map;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            auto sphere_entity = m_scene->createDefaultEntity("sphere_" + std::to_string(i) + "_" +
                                                              std::to_string(j));
            sphere_entity->addComponent<MeshComponent>(sphereVertices, sphereIndices);
            auto &mat = sphere_entity->addComponent<MaterialComponent>(MaterialType::PBR);
            // mat.color     = glm::vec3(1.0f, 1.0f, 1.0f);
            mat.color     = glm::vec3(0.6f, 0.0f, 0.0f);
            mat.metallic  = glm::clamp(i / 6.0f, 0.0f, 1.0f);
            mat.roughness = glm::clamp(j / 6.0f, 0.05f, 1.0f);
            mat.ao        = 1.0f;

            // mat.albedoMap    = albedoMap;
            // mat.normalMap    = normalMap;
            // mat.metallicMap  = metallicMap;
            // mat.roughnessMap = roughnessMap;
            mat.irradianceMap = irradiance_map;

            auto &sphere_transform = sphere_entity->getComponent<TransformComponent>();
            sphere_transform.setPosition(glm::vec3(j * 3.0f - 8.0f, i * 3.0f - 8.0f, 0.0f));
            m_scene->setEntityParent(sphere_entity, sphere_container_entity);
        }
    }
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
