#include <iostream>
#include <memory>
#include "aw_engine.hpp"
#include <imgui.h>

#include <glad/glad.h>

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

        // 添加背景系统
        std::array<std::string, 6> skyboxPaths = {
            TEXTURE_DIR "cube_textures/bridge2/posx.jpg",
            TEXTURE_DIR "cube_textures/bridge2/negx.jpg",
            TEXTURE_DIR "cube_textures/bridge2/posy.jpg",
            TEXTURE_DIR "cube_textures/bridge2/negy.jpg",
            TEXTURE_DIR "cube_textures/bridge2/posz.jpg",
            TEXTURE_DIR "cube_textures/bridge2/negz.jpg",
        };
        std::shared_ptr<Airwave::CubeTexture> skyboxTexture =
            std::make_shared<Airwave::CubeTexture>(skyboxPaths);
        auto backgroundSystem = std::make_shared<Airwave::BackgroundSystem>(skyboxTexture);
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
        auto controllerComp =
            cameraEntity->addComponent<Airwave::FPSCameraControllerComponent>(cameraComp.camera);
        //===============================================================================

        // ================================创建光源实体===================================
        auto pointLightEntity = m_scene->createEntity("PointLight");
        // 添加光源组件
        auto &pointLightComp = pointLightEntity->addComponent<Airwave::LightComponent>(
            Airwave::LightType::Point, 1.0f, true);
        pointLightComp.ambient   = glm::vec3(0.2f);
        pointLightComp.diffuse   = glm::vec3(1.0f);
        pointLightComp.specular  = glm::vec3(1.0f);
        pointLightComp.position  = glm::vec3(0.0f, 3.0f, 2.0f);
        pointLightComp.constant  = 1.0f;
        pointLightComp.linear    = 0.09f;
        pointLightComp.quadratic = 0.032f;

        auto dirLightEntity = m_scene->createEntity("DirLight");
        // 添加光源组件
        auto &dirLightComp = dirLightEntity->addComponent<Airwave::LightComponent>(
            Airwave::LightType::Directional, 1.0f, true);
        dirLightComp.ambient   = glm::vec3(0.2f);
        dirLightComp.diffuse   = glm::vec3(1.0f);
        dirLightComp.specular  = glm::vec3(1.0f);
        dirLightComp.direction = glm::vec3(-2.0f, -1.0f, -3.0f);
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
        auto &materialComp      = cubeEntity->addComponent<Airwave::MaterialComponent>(Airwave::MaterialType::BlinnPhong);
        materialComp.ambient    = glm::vec3(1.0f, 0.5f, 0.31f);
        materialComp.diffuse    = glm::vec3(1.0f, 0.5f, 0.31f);
        materialComp.specular   = glm::vec3(0.5f, 0.5f, 0.5f);
        materialComp.shininess  = 32.0f;
        materialComp.diffuseMap = Airwave::TEXTURE_LIB.load(
            "container_diffuse", TEXTURE_DIR "container2.png", Airwave::TextureSpecification());
        materialComp.specularMap =
            Airwave::TEXTURE_LIB.load("container_specular", TEXTURE_DIR "container2_specular.png",
                                      Airwave::TextureSpecification());

        // 添加变换组件
        auto transformComp = cubeEntity->addComponent<Airwave::TransformComponent>();

        //===============================================================================
    }

    void onUpdate(float deltaTime) override
    {

        auto pointLightEntity = m_scene->getEntity("PointLight");
        auto &pointLightComp  = pointLightEntity->getComponent<Airwave::LightComponent>();

        auto cameraEntity = m_scene->getEntity("Camera");
        auto &cameraComp  = cameraEntity->getComponent<Airwave::CameraComponent>();

        pointLightComp.position = cameraComp.camera->getPosition();

        m_scene->updateSystems(deltaTime);
    }

    void onRender() override {}

    void onImGuiRender() override
    {
        ImGui::Begin("Triangle");
        ImGui::Text("Hello, Triangle!");
        ImGui::End();
    }

  private:
    std::shared_ptr<Airwave::Scene> m_scene;
};

Airwave::Application *CreateAirwaveEngineApplication() { return new Sandbox(); }