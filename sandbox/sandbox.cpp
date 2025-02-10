#include "sandbox.hpp"
#include <glad/glad.h>

#include "controller/trackball_camera_system.hpp"
#include "controller/trackball_controller_component.hpp"

#include "rendering/mesh.hpp"
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
    std::vector<AwVertex> _cubeVertices;
    std::vector<uint32_t> cubeIndices;
    std::vector<AwVertex> _sphereVertices;
    std::vector<uint32_t> sphereIndices;
    GeometryUtils::CreateCube(_cubeVertices, cubeIndices, 1.0f, 1.0f, 1.0f, 1, 1, 1);
    GeometryUtils::CreateSphere(_sphereVertices, sphereIndices, 1.0f, 36, 32);
    std::vector<float> cubeVertices = GeometryUtils::ConvertWaveVertexToFloatArray(_cubeVertices);
    std::vector<float> sphereVertices = GeometryUtils::ConvertWaveVertexToFloatArray(_sphereVertices);
    

    // entity

    // camera
    auto &reg = m_scene->getRegistry();

    auto main_camera_entity = m_scene->createDefaultEntity("main_camera");
    reg.emplace<CameraComponent>(main_camera_entity);
    reg.emplace<TrackballController>(main_camera_entity);
    auto &camera_transform = reg.get<TransformComponent>(main_camera_entity);
    camera_transform.setPosition(glm::vec3(0.0f, 0.0f, 30.0f));


    // // lights
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            auto light_entity     = m_scene->createDefaultEntity("light_" + std::to_string(i) + "_" + std::to_string(j));
            auto &light_comp      = reg.emplace<LightComponent>(light_entity);
            light_comp.intensity  = 300.0f;
            light_comp.color      = glm::vec3(.0f);
            auto &light_transform = reg.emplace<TransformComponent>(light_entity);
            light_transform.setPosition(glm::vec3(i * 10.0f - 5.0f, j * 10.0f - 5.0f, 10.0f));
        }
    }

    // sphere container
    auto sphere_container_entity = m_scene->createDefaultEntity("sphere_container");

    // spheres 7 x 7
    TextureSpecification spec;
    spec.isHDR          = true;
    spec.generateMipmap = false;
    // auto envMap  = RES.load<TextureResource>(std::string("envMap"), std::string(PROJECT_ROOT_DIR "/assets/textures/hdr/kiara_8_sunset_2k.hdr"),
    // spec);
    auto envMap = std::make_shared<TextureResource>();
    envMap->load(std::string(PROJECT_ROOT_DIR "/assets/textures/hdr/newport_loft.hdr"), spec);
    bool load = RES.add("envMap", envMap);

    // HDR to Cubemap
    auto cube_map = TextureUtils::equirectangularToCubemap(m_renderer.get(), envMap, 1024, true);
    // 获取辐照度贴图
    auto irradiance_map = TextureUtils::irradianceConvolution(m_renderer.get(), cube_map, 32);
    // 获取预过滤贴图
    auto prefilter_map = TextureUtils::prefilterEnvMap(m_renderer.get(), cube_map, 256, 5);

    RES.add("cubeMap", cube_map);
    RES.add("irradiance_map", irradiance_map);
    RES.add("prefilter_map", prefilter_map);

    auto adminEntity            = m_scene->getAdminEntity();
    auto &renderer_comp         = reg.emplace<RendererComponent>(adminEntity);
    renderer_comp.backgroundMap = cube_map;
    renderer_comp.envMap        = cube_map;

    auto brdf_lut = RES.get<TextureResource>("brdf_lut");

    auto sphere_mesh = std::make_shared<Mesh>(sphereVertices, sphereIndices);

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            auto sphere_entity = m_scene->createDefaultEntity("sphere_" + std::to_string(i) + "_" + std::to_string(j));
            auto &mesh_comp = reg.emplace<MeshComponent>(sphere_entity);
            mesh_comp.mesh = sphere_mesh;
            auto &mat = reg.emplace<MaterialComponent>(sphere_entity, MaterialType::PBR);

            // mat.color     = glm::vec3(1.0f, 1.0f, 1.0f);
            mat.color     = glm::vec3(0.6f, 0.0f, 0.0f);
            mat.metallic  = glm::clamp(i / 6.0f, 0.0f, 1.0f);
            mat.roughness = glm::clamp(j / 6.0f, 0.05f, 1.0f);
            mat.ao        = 1.0f;

            mat.irradianceMap = irradiance_map;
            mat.prefilterMap  = prefilter_map;

            auto &sphere_transform = reg.get<TransformComponent>(sphere_entity);
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
