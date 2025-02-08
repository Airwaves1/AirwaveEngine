#pragma once

#include "ecs/aw_system.hpp"

#include <glm/glm.hpp>

#include "ecs/components/camera_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/mesh_component.hpp"
#include "ecs/components/material_component.hpp"
#include "ecs/components/singleton_components/lights_manager_component.hpp"
#include "ecs/components/singleton_components/renderer_component.hpp"

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

    void renderBackground(CameraComponent &camera)
    {
        auto renderer       = m_scene->getApplication()->getRenderer();
        auto adminEntity    = m_scene->getAdminEntity();
        auto &renderer_comp = adminEntity->getComponent<RendererComponent>();
        if (renderer_comp.backgroundMap)
        {
            glDepthFunc(GL_LEQUAL);
            auto shader = renderer_comp.backgroundShader;
            shader->bind();
            renderer_comp.backgroundMap->bind();
            shader->setUniformInt("u_backgroundMap", 0);
            shader->setUniformMat4("u_viewMatrix", camera.getWorldInverseMatrix());
            shader->setUniformMat4("u_projectionMatrix", camera.getProjectionMatrix());
            renderer_comp.meshComp->draw();
            renderer_comp.backgroundMap->unbind();
        }
    }

    void renderScene(CameraComponent &camera)
    {
        auto renderer = m_scene->getApplication()->getRenderer();

        renderer->getFramebuffer()->bind();
        renderer->clear();
        renderer->enable(GL_DEPTH_TEST);

        auto &reg          = m_scene->getRegistry();
        auto renderObjects = reg.view<MaterialComponent, MeshComponent, TransformComponent>();
        auto adminEntity   = m_scene->getAdminEntity();
        auto &rendererComp = adminEntity->getComponent<RendererComponent>();

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

            auto &lightsManagerComp = adminEntity->getComponent<LightsManagerComponent>();
            shader->setUniformInt("u_lightCount", lightsManagerComp.lights.size());
            for (int i = 0; i < lightsManagerComp.lights.size(); i++)
            {
                auto &lightComp = lightsManagerComp.lights[i]->getComponent<LightComponent>();
                auto &lightTransform =
                    lightsManagerComp.lights[i]->getComponent<TransformComponent>();

                shader->setUniformFloat3("u_lights[" + std::to_string(i) + "].position",
                                         lightTransform.getPosition());
                shader->setUniformFloat3("u_lights[" + std::to_string(i) + "].color",
                                         lightComp.color);
                shader->setUniformFloat("u_lights[" + std::to_string(i) + "].intensity",
                                        lightComp.intensity);
            }

            shader->setUniformFloat3("u_material.albedo", material.color);
            shader->setUniformFloat("u_material.roughness", material.roughness);
            shader->setUniformFloat("u_material.metallic", material.metallic);
            shader->setUniformFloat("u_material.ao", material.ao);

            int slots = 0;

            // bind textures
            if (material.albedoMap)
                material.albedoMap->bind(slots);
            else
                ResourceManager::GetInstance().getTexture("empty")->bind(slots);
            shader->setUniformInt("u_material.albedoMap", slots);
            slots++;

            if (material.normalMap)
                material.normalMap->bind(slots);
            else
                ResourceManager::GetInstance().getTexture("defaultNormal")->bind(slots);
            shader->setUniformInt("u_material.normalMap", slots);
            slots++;

            if (material.metallicMap)
                material.metallicMap->bind(slots);
            else
                ResourceManager::GetInstance().getTexture("empty")->bind(slots);
            shader->setUniformInt("u_material.metallicMap", slots);
            slots++;

            if (material.roughnessMap)
                material.roughnessMap->bind(slots);
            else
                ResourceManager::GetInstance().getTexture("empty")->bind(slots);
            shader->setUniformInt("u_material.roughnessMap", slots);
            slots++;

            if (material.aoMap)
                material.aoMap->bind(slots);
            else
                ResourceManager::GetInstance().getTexture("empty")->bind(slots);
            shader->setUniformInt("u_material.aoMap", slots);
            slots++;

            // 辐照度贴图
            if (material.irradianceMap)
            {
                material.irradianceMap->bind(slots);
                shader->setUniformInt("u_material.irradianceMap", slots);
                slots++;
            }

            mesh.draw();

            for (int i = 0; i < slots; i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            drawCalls++;
        }

        renderBackground(camera);
        renderer->getFramebuffer()->unbind();
    }
};
} // namespace Airwave