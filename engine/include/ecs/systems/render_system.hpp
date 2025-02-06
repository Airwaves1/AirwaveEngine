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
            // auto camera_entity = m_scene->getEntity(entity);
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
            shader->setUniformMat4("u_viewMatrix", camera.getWorldInverseMatrix());
            shader->setUniformMat4("u_projectionMatrix", camera.getProjectionMatrix());
            shader->setUniformMat3("u_normalMatrix", glm::transpose(glm::inverse(
                                                       glm::mat3(transform.getWorldMatrix()))));
            shader->setUniformFloat3("u_cameraPosition", camera.getCameraPosition());

            shader->setUniformFloat3("u_material.albedo", material.color);
            shader->setUniformFloat("u_material.roughness", material.roughness);
            shader->setUniformFloat("u_material.metallic", material.metallic);
            shader->setUniformFloat("u_material.ao", material.ao);

            auto &lightsManagerComp = adminEntity->getComponent<LightsManagerComponent>();
            for (int i = 0; i < lightsManagerComp.lights.size(); i++)
            {
                auto &lightComp = lightsManagerComp.lights[i]->getComponent<LightComponent>();
                auto &lightTransform =
                    lightsManagerComp.lights[i]->getComponent<TransformComponent>();

                shader->setUniformFloat3("lightPositions[" + std::to_string(i) + "]",
                                         lightTransform.getPosition());

                shader->setUniformFloat3("lightColors[" + std::to_string(i) + "]", lightComp.color);
            }

            mesh.draw();
            drawCalls++;
            // shader->unbind();
        }

        renderer->getFramebuffer()->unbind();
    }
};
} // namespace Airwave

// if (adminEntity && adminEntity->hasComponent<LightsManagerComponent>())
// {
//     auto &lightsManager =
//     adminEntity->getComponent<LightsManagerComponent>();
//     shader->setUniformInt("u_lightCount", lightsManager.lights.size());
//     for (int i = 0; i < lightsManager.lights.size(); i++)
//     {
//         auto &lightComp =
//             lightsManager.lights[i]->getComponent<LightComponent>();
//         shader->setUniformInt("u_lights[" + std::to_string(i) + "].type",
//                               static_cast<int>(lightComp.type));
//         shader->setUniformFloat3("u_lights[" + std::to_string(i) +
//         "].position",
//                                  lightsManager.lights[i]
//                                      ->getComponent<TransformComponent>()
//                                      .getPosition());
//         shader->setUniformFloat("u_lights[" + std::to_string(i) +
//         "].intensity",
//                                 lightComp.intensity);
//         shader->setUniformFloat3("u_lights[" + std::to_string(i) + "].color",
//                                  lightComp.color);
//     }
// }