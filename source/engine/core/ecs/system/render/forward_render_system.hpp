#pragma once

#include "ecs/system/aw_system.hpp"
#include "material_system.hpp"
#include "light_system.hpp"
#include "backgroud_system.hpp"

#include "ecs/system/transform/transform_system.hpp"

#include "ecs/component/mesh/mesh_component.hpp"
#include "ecs/component/material/material_component.hpp"
#include "resource/shader_lib.hpp"
#include "resource/texture_lib.hpp"
#include "opengl/gl_framebuffer.hpp"
#include "render/full_screen_quad.hpp"
#include <glad/glad.h>

namespace Airwave
{
class ForwardRenderSystem : public AwSystem
{
  public:
    ForwardRenderSystem()
    {
        FramebufferSpecification spec;
        spec.enableMSAA          = true;
        spec.samples             = 4;
        spec.colorInternalFormat = TextureInternalFormat::RGBA16F;
        spec.colorFormat         = TextureFormat::RGBA;
        m_framebuffer            = std::make_shared<Framebuffer>(1400, 800, spec);

        m_fullScreenQuad = std::make_shared<FullScreenQuad>();

        m_eventObserver = std::make_shared<EventObserver>();
        m_eventObserver->onEvent<WindowResizeEvent>(
            [this](const WindowResizeEvent &e)
            {
                // LOG_DEBUG("WindowResizeEvent: {0},{1}", e.getWindowWidth(), e.getWindowHeight());
                m_framebuffer->resize(e.getWindowWidth(), e.getWindowHeight());
            });

        m_materialSystem = std::make_shared<MaterialSystem>();
    }

    ~ForwardRenderSystem() {}

    void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) override
    {
        if (!scene->hasSystem<LightSystem>())
        {
            m_lightSystem = std::make_shared<LightSystem>();
            scene->addSystem(m_lightSystem);
        }
        else
        {
            m_lightSystem = scene->getSystem<LightSystem>();
        }

        auto cameraView = scene->getRegistry()->view<CameraComponent>();

        for (auto entity : cameraView)
        {
            auto &cameraComp = cameraView.get<CameraComponent>(entity);

            renderScene(scene, cameraComp, deltaTime);
        }
    }

    void renderScene(const std::shared_ptr<Scene> &scene, const CameraComponent &cameraComp,
                     float deltaTime = 0.0f)
    {
        m_framebuffer->bind();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        if (!scene->hasSystem<BackgroundSystem>())
        {
            m_backgroundSystem =
                std::make_shared<BackgroundSystem>(glm::vec4(0.6f, 0.2f, 0.2f, 1.0f));
            scene->addSystem(m_backgroundSystem);
        }
        else
        {
            m_backgroundSystem = scene->getSystem<BackgroundSystem>();
        }
        m_backgroundSystem->render(cameraComp.camera);

        auto matMesh = scene->getRegistry()->view<MeshComponent, MaterialComponent>();
        for (auto entity : matMesh)
        {
            auto &mesh     = matMesh.get<MeshComponent>(entity);
            auto &material = matMesh.get<MaterialComponent>(entity);

            glm::mat4 modelMatrix = glm::mat4(1.0f);
            if (auto transform = scene->getRegistry()->try_get<TransformComponent>(entity))
            {
                modelMatrix = transform->getTransformMatrix();
            }

            m_materialSystem->applyMaterial(material, cameraComp, modelMatrix,
                                                 m_lightSystem->getLights(), deltaTime);

            mesh.draw();
        }

        m_framebuffer->unbind();

        m_fullScreenQuad->setRenderTarget(m_framebuffer);
        m_fullScreenQuad->renderToScreen();
    }

    void printMatrix(const glm::mat4 &matrix)
    {
        for (int i = 0; i < 4; ++i)
        {
            std::cout << "| ";
            for (int j = 0; j < 4; ++j)
            {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << "|\n";
        }
    }

  private:
    std::shared_ptr<EventObserver> m_eventObserver;
    std::shared_ptr<Framebuffer> m_framebuffer;
    std::shared_ptr<FullScreenQuad> m_fullScreenQuad;

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<MeshComponent> m_mesh;

    std::shared_ptr<MaterialSystem> m_materialSystem;
    std::shared_ptr<LightSystem> m_lightSystem;
    std::shared_ptr<BackgroundSystem> m_backgroundSystem;
};

} // namespace Airwave
