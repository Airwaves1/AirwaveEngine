#include "sandbox.hpp"
#include <glad/glad.h>

#include "controller/trackball_camera_system.hpp"
#include "controller/trackball_controller_component.hpp"

// #include "resource/model_resource.hpp"

namespace Airwave
{
void Sandbox::onConfigurate(Airwave::ApplicationConfig &config)
{
    config.width  = 1920;
    config.height = 1080;
    config.title  = "Airwave Editor";
}

void Sandbox::onPreLoad()
{
    // Model
    auto model_0 = RES.load<ModelResource>("models/DamagedHelmet/glTF/DamagedHelmet.gltf");

}

void Sandbox::onInit()
{
    m_editor = std::make_unique<Airwave::Editor>(this);

    m_scene->addSystem<TrackballCameraSystem>();

    // entity

    // camera
    auto &reg = m_scene->getRegistry();

    auto main_camera_entity = m_scene->createDefaultEntity("main_camera");
    m_scene->addComponent<CameraComponent>(main_camera_entity);
    m_scene->addComponent<TrackballController>(main_camera_entity);
    auto &camera_transform = reg.get<TransformComponent>(main_camera_entity);
    camera_transform.setPosition(glm::vec3(0.0f, 0.0f, 30.0f));

    // // lights
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            auto light_entity     = m_scene->createDefaultEntity("light_" + std::to_string(i) + "_" + std::to_string(j));
            auto &light_comp      = m_scene->addComponent<LightComponent>(light_entity);
            light_comp.intensity  = 200.0f;
            light_comp.color      = glm::vec3(1.0f);
            auto &light_transform = m_scene->getComponent<TransformComponent>(light_entity);
            light_transform.setPosition(glm::vec3(i * 10.0f - 5.0f, j * 10.0f - 5.0f, 10.0f));
        }
    }

    // sphere container
    auto sphere_container_entity = m_scene->createDefaultEntity("sphere_container");

    // spheres 7 x 7
    TextureSpecification spec;
    spec.sRGB      = true;
    auto albedoMap = RES.load<TextureResource>("textures/rustediron1-alt2-Unreal-Engine/rustediron2_basecolor.png", spec);

    spec.sRGB      = false;
    auto normalMap = RES.load<TextureResource>("textures/rustediron1-alt2-Unreal-Engine/rustediron2_normal.png", spec);

    auto metallicMap  = RES.load<TextureResource>("textures/rustediron1-alt2-Unreal-Engine/rustediron2_metallic.png", spec);
    auto roughnessMap = RES.load<TextureResource>("textures/rustediron1-alt2-Unreal-Engine/rustediron2_roughness.png", spec);

    spec.isHDR          = true;
    spec.flip           = true;
    spec.generateMipmap = false;
    auto envMap         = RES.load<TextureResource>("textures/hdr/newport_loft.hdr", spec);

    // HDR to Cubemap
    auto cube_map = TextureUtils::equirectangularToCubemap(m_renderer.get(), envMap->getTexture(), 1024, true);
    // // 获取辐照度贴图
    auto irradiance_map = TextureUtils::irradianceConvolution(m_renderer.get(), cube_map, 32);
    // // 获取预过滤贴图
    auto prefilter_map = TextureUtils::prefilterEnvMap(m_renderer.get(), cube_map, 256, 5);

    RES.add<TextureResource>("cubeMap", std::make_shared<TextureResource>(cube_map));
    RES.add<TextureResource>("irradiance_map", std::make_shared<TextureResource>(irradiance_map));
    RES.add<TextureResource>("prefilter_map", std::make_shared<TextureResource>(prefilter_map));

    auto adminEntity            = m_scene->getAdminEntity();
    auto &renderer_comp         = m_scene->getComponent<RendererComponent>(adminEntity);
    renderer_comp.backgroundMap = cube_map;
    renderer_comp.envMap        = cube_map;

    auto brdf_lut             = RES.get<TextureResource>("brdf_lut")->getTexture();
    m_editor->onDrawDebugInfo = [this, brdf_lut]()
    {
        ImGui::Begin("Debug Info");
        ImGui::Image((void *)(intptr_t)brdf_lut->getHandle(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    };

    // auto sphere = GeometryUtils::CreateSphere(1.0f, 36, 32);

    // for (int i = 0; i < 7; i++)
    // {
    //     for (int j = 0; j < 7; j++)
    //     {
    //         auto sphere_entity = m_scene->createDefaultEntity("sphere_" + std::to_string(i) + "_" + std::to_string(j));
    //         auto &mesh_comp    = m_scene->addComponent<MeshComponent>(sphere_entity);
    //         mesh_comp.primitives.push_back(sphere);
    //         auto &mat_comp = m_scene->addComponent<MaterialComponent>(sphere_entity, MaterialType::PBR);

    //         // mat_comp.material->color     = glm::vec3(0.6f, 0.0f, 0.0f);
    //         mat_comp.material->color     = glm::vec3(1.0f, 1.0f, 1.0f);
    //         mat_comp.material->metallic  = glm::clamp(i / 6.0f, 0.0f, 1.0f);
    //         mat_comp.material->roughness = glm::clamp(j / 6.0f, 0.05f, 1.0f);
    //         mat_comp.material->ao        = 1.0f;

    //         mat_comp.material->albedoMap     = albedoMap->getTexture();
    //         mat_comp.material->normalMap     = normalMap->getTexture();
    //         mat_comp.material->metallicMap   = metallicMap->getTexture();
    //         mat_comp.material->roughnessMap  = roughnessMap->getTexture();
    //         mat_comp.material->irradianceMap = irradiance_map;
    //         mat_comp.material->prefilterMap  = prefilter_map;

    //         auto &sphere_transform = reg.get<TransformComponent>(sphere_entity);
    //         sphere_transform.setPosition(glm::vec3(j * 3.0f - 8.0f, i * 3.0f - 8.0f, 0.0f));
    //         m_scene->setEntityParent(sphere_entity, sphere_container_entity);
    //     }
    // }

    // model
    auto model_resource = RES.get<ModelResource>("models/DamagedHelmet/glTF/DamagedHelmet.gltf");
    auto model_entity = m_scene->createDefaultEntity("model");
    model_resource->instantiate(m_scene.get(), model_entity);
    m_scene->traverseEntity(model_entity,
                            [&](entt::entity entity)
                            {
                                if (m_scene->hasComponent<MaterialComponent>(entity))
                                {
                                    auto &mat_comp                   = m_scene->getComponent<MaterialComponent>(entity);
                                    mat_comp.material->irradianceMap = irradiance_map;
                                    mat_comp.material->prefilterMap  = prefilter_map;
                                }
                            });
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
