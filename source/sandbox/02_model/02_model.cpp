#include <iostream>
#include <memory>
#include "aw_engine.hpp"
#include <imgui.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Sandbox : public Airwave::Application
{
  public:
    void onInit() override
    {
        // ================================初始化场景和系统===================================
        // 创建场景
        m_scene = std::make_shared<Airwave::Scene>(Airwave::SceneSpecification("Sandbox"));

        // 添加相机系统
        auto cameraSystem = std::make_shared<Airwave::CameraSystem>();
        m_scene->addSystem(cameraSystem);

        // 添加渲染系统
        auto renderSystem = std::make_shared<Airwave::ForwardRenderSystem>();
        m_scene->addSystem(renderSystem);

        // 添加变换系统
        auto transformSystem = std::make_shared<Airwave::TransformSystem>();
        m_scene->addSystem(transformSystem);

        // 添加背景系统
        // std::array<std::string, 6> skyboxPaths = {
        //     TEXTURE_DIR "cube_textures/bridge2/posx.jpg",
        //     TEXTURE_DIR "cube_textures/bridge2/negx.jpg",
        //     TEXTURE_DIR "cube_textures/bridge2/posy.jpg",
        //     TEXTURE_DIR "cube_textures/bridge2/negy.jpg",
        //     TEXTURE_DIR "cube_textures/bridge2/posz.jpg",
        //     TEXTURE_DIR "cube_textures/bridge2/negz.jpg",
        // };
        // std::shared_ptr<Airwave::CubeTexture> skyboxTexture =
        //     std::make_shared<Airwave::CubeTexture>(skyboxPaths);
        auto backgroundSystem =
            std::make_shared<Airwave::BackgroundSystem>(glm::vec4(0.0, 0.0, 0.0, 1.0));
        m_scene->addSystem(backgroundSystem);
        //===============================================================================

        // ================================创建相机实体===================================
        auto cameraEntity = m_scene->createEntity("Camera");
        // 添加相机组件
        auto cameraComp = cameraEntity->addComponent<Airwave::CameraComponent>(
            Airwave::CameraType::Perspective, 45.0f,
            getWindow()->getWidth() / getWindow()->getHeight(), 0.1, 100.0);

        cameraComp.camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        // 添加控制器组件
        auto &controllerComp =
            cameraEntity->addComponent<Airwave::FPSCameraControllerComponent>(cameraComp.camera);
        controllerComp.controller->setSpeed(5.0f, 0.002f);
        //===============================================================================

        // ================================创建光源实体===================================
        // auto pointLightEntity = m_scene->createEntity("PointLight");
        // // 添加光源组件
        // auto &pointLightComp = pointLightEntity->addComponent<Airwave::LightComponent>(
        //     Airwave::LightType::Point, 1.0f, true);
        // pointLightComp.ambient   = glm::vec3(0.0f);
        // pointLightComp.diffuse   = glm::vec3(1.0f);
        // pointLightComp.specular  = glm::vec3(1.0f);
        // pointLightComp.position  = glm::vec3(0.0f, -8.0f, 0.0f);
        // pointLightComp.constant  = 1.0f;
        // pointLightComp.linear    = 0.09f;
        // pointLightComp.quadratic = 0.032f;

        auto dirLightEntity = m_scene->createEntity("DirLight");
        // 添加光源组件
        auto &dirLightComp = dirLightEntity->addComponent<Airwave::LightComponent>(
            Airwave::LightType::Directional, 1.0f, true);
        dirLightComp.ambient   = glm::vec3(0.2f);
        dirLightComp.diffuse   = glm::vec3(1.0f);
        dirLightComp.specular  = glm::vec3(1.0f);
        dirLightComp.direction = glm::vec3(-2.0f, -1.0f, -3.0f);
        // ===============================================================================

        // ================================创建平面实体===================================
        // 创建实体
        auto planeEntity = m_scene->createEntity("Plane");
        // 添加网格组件
        std::vector<Airwave::AWVertex> planeVertices;
        std::vector<uint32_t> planeIndices;
        Airwave::GeometryUtils::CreatePlane(planeVertices, planeIndices, 50.0f, 50.0f, 10, 10,10.0f, 10.0f);
        auto &planeMeshComp =
            planeEntity->addComponent<Airwave::MeshComponent>(planeVertices, planeIndices);
        // 添加材质组件
        auto &planeMaterialComp = planeEntity->addComponent<Airwave::MaterialComponent>(
            Airwave::MaterialType::BlinnPhong);
        planeMaterialComp.name      = "Plane";
        planeMaterialComp.ambient   = glm::vec3(0.2f, 0.2f, 0.2f);
        planeMaterialComp.diffuse   = glm::vec3(0.5f, 0.5f, 0.5f);
        planeMaterialComp.specular  = glm::vec3(0.5f, 0.5f, 0.5f);
        planeMaterialComp.shininess = 32.0f;
        Airwave::TextureSpecification spec;
        spec.internalFormat = Airwave::TextureInternalFormat::SRGB_ALPHA;
        planeMaterialComp.diffuseMap =
            Airwave::TEXTURE_LIB.load("floor_diffuse", TEXTURE_DIR "wood.png", spec);

        // 添加变换组件
        auto &planeTransformComp = planeEntity->addComponent<Airwave::TransformComponent>();
        planeTransformComp.setPosition(glm::vec3(0.0f, -10.0f, 0.0f));
        // planeTransformComp.setScale(glm::vec3(50.0f, 1.0f, 50.0f));
        // planeTransformComp.setRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f,
        // 0.0f)));
        //===============================================================================

        // ================================创建立方体实体===================================

        // 创建实体
        auto cubeEntity = m_scene->createEntity("Cube");
        // 添加网格组件
        std::vector<Airwave::AWVertex> vertices;
        std::vector<uint32_t> indices;
        Airwave::GeometryUtils::CreateCube(vertices, indices);
        auto &meshComp = cubeEntity->addComponent<Airwave::MeshComponent>(vertices, indices);
        // 添加材质组件
        auto &materialComp =
            cubeEntity->addComponent<Airwave::MaterialComponent>(Airwave::MaterialType::BlinnPhong);
        materialComp.ambient   = glm::vec3(1.0f, 0.5f, 0.31f);
        materialComp.diffuse   = glm::vec3(1.0f, 0.5f, 0.31f);
        materialComp.specular  = glm::vec3(0.5f, 0.5f, 0.5f);
        materialComp.shininess = 32.0f;
        materialComp.diffuseMap =
            Airwave::TEXTURE_LIB.load("container_diffuse", TEXTURE_DIR "container2.png", spec);
        materialComp.specularMap = Airwave::TEXTURE_LIB.load(
            "container_specular", TEXTURE_DIR "container2_specular.png", spec);

        // 添加变换组件
        auto &transformComp = cubeEntity->addComponent<Airwave::TransformComponent>();
        transformComp.setPosition(glm::vec3(-3.0f, 3.0f, -5.0f));
        //===============================================================================

        // ================================创建无人机模型实体===================================
        auto droneEntity = m_scene->createEntity("Drone");
        Airwave::ObjLoader::Load(droneEntity, MODEL_DIR "intergalactic_spaceship-obj/"
                                                        "Intergalactic_Spaceship-(Wavefront).obj");
        auto &droneTransformComp = droneEntity->addComponent<Airwave::TransformComponent>();
        droneTransformComp.setPosition(glm::vec3(-3.0f, 0.0f, -5.0f));

        // ===============================================================================

        // ================================创建纳米装甲===================================
        auto nanoEntity = m_scene->createEntity("Nano");
        // Airwave::ObjLoader::Load(nanoEntity, MODEL_DIR "nanosuit/nanosuit.obj");
        Airwave::ObjLoader::Load(nanoEntity, MODEL_DIR "Eiffel_Tower/10067_Eiffel_Tower_v1_max2010_it1.obj");
        auto &nanoTransformComp = nanoEntity->addComponent<Airwave::TransformComponent>();
        nanoTransformComp.setPosition(glm::vec3(3.0f, -3.0f, -5.0f));
        nanoTransformComp.setScale(glm::vec3(0.0005f));
        nanoTransformComp.setRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    }

    void onUpdate(float deltaTime) override
    {

        // auto pointLightEntity = m_scene->getEntity("PointLight");
        // auto &pointLightComp  = pointLightEntity->getComponent<Airwave::LightComponent>();

        // auto cameraEntity = m_scene->getEntity("Camera");
        // auto &cameraComp  = cameraEntity->getComponent<Airwave::CameraComponent>();

        // pointLightComp.position = cameraComp.camera->getPosition();

        m_scene->updateSystems(deltaTime);
    }

    void onRender() override {}

    void onImGuiRender() override
    {
        ImGui::Begin("Entity Settings");

        // 获取 "model_1" 实体
        if (auto entity = m_scene->getEntity("Nano"); entity)
        {
            // 获取变换组件
            Airwave::TransformComponent &transform =
                entity->getComponent<Airwave::TransformComponent>();

            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                // 分隔线
                ImGui::Separator();
                ImGui::Text("Position");

                // 显示并编辑位置
                glm::vec3 position = transform.getPosition();
                if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.01f))
                {
                    transform.setPosition(position); // 更新位置
                }

                ImGui::Separator();
                ImGui::Text("Rotation");

                // 显示并编辑旋转（转换为欧拉角以便编辑）
                glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(transform.getRotation()));
                if (ImGui::DragFloat3("##Rotation", glm::value_ptr(eulerRotation), 0.5f))
                {
                    transform.setRotation(glm::quat(glm::radians(eulerRotation))); // 更新旋转
                }

                ImGui::Separator();
                ImGui::Text("Scale");

                // 显示并编辑缩放
                glm::vec3 scale = transform.getScale();
                if (ImGui::DragFloat3("##Scale", glm::value_ptr(scale), 0.01f, 0.1f, 10.0f))
                {
                    transform.setScale(scale); // 更新缩放
                }
            }
        }
        else
        {
            ImGui::Text("Entity 'model_1' not found.");
        }

        // 点光源编辑界面
        // if (auto entity = m_scene->getEntity("PointLight"); entity)
        // {
        //     Airwave::LightComponent &light = entity->getComponent<Airwave::LightComponent>();

        //     if (ImGui::CollapsingHeader("Point Light Settings", ImGuiTreeNodeFlags_DefaultOpen))
        //     {
        //         // 分隔线
        //         ImGui::Separator();
        //         ImGui::Text("Position");

        //         // 显示并编辑位置
        //         glm::vec3 position = light.position;
        //         if (ImGui::DragFloat3("##Position", glm::value_ptr(position), 0.01f))
        //         {
        //             light.position = position; // 更新位置
        //         }

        //         ImGui::Separator();
        //         ImGui::Text("Ambient");

        //         // 显示并编辑环境光
        //         glm::vec3 ambient = light.ambient;
        //         if (ImGui::ColorEdit3("##Ambient", glm::value_ptr(ambient)))
        //         {
        //             light.ambient = ambient; // 更新环境光
        //         }

        //         ImGui::Separator();
        //         ImGui::Text("Diffuse");

        //         // 显示并编辑漫反射光
        //         glm::vec3 diffuse = light.diffuse;
        //         if (ImGui::ColorEdit3("##Diffuse", glm::value_ptr(diffuse)))
        //         {
        //             light.diffuse = diffuse; // 更新漫反射光
        //         }

        //         ImGui::Separator();
        //         ImGui::Text("Specular");

        //         // 显示并编辑镜面反射光
        //         glm::vec3 specular = light.specular;
        //         if (ImGui::ColorEdit3("##Specular", glm::value_ptr(specular)))
        //         {
        //             light.specular = specular; // 更新镜面反射光
        //         }

        //         ImGui::Separator();
        //         ImGui::Text("Constant");

        //         // 显示并编辑常数项
        //         float constant = light.constant;
        //         if (ImGui::DragFloat("##Constant", &constant, 0.01f))
        //         {
        //             light.constant = constant; // 更新常数项
        //         }

        //         ImGui::Separator();
        //         ImGui::Text("Linear");

        //         // 显示并编辑线性项
        //         float linear = light.linear;
        //         if (ImGui::DragFloat("##Linear", &linear, 0.01f))
        //         {
        //             light.linear = linear; // 更新线性项
        //         }

        //         ImGui::Separator();

        //         ImGui::Text("Quadratic");

        //         // 显示并编辑二次项
        //         float quadratic = light.quadratic;
        //         if (ImGui::DragFloat("##Quadratic", &quadratic, 0.01f))
        //         {
        //             light.quadratic = quadratic; // 更新二次项
        //         }

        //         ImGui::Separator();
        //         ImGui::Text("Intensity");

        //         // 显示并编辑强度
        //         float intensity = light.intensity;

        //         if (ImGui::DragFloat("##Intensity", &intensity, 0.01f))
        //         {
        //             light.intensity = intensity; // 更新强度
        //         }
        //     }
        // }else{
        //     ImGui::Text("Entity 'PointLight' not found.");
        // }

        // 平行光源编辑界面
        if (auto entity = m_scene->getEntity("DirLight"); entity)
        {
            Airwave::LightComponent &light = entity->getComponent<Airwave::LightComponent>();

            if (ImGui::CollapsingHeader("Directional Light Settings", ImGuiTreeNodeFlags_DefaultOpen))
            {
                // 分隔线
                ImGui::Separator();
                ImGui::Text("Direction");

                // 显示并编辑方向
                glm::vec3 direction = light.direction;
                if (ImGui::DragFloat3("##Direction", glm::value_ptr(direction), 0.01f))
                {
                    light.direction = direction; // 更新方向
                }

                ImGui::Separator();
                ImGui::Text("Ambient");

                // 显示并编辑环境光
                glm::vec3 ambient = light.ambient;
                if (ImGui::ColorEdit3("##Ambient", glm::value_ptr(ambient)))
                {
                    light.ambient = ambient; // 更新环境光
                }

                ImGui::Separator();
                ImGui::Text("Diffuse");

                // 显示并编辑漫反射光
                glm::vec3 diffuse = light.diffuse;
                if (ImGui::ColorEdit3("##Diffuse", glm::value_ptr(diffuse)))
                {
                    light.diffuse = diffuse; // 更新漫反射光
                }

                ImGui::Separator();
                ImGui::Text("Specular");

                // 显示并编辑镜面反射光
                glm::vec3 specular = light.specular;
                if (ImGui::ColorEdit3("##Specular", glm::value_ptr(specular)))
                {
                    light.specular = specular; // 更新镜面反射光
                }

                ImGui::Separator();
                ImGui::Text("Intensity");

                // 显示并编辑强度
                float intensity = light.intensity;

                if (ImGui::DragFloat("##Intensity", &intensity, 0.01f))
                {
                    light.intensity = intensity; // 更新强度
                }
            }
        }else{
            ImGui::Text("Entity 'DirLight' not found.");
        }

        ImGui::End();
    }

  private:
    std::shared_ptr<Airwave::Scene> m_scene;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }