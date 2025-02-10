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

    void renderBackground(Renderer *renderer, CameraComponent &camera)
    {
        auto &reg          = m_scene->getRegistry();
        auto adminEntity    = m_scene->getAdminEntity();
        auto &renderer_comp = reg.get<RendererComponent>(adminEntity);

        if (renderer_comp.backgroundMap)
        {
            glDepthFunc(GL_LEQUAL);
            auto shader = renderer_comp.backgroundShader;

            renderer->bindShader(shader->getHandle());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, renderer_comp.backgroundMap->getHandle());

            renderer->set("u_backgroundMap", 0);
            renderer->set("u_viewMatrix", camera.getWorldInverseMatrix());
            renderer->set("u_projectionMatrix", camera.getProjectionMatrix());
            renderer->uploadUniforms(shader->getHandle());

            renderer_comp.mesh->draw();
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
        auto &rendererComp = reg.get<RendererComponent>(adminEntity);

        auto &emptyMap      = rendererComp.emptyMap;
        auto &defaultNormal = rendererComp.defaultNormal;

        for (auto entity : renderObjects)
        {
            auto &material          = reg.get<MaterialComponent>(entity);
            auto &mesh              = reg.get<MeshComponent>(entity);
            auto &transform         = reg.get<TransformComponent>(entity);
            auto &lightsManagerComp = reg.get<LightsManagerComponent>(adminEntity);

            if (!material.materialRenderParams.visible) continue;

            auto shader = material.shader->getHandle();
            if (shader == 0) continue;

            renderer->bindShader(shader);

            renderer->set("u_worldMatrix", transform.getWorldMatrix());
            renderer->set("u_viewMatrix", camera.getWorldInverseMatrix());
            renderer->set("u_projectionMatrix", camera.getProjectionMatrix());
            renderer->set("u_normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform.getWorldMatrix()))));
            renderer->set("u_cameraPosition", camera.getCameraPosition());
            renderer->set("u_material.albedo", material.color);
            renderer->set("u_material.roughness", material.roughness);
            renderer->set("u_material.metallic", material.metallic);
            renderer->set("u_material.ao", material.ao);

            int slots = 0;

            // albedo, normal, roughness, metallic, ao
            if (material.albedoMap)
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, material.albedoMap->getHandle());
            }
            else
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
            }
            renderer->set("u_material.albedoMap", slots);
            slots++;

            if (material.normalMap)
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, material.normalMap->getHandle());
            }
            else
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, defaultNormal->getHandle());
            }
            renderer->set("u_material.normalMap", slots);
            slots++;

            if (material.roughnessMap)
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, material.roughnessMap->getHandle());
            }
            else
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
            }
            renderer->set("u_material.roughnessMap", slots);
            slots++;

            if (material.metallicMap)
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, material.metallicMap->getHandle());
            }
            else
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
            }
            renderer->set("u_material.metallicMap", slots);
            slots++;

            if (material.aoMap)
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, material.aoMap->getHandle());
            }
            else
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
            }
            renderer->set("u_material.aoMap", slots);
            slots++;
            // end ----------------

            // lights
            if(material.irradianceMap)
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_CUBE_MAP, material.irradianceMap->getHandle());
            }
            renderer->set("u_material.irradianceMap", slots);
            slots++;

            if(material.prefilterMap)
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_CUBE_MAP, material.prefilterMap->getHandle());
            }
            renderer->set("u_material.prefilterMap", slots);
            slots++;

            if(material.brdfLUT)
            {
                glActiveTexture(GL_TEXTURE0 + slots);
                glBindTexture(GL_TEXTURE_2D, material.brdfLUT->getHandle());
            }
            renderer->set("u_material.brdf_lut", slots);
            slots++;


            renderer->set("u_lightCount", lightsManagerComp.lights.size());
            for (int i = 0; i < lightsManagerComp.lights.size(); i++)
            {
                auto light_entity = lightsManagerComp.lights[i];
                auto &lightComp   = reg.get<LightComponent>(light_entity);
                auto &lightTrans  = reg.get<TransformComponent>(light_entity);
                renderer->set("u_lights[" + std::to_string(i) + "].position", lightTrans.getPosition());
                renderer->set("u_lights[" + std::to_string(i) + "].color", lightComp.color);
                renderer->set("u_lights[" + std::to_string(i) + "].intensity", lightComp.intensity);
            }

            renderer->uploadUniforms(shader);
            if(mesh.mesh)
            {
                mesh.mesh->draw();
            }

            drawCalls++;
        }

        renderBackground(renderer, camera);
        renderer->getFramebuffer()->unbind();
    }
};
} // namespace Airwave