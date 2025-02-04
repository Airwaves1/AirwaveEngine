#pragma once

#include "ecs/aw_system.hpp"

#include <glm/glm.hpp>

#include "ecs/components/camera_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/mesh_component.hpp"
#include "ecs/components/material_component.hpp"
#include "ecs/components/singleton_components/lights_manager_component.hpp"

namespace Airwave
{
class RenderSystem : public AwSystem
{
  public:
    RenderSystem()  = default;
    ~RenderSystem() = default;

  protected:
    void onUpdate(float deltaTime) override
    {
        auto &reg       = m_scene->getRegistry();
        auto cameraView = reg.view<CameraComponent, TransformComponent>();

        drawCalls = 0;
        for (auto entity : cameraView)
        {
            auto &camera = cameraView.get<CameraComponent>(entity);
            if (!camera.enable)
            {
                continue;
            }
            renderScene(camera);
        }
        auto renderer       = m_scene->getApplication()->getRenderer();
        renderer->drawCalls = drawCalls;
    }

  private:
    uint64_t drawCalls = 0;

    void renderScene(CameraComponent &camera)
    {
        auto renderer = m_scene->getApplication()->getRenderer();

        renderer->getFramebuffer()->bind();
        // renderer->setClearColor(glm::vec3(0.2f, 0.8f, 0.3f));
        renderer->clear();
        renderer->enable(GL_DEPTH_TEST);

        auto &reg          = m_scene->getRegistry();
        auto renderObjects = reg.view<MaterialComponent, MeshComponent, TransformComponent>();
        auto adminEntity   = m_scene->getAdminEntity();

        for (auto entity : renderObjects)
        {
            auto awEntity   = m_scene->getEntity(entity);
            auto &material  = awEntity->getComponent<MaterialComponent>();
            auto &mesh      = awEntity->getComponent<MeshComponent>();
            auto &transform = awEntity->getComponent<TransformComponent>();

            if (!material.materialRenderParams.visible) continue;

            auto shader = material.shader;
            if (shader == nullptr) continue;

            shader->bind();

            shader->setUniformMat4("u_worldMatrix", transform.getWorldMatrix());
            shader->setUniformMat4("u_cameraWorldInverse", camera.getWorldInverseMatrix());
            shader->setUniformMat4("u_projectionMatrix", camera.getProjectionMatrix());
            shader->setUniformFloat3("u_cameraPosition", camera.getCameraPosition());

            // 深度测试
            if (material.materialRenderParams.depthTest)
            {
                renderer->enable(GL_DEPTH_TEST);
                renderer->setDepthFunc(material.materialRenderParams.depthFunc);
            }
            else
            {
                renderer->disable(GL_DEPTH_TEST);
            }

            // 深度写入
            renderer->setDepthMask(material.materialRenderParams.depthWrite);

            // 面剔除
            renderer->setCullFace(material.materialRenderParams.cullFace);

            // glEnable(GL_DEPTH_TEST);

            int slot = 0;
            shader->setUniformFloat3("u_color", material.color);
            if (material.diffuseMap)
            {
                material.diffuseMap->bind(slot);
                shader->setUniformInt("u_diffuseMap", slot);
                slot++;
            }
            switch (material.m_type)
            {
                case MaterialType::Basic:

                    break;
                case MaterialType::Phong:
                    break;
                case MaterialType::PBR:
                    // material
                    shader->setUniformFloat("u_material.roughness", material.roughness);
                    shader->setUniformFloat("u_material.metallic", material.metallic);

                    // light
                    if (adminEntity && adminEntity->hasComponent<LightsManagerComponent>())
                    {
                        auto &lightsManager = adminEntity->getComponent<LightsManagerComponent>();
                        shader->setUniformInt("u_lightCount", lightsManager.lights.size());
                        for (int i = 0; i < lightsManager.lights.size(); i++)
                        {
                            auto &lightComp =
                                lightsManager.lights[i]->getComponent<LightComponent>();
                            shader->setUniformInt("u_lights[" + std::to_string(i) + "].type",
                                                  static_cast<int>(lightComp.type));
                            shader->setUniformFloat3("u_lights[" + std::to_string(i) + "].position",
                                                     lightsManager.lights[i]
                                                         ->getComponent<TransformComponent>()
                                                         .getPosition());
                            shader->setUniformFloat("u_lights[" + std::to_string(i) + "].intensity",
                                                    lightComp.intensity);
                            shader->setUniformFloat3("u_lights[" + std::to_string(i) +
                                                         "].color",
                                                     lightComp.color);
                        }
                    }
                    break;
                default:
                    break;
            }

            mesh.draw();
            drawCalls++;

            if (material.diffuseMap)
            {
                material.diffuseMap->unbind();
            }
        }

        renderer->getFramebuffer()->unbind();
    }
};
} // namespace Airwave