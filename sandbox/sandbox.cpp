#include "sandbox.hpp"
#include <glad/glad.h>

// #include "controller/trackball_camera_system.hpp"
// #include "controller/trackball_controller_component.hpp"
#include "game/components/fps_controller.hpp"
#include "game/components/fps_controller_system.hpp"

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

    // m_scene->addSystem<TrackballCameraSystem>();
    m_scene->addSystem<FPSControllerSystem>();

    // entity

    // camera
    auto &reg = m_scene->getRegistry();

    auto main_camera_entity = m_scene->createDefaultEntity("main_camera");
    auto &camera_comp = m_scene->addComponent<CameraComponent>(main_camera_entity);
    // m_scene->addComponent<TrackballController>(main_camera_entity);
    auto& fps_controller = m_scene->addComponent<FPSControllerComponent>(main_camera_entity);
    auto &camera_transform = reg.get<TransformComponent>(main_camera_entity);
    camera_comp.setFarPlane(1000.0f);
    camera_transform.setPosition(glm::vec3(0.0f, 0.0f, 10.0f));

    // // lights
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            auto light_entity     = m_scene->createDefaultEntity("light_" + std::to_string(i) + "_" + std::to_string(j));
            auto &light_comp      = m_scene->addComponent<LightComponent>(light_entity);
            light_comp.intensity  = 300.0f;
            light_comp.color      = glm::vec3(1.0f);
            auto &light_transform = m_scene->getComponent<TransformComponent>(light_entity);
            light_transform.setPosition(glm::vec3(i * 10.0f - 5.0f, j * 10.0f - 5.0f, 5.0f));
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
    renderer_comp.irradianceMap = irradiance_map;
    renderer_comp.prefilterMap  = prefilter_map;
    renderer_comp.brdfLUT       = RES.get<TextureResource>("brdf_lut")->getTexture();

    auto brdf_lut = RES.get<TextureResource>("brdf_lut")->getTexture();

    auto sphere = GeometryUtils::CreateSphere(1.0f, 36, 32);

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            auto sphere_entity = m_scene->createDefaultEntity("sphere_" + std::to_string(i) + "_" + std::to_string(j));
            auto &mesh_comp    = m_scene->addComponent<MeshComponent>(sphere_entity);
            mesh_comp.primitives.push_back(sphere);
            auto &mat_comp = m_scene->addComponent<MaterialComponent>(sphere_entity, MaterialType::PBR);

            mat_comp.material->color = glm::vec3(0.6f, 0.0f, 0.0f);
            // mat_comp.material->color     = glm::vec3(1.0f, 1.0f, 1.0f);
            mat_comp.material->metallic  = glm::clamp(i / 7.0f, 0.05f, 1.0f);
            mat_comp.material->roughness = glm::clamp(j / 7.0f, 0.05f, 1.0f);
            mat_comp.material->ao        = 1.0f;

            // mat_comp.material->albedoMap     = albedoMap->getTexture();
            // mat_comp.material->normalMap     = normalMap->getTexture();
            // mat_comp.material->metallicMap   = metallicMap->getTexture();
            // mat_comp.material->roughnessMap  = roughnessMap->getTexture();
            mat_comp.material->irradianceMap = irradiance_map;
            mat_comp.material->prefilterMap  = prefilter_map;

            auto &sphere_transform = reg.get<TransformComponent>(sphere_entity);
            sphere_transform.setPosition(glm::vec3(j * 3.0f - 8.0f, i * 3.0f - 8.0f, 0.0f));
            
            auto& sphere_rb = m_scene->addComponent<RigidBodyComponent>(sphere_entity);
            sphere_rb.mass = 1.0f;
            sphere_rb.colliderType = ColliderType::Sphere;
            sphere_rb.shapeSize = glm::vec3(1.0f);

            m_scene->setEntityParent(sphere_entity, sphere_container_entity);
        }
    }

    // ground
    glm::vec3 ground_size = glm::vec3(1000.0f, 0.01f, 1000.0f);
    auto ground_entity = m_scene->createDefaultEntity("ground");
    auto ground        = GeometryUtils::CreatePlane(1.0f, 1.0f, 1, 1);
    auto &mesh_comp    = m_scene->addComponent<MeshComponent>(ground_entity);
    mesh_comp.primitives.push_back(ground);
    auto &mat_comp                   = m_scene->addComponent<MaterialComponent>(ground_entity, MaterialType::PBR);
    mat_comp.material->color         = glm::vec3(0.6f, 0.6f, 0.6f);
    mat_comp.material->metallic      = 0.0f;
    mat_comp.material->roughness     = 0.5f;
    mat_comp.material->ao            = 1.0f;
    mat_comp.material->irradianceMap = irradiance_map;
    mat_comp.material->prefilterMap  = prefilter_map;

    auto &ground_transform = reg.get<TransformComponent>(ground_entity);
    ground_transform.setPosition(glm::vec3(0.0f, -15.0f, 0.0f));
    ground_transform.setScale(ground_size);

    auto& ground_rb = m_scene->addComponent<RigidBodyComponent>(ground_entity);
    ground_rb.mass = 0.0f;
    ground_rb.colliderType = ColliderType::Box;
    ground_rb.shapeSize = ground_size;

    // walls
    // auto wall_0_entity = m_scene->createDefaultEntity("wall_0");
    // auto wall_0        = GeometryUtils::CreatePlane(1.0f, 1.0f, 1, 1);
    // auto &mesh_comp_0  = m_scene->addComponent<MeshComponent>(wall_0_entity);
    // mesh_comp_0.primitives.push_back(wall_0);
    // auto &mat_comp_0                   = m_scene->addComponent<MaterialComponent>(wall_0_entity, MaterialType::PBR);
    // mat_comp_0.material->color         = glm::vec3(0.6f, 0.6f, 0.6f);
    // mat_comp_0.material->metallic      = 0.0f;
    // mat_comp_0.material->roughness     = 0.5f;
    // mat_comp_0.material->ao            = 1.0f;
    // mat_comp_0.material->irradianceMap = irradiance_map;
    // mat_comp_0.material->prefilterMap  = prefilter_map;

    // auto &wall_transform_0 = reg.get<TransformComponent>(wall_0_entity);
    // wall_transform_0.setPosition(glm::vec3(0.0f, 0.0f, -500.0f));
    // wall_transform_0.setScale(glm::vec3(1000.0f, 1000.0f, 1.0f));


    // model 
    auto model_resource = RES.get<ModelResource>("models/DamagedHelmet/glTF/DamagedHelmet.gltf");

    // model_1
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
    auto &model_transform = m_scene->getComponent<TransformComponent>(model_entity);
    model_transform.setPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
                        
    auto &model_rb = m_scene->addComponent<RigidBodyComponent>(model_entity);
    model_rb.mass = 1.0f;
    model_rb.colliderType = ColliderType::Box;
    model_rb.shapeSize = glm::vec3(0.9f);

    // model_2
    auto model_entity_2 = m_scene->createDefaultEntity("model_2");
    model_resource->instantiate(m_scene.get(), model_entity_2);
    m_scene->traverseEntity(model_entity_2,
                            [&](entt::entity entity)
                            {
                                if (m_scene->hasComponent<MaterialComponent>(entity))
                                {
                                    auto &mat_comp                   = m_scene->getComponent<MaterialComponent>(entity);
                                    mat_comp.material->irradianceMap = irradiance_map;
                                    mat_comp.material->prefilterMap  = prefilter_map;
                                }
                            });
    auto &model_transform_2 = m_scene->getComponent<TransformComponent>(model_entity_2);
    model_transform_2.setPosition(glm::vec3(10.0f, 0.0f, 0.0f));

    auto &model_rb_2 = m_scene->addComponent<RigidBodyComponent>(model_entity_2);
    model_rb_2.mass = 1.0f;
    model_rb_2.colliderType = ColliderType::Box;
    model_rb_2.shapeSize = glm::vec3(0.9f);

    m_editor->onDrawDebugInfo = [&, adminEntity, albedoMap, normalMap]()
    {
        ImGui::Begin("Debug Info");

        auto g_buffer = m_renderer->getGBuffer();
        if (g_buffer)
        {
            auto pos = g_buffer->getColorAttachment(0);
            if (pos)
            {
                ImGui::Text("Position");
                ImGui::Image((void *)(intptr_t)pos->getHandle(), ImVec2(350, 256), ImVec2(0, 1), ImVec2(1, 0));
            }

            auto normal = g_buffer->getColorAttachment(1);
            if (normal)
            {
                ImGui::Text("Normal");
                ImGui::Image((void *)(intptr_t)normal->getHandle(), ImVec2(350, 256), ImVec2(0, 1), ImVec2(1, 0));
            }

            auto albedo = g_buffer->getColorAttachment(2);
            if (albedo)
            {
                ImGui::Text("Albedo");
                ImGui::Image((void *)(intptr_t)albedo->getHandle(), ImVec2(350, 256), ImVec2(0, 1), ImVec2(1, 0));
            }

            auto material = g_buffer->getColorAttachment(3);
            if (material)
            {
                ImGui::Text("Material");
                ImGui::Image((void *)(intptr_t)material->getHandle(), ImVec2(350, 256), ImVec2(0, 1), ImVec2(1, 0));
            }

            auto depth = m_renderer->getFramebuffer()->getDepthAttachment();
            if (depth)
            {
                ImGui::Text("Depth");
                ImGui::Image((void *)(intptr_t)depth->getHandle(), ImVec2(350, 256), ImVec2(0, 1), ImVec2(1, 0));
            }

            auto empty_map = m_scene->getComponent<RendererComponent>(adminEntity).emptyMap;
            if (empty_map)
            {
                ImGui::Text("Empty Map");
                ImGui::Image((void *)(intptr_t)empty_map->getHandle(), ImVec2(350, 256), ImVec2(0, 1), ImVec2(1, 0));
            }

            auto default_normal = m_scene->getComponent<RendererComponent>(adminEntity).defaultNormal;
            if (default_normal)
            {
                ImGui::Text("Default Normal");
                ImGui::Image((void *)(intptr_t)default_normal->getHandle(), ImVec2(350, 256), ImVec2(0, 1), ImVec2(1, 0));
            }
        }

        ImGui::End();
    };
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
