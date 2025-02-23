#include "sandbox.hpp"
#include <glad/glad.h>

// #include "controller/trackball_camera_system.hpp"
// #include "controller/trackball_controller_component.hpp"
#include "game/components/fps_controller.hpp"
#include "game/components/fps_controller_system.hpp"
#include <glm/gtc/random.hpp>

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
    auto &camera_comp       = m_scene->addComponent<CameraComponent>(main_camera_entity);
    // m_scene->addComponent<TrackballController>(main_camera_entity);
    auto &fps_controller   = m_scene->addComponent<FPSControllerComponent>(main_camera_entity);
    auto &camera_transform = reg.get<TransformComponent>(main_camera_entity);
    camera_comp.setFarPlane(1000.0f);
    camera_transform.setPosition(glm::vec3(-50.0f, 30.0f, 50.0f));
    camera_transform.setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));

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

    // auto sphere = GeometryUtils::CreateSphere(1.0f, 36, 32);

    // for (int i = 0; i < 7; i++)
    // {
    //     for (int j = 0; j < 7; j++)
    //     {
    //         auto sphere_entity = m_scene->createDefaultEntity("sphere_" + std::to_string(i) + "_" + std::to_string(j));
    //         auto &mesh_comp    = m_scene->addComponent<MeshComponent>(sphere_entity);
    //         mesh_comp.primitives.push_back(sphere);
    //         auto &mat_comp = m_scene->addComponent<MaterialComponent>(sphere_entity, MaterialType::PBR);

    //         mat_comp.material->color = glm::vec3(0.6f, 0.0f, 0.0f);
    //         // mat_comp.material->color     = glm::vec3(1.0f, 1.0f, 1.0f);
    //         mat_comp.material->metallic  = glm::clamp(i / 7.0f, 0.05f, 1.0f);
    //         mat_comp.material->roughness = glm::clamp(j / 7.0f, 0.05f, 1.0f);
    //         mat_comp.material->ao        = 1.0f;

    //         // mat_comp.material->albedoMap     = albedoMap->getTexture();
    //         // mat_comp.material->normalMap     = normalMap->getTexture();
    //         // mat_comp.material->metallicMap   = metallicMap->getTexture();
    //         // mat_comp.material->roughnessMap  = roughnessMap->getTexture();
    //         mat_comp.material->irradianceMap = irradiance_map;
    //         mat_comp.material->prefilterMap  = prefilter_map;

    //         auto &sphere_transform = reg.get<TransformComponent>(sphere_entity);
    //         sphere_transform.setPosition(glm::vec3(j * 3.0f - 8.0f, i * 3.0f - 8.0f, 0.0f));

    //         auto &sphere_rb        = m_scene->addComponent<RigidBodyComponent>(sphere_entity);
    //         sphere_rb.mass         = 1.0f;
    //         sphere_rb.colliderType = ColliderType::Sphere;
    //         sphere_rb.shapeSize    = glm::vec3(1.0f);

    //         m_scene->setEntityParent(sphere_entity, sphere_container_entity);
    //     }
    // }

    auto play_ground_entity = m_scene->createDefaultEntity("play_ground");

    auto cube = GeometryUtils::CreateCube(1.0f, 1.0f, 1);
    // ground
    glm::vec3 ground_size = glm::vec3(150.0f, 0.01f, 100.0f);
    glm::vec3 ground_pos  = glm::vec3(0.0f, 0.0f, 0.0f);
    auto ground_entity    = m_scene->createDefaultEntity("ground");
    auto &mesh_comp       = m_scene->addComponent<MeshComponent>(ground_entity);
    mesh_comp.primitives.push_back(cube);
    auto &mat_comp                   = m_scene->addComponent<MaterialComponent>(ground_entity, MaterialType::PBR);
    mat_comp.material->color         = glm::vec3(0.6f, 0.6f, 0.6f);
    mat_comp.material->metallic      = 0.0f;
    mat_comp.material->roughness     = 0.5f;
    mat_comp.material->ao            = 1.0f;
    mat_comp.material->irradianceMap = irradiance_map;
    mat_comp.material->prefilterMap  = prefilter_map;

    auto &ground_transform = reg.get<TransformComponent>(ground_entity);
    ground_transform.setPosition(ground_pos);
    ground_transform.setScale(ground_size);

    auto &ground_rb        = m_scene->addComponent<RigidBodyComponent>(ground_entity);
    ground_rb.mass         = 0.0f;
    ground_rb.colliderType = ColliderType::Box;
    ground_rb.shapeSize    = ground_size;

    // walls
    float wall_height    = 20.0f;
    float wall_thickness = 1.0f;
    // front
    glm::vec3 front_size  = glm::vec3(ground_size.x, wall_height, wall_thickness);
    auto front_entity     = m_scene->createDefaultEntity("front_wall");
    auto &front_mesh_comp = m_scene->addComponent<MeshComponent>(front_entity);
    front_mesh_comp.primitives.push_back(cube);
    auto &front_mat_comp                   = m_scene->addComponent<MaterialComponent>(front_entity, MaterialType::PBR);
    front_mat_comp.material->color         = glm::vec3(0.6f, 0.6f, 0.6f);
    front_mat_comp.material->metallic      = 0.0f;
    front_mat_comp.material->roughness     = 0.5f;
    front_mat_comp.material->ao            = 1.0f;
    front_mat_comp.material->irradianceMap = irradiance_map;
    front_mat_comp.material->prefilterMap  = prefilter_map;

    auto &front_transform = reg.get<TransformComponent>(front_entity);
    front_transform.setPosition(glm::vec3(0.0f, ground_pos.y + ground_size.y * 0.5f + wall_height * 0.5f, ground_size.z * 0.5f));
    front_transform.setScale(front_size);

    auto &front_rb        = m_scene->addComponent<RigidBodyComponent>(front_entity);
    front_rb.mass         = 0.0f;
    front_rb.colliderType = ColliderType::Box;
    front_rb.shapeSize    = front_size;

    // back
    glm::vec3 back_size  = glm::vec3(ground_size.x, wall_height, wall_thickness);
    auto back_entity     = m_scene->createDefaultEntity("back_wall");
    auto &back_mesh_comp = m_scene->addComponent<MeshComponent>(back_entity);
    back_mesh_comp.primitives.push_back(cube);
    auto &back_mat_comp              = m_scene->addComponent<MaterialComponent>(back_entity, MaterialType::PBR);
    back_mat_comp.material->color    = glm::vec3(0.6f, 0.6f, 0.6f);
    back_mat_comp.material->metallic = 0.0f;

    back_mat_comp.material->roughness     = 0.5f;
    back_mat_comp.material->ao            = 1.0f;
    back_mat_comp.material->irradianceMap = irradiance_map;
    back_mat_comp.material->prefilterMap  = prefilter_map;

    auto &back_transform = reg.get<TransformComponent>(back_entity);
    back_transform.setPosition(glm::vec3(0.0f, ground_pos.y + ground_size.y * 0.5f + wall_height * 0.5f, -ground_size.z * 0.5f));
    back_transform.setScale(back_size);

    auto &back_rb        = m_scene->addComponent<RigidBodyComponent>(back_entity);
    back_rb.mass         = 0.0f;
    back_rb.colliderType = ColliderType::Box;
    back_rb.shapeSize    = back_size;

    // left
    glm::vec3 left_size  = glm::vec3(wall_thickness, wall_height, ground_size.z);
    auto left_entity     = m_scene->createDefaultEntity("left_wall");
    auto &left_mesh_comp = m_scene->addComponent<MeshComponent>(left_entity);
    left_mesh_comp.primitives.push_back(cube);
    auto &left_mat_comp                   = m_scene->addComponent<MaterialComponent>(left_entity, MaterialType::PBR);
    left_mat_comp.material->color         = glm::vec3(0.6f, 0.6f, 0.6f);
    left_mat_comp.material->metallic      = 0.0f;
    left_mat_comp.material->roughness     = 0.5f;
    left_mat_comp.material->ao            = 1.0f;
    left_mat_comp.material->irradianceMap = irradiance_map;
    left_mat_comp.material->prefilterMap  = prefilter_map;

    auto &left_transform = reg.get<TransformComponent>(left_entity);
    left_transform.setPosition(glm::vec3(-ground_size.x * 0.5f, ground_pos.y + ground_size.y * 0.5f + wall_height * 0.5f, 0.0f));
    left_transform.setScale(left_size);

    auto &left_rb        = m_scene->addComponent<RigidBodyComponent>(left_entity);
    left_rb.mass         = 0.0f;
    left_rb.colliderType = ColliderType::Box;
    left_rb.shapeSize    = left_size;

    // right
    glm::vec3 right_size  = glm::vec3(wall_thickness, wall_height, ground_size.z);
    auto right_entity     = m_scene->createDefaultEntity("right_wall");
    auto &right_mesh_comp = m_scene->addComponent<MeshComponent>(right_entity);
    right_mesh_comp.primitives.push_back(cube);
    auto &right_mat_comp = m_scene->addComponent<MaterialComponent>(right_entity, MaterialType::PBR);

    right_mat_comp.material->color         = glm::vec3(0.6f, 0.6f, 0.6f);
    right_mat_comp.material->metallic      = 0.0f;
    right_mat_comp.material->roughness     = 0.5f;
    right_mat_comp.material->ao            = 1.0f;
    right_mat_comp.material->irradianceMap = irradiance_map;
    right_mat_comp.material->prefilterMap  = prefilter_map;

    auto &right_transform = reg.get<TransformComponent>(right_entity);
    right_transform.setPosition(glm::vec3(ground_size.x * 0.5f, ground_pos.y + ground_size.y * 0.5f + wall_height * 0.5f, 0.0f));
    right_transform.setScale(right_size);

    auto &right_rb        = m_scene->addComponent<RigidBodyComponent>(right_entity);
    right_rb.mass         = 0.0f;
    right_rb.colliderType = ColliderType::Box;
    right_rb.shapeSize    = right_size;

    m_scene->setEntityParent(ground_entity, play_ground_entity);
    m_scene->setEntityParent(front_entity, play_ground_entity);
    m_scene->setEntityParent(back_entity, play_ground_entity);
    m_scene->setEntityParent(left_entity, play_ground_entity);
    m_scene->setEntityParent(right_entity, play_ground_entity);

    // boxs
    int box_num = 30;
    for (int i = 0; i < box_num; i++)
    {
        auto box_entity = m_scene->createDefaultEntity("box_" + std::to_string(i));
        auto &mesh_comp = m_scene->addComponent<MeshComponent>(box_entity);
        mesh_comp.primitives.push_back(GeometryUtils::CreateCube(1.0f, 1.0f, 1));

        auto &mat_comp                   = m_scene->addComponent<MaterialComponent>(box_entity, MaterialType::PBR);
        mat_comp.material->color         = glm::vec3(3.0f / 255.0f, 108.0f / 255.0f, 193.0f / 255.0f);
        mat_comp.material->metallic      = 0.1f;
        mat_comp.material->roughness     = 0.6f;
        mat_comp.material->ao            = 1.0f;
        mat_comp.material->irradianceMap = irradiance_map;
        mat_comp.material->prefilterMap  = prefilter_map;

        // 随机位置
        auto &box_transform = reg.get<TransformComponent>(box_entity);
        box_transform.setScale(glm::vec3(glm::linearRand(2.0f, 10.0f)));
        box_transform.setPosition(glm::vec3(glm::linearRand(-ground_size.x / 3, ground_size.x / 3), box_transform.getScale().y * 0.5f,
                                            glm::linearRand(-ground_size.z / 2, ground_size.z / 2)));
        box_transform.setRotation(glm::angleAxis(glm::linearRand(0.0f, glm::two_pi<float>()), glm::vec3(0.0f, 1.0f, 0.0f)));

        auto &box_rb        = m_scene->addComponent<RigidBodyComponent>(box_entity);
        box_rb.mass         = 1.0f;
        box_rb.colliderType = ColliderType::Box;
        box_rb.shapeSize    = box_transform.getScale();

        m_scene->setEntityParent(box_entity, play_ground_entity);
    }

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

    auto &model_rb        = m_scene->addComponent<RigidBodyComponent>(model_entity);
    model_rb.mass         = 1.0f;
    model_rb.colliderType = ColliderType::Box;
    model_rb.shapeSize    = glm::vec3(1.1);

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

    auto &model_rb_2        = m_scene->addComponent<RigidBodyComponent>(model_entity_2);
    model_rb_2.mass         = 1.0f;
    model_rb_2.colliderType = ColliderType::Box;
    model_rb_2.shapeSize    = glm::vec3(1.1);

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

entt::entity Sandbox::createStairs(std::string name, glm::vec3 position, glm::vec3 size, glm::vec3 rotation, int step)
{
    auto entity = m_scene->createDefaultEntity(name);
    auto &reg   = m_scene->getRegistry();

    auto cube = GeometryUtils::CreateCube(size.x, size.y, size.z);

    for (int i = 0; i < step; i++)
    {
        auto stair_entity = m_scene->createDefaultEntity(name + "_stair_" + std::to_string(i));
        auto &mesh_comp   = m_scene->addComponent<MeshComponent>(stair_entity);
        mesh_comp.primitives.push_back(cube);
        auto &mat_comp               = m_scene->addComponent<MaterialComponent>(stair_entity, MaterialType::PBR);
        mat_comp.material->color     = glm::vec3(0.6f, 0.6f, 0.6f);
        mat_comp.material->metallic  = 0.0f;
        mat_comp.material->roughness = 0.5f;
        mat_comp.material->ao        = 1.0f;

        auto &stair_transform = reg.get<TransformComponent>(stair_entity);
        stair_transform.setPosition(glm::vec3(position.x, position.y + i * size.y, position.z));
        stair_transform.setRotation(rotation);

        auto &stair_rb        = m_scene->addComponent<RigidBodyComponent>(stair_entity);
        stair_rb.mass         = 0.0f;
        stair_rb.colliderType = ColliderType::Box;
        stair_rb.shapeSize    = size;

        m_scene->setEntityParent(stair_entity, entity);
    }

    return entity;
}

} // namespace Airwave
