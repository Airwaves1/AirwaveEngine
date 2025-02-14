#include "ecs/systems/render_system.hpp"
#include "rendering/renderer.hpp"
#include "core/application.hpp"

#include "ecs/components/camera_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/mesh_component.hpp"
#include "ecs/components/material_component.hpp"
#include "ecs/components/singleton_components/lights_manager_component.hpp"
#include "ecs/components/singleton_components/renderer_component.hpp"

namespace Airwave
{

void RenderSystem::onUpdate(float deltaTime)
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

void RenderSystem::renderBackground(Renderer *renderer, CameraComponent &camera)
{
    auto &reg           = m_scene->getRegistry();
    auto adminEntity    = m_scene->getAdminEntity();
    auto &renderer_comp = reg.get<RendererComponent>(adminEntity);

    if (renderer_comp.backgroundMap)
    {
        glDepthFunc(GL_LEQUAL);
        auto shader = renderer_comp.backgroundShader;
        if (!shader)
        {
            LOG_ERROR("RenderSystem::renderBackground: background shader is nullptr");
            return;
        }
        renderer->bindShader(shader->getHandle());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, renderer_comp.backgroundMap->getHandle());

        renderer->set("u_backgroundMap", 0);
        renderer->set("u_viewMatrix", camera.getWorldInverseMatrix());
        renderer->set("u_projectionMatrix", camera.getProjectionMatrix());
        renderer->uploadUniforms(shader->getHandle());

        renderer_comp.cube->draw();
    }
}

void RenderSystem::renderScene(CameraComponent &camera)
{
    auto renderer = m_scene->getApplication()->getRenderer();

    renderer->getFramebuffer()->bind();
    renderer->clear();
    renderer->enable(GL_DEPTH_TEST);

    auto &reg          = m_scene->getRegistry();
    auto renderObjects = reg.view<MaterialComponent, MeshComponent, TransformComponent>();
    auto adminEntity   = m_scene->getAdminEntity();
    auto &rendererComp = reg.get<RendererComponent>(adminEntity);

    auto &emptyMap           = rendererComp.emptyMap;
    auto &defaultNormal      = rendererComp.defaultNormal;
    auto &lightsManager_comp = reg.get<LightsManagerComponent>(adminEntity);

    for (auto entity : renderObjects)
    {
        auto &mat_comp       = reg.get<MaterialComponent>(entity);
        auto &mesh_comp      = reg.get<MeshComponent>(entity);
        auto &transform_comp = reg.get<TransformComponent>(entity);

        auto &tag_comp = reg.get<TagComponent>(entity);

        if (!mat_comp.materialRenderParams.visible) continue;

        auto shader = mat_comp.material->shader->getHandle();
        if (shader == 0) continue;

        renderer->bindShader(shader);

        renderer->set("u_worldMatrix", transform_comp.getWorldMatrix());
        renderer->set("u_viewMatrix", camera.getWorldInverseMatrix());
        renderer->set("u_projectionMatrix", camera.getProjectionMatrix());
        renderer->set("u_normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform_comp.getWorldMatrix()))));
        renderer->set("u_cameraPosition", camera.getCameraPosition());
        renderer->set("u_material.albedo", mat_comp.material->color);
        renderer->set("u_material.roughness", mat_comp.material->roughness);
        renderer->set("u_material.metallic", mat_comp.material->metallic);
        renderer->set("u_material.ao", mat_comp.material->ao);

        int slots = 0;

        // albedo, normal, roughness, metallic, ao
        if (mat_comp.material->albedoMap)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, mat_comp.material->albedoMap->getHandle());
        }
        else
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.albedoMap", slots);
        slots++;

        if (mat_comp.material->normalMap)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, mat_comp.material->normalMap->getHandle());
        }
        else
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, defaultNormal->getHandle());
        }
        renderer->set("u_material.normalMap", slots);
        slots++;

        if (mat_comp.material->metallicRoughnessMap)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, mat_comp.material->metallicRoughnessMap->getHandle());
        }
        else
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.metallicRoughnessMap", slots);
        slots++;

        if (mat_comp.material->roughnessMap)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, mat_comp.material->roughnessMap->getHandle());
        }
        else
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.roughnessMap", slots);
        slots++;

        if (mat_comp.material->metallicMap)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, mat_comp.material->metallicMap->getHandle());
        }
        else
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.metallicMap", slots);
        slots++;

        if (mat_comp.material->aoMap)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, mat_comp.material->aoMap->getHandle());
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
        if (mat_comp.material->irradianceMap)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mat_comp.material->irradianceMap->getHandle());
        }
        renderer->set("u_material.irradianceMap", slots);
        slots++;

        if (mat_comp.material->prefilterMap)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_CUBE_MAP, mat_comp.material->prefilterMap->getHandle());
        }
        renderer->set("u_material.prefilterMap", slots);
        slots++;

        if (mat_comp.material->brdfLUT)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, mat_comp.material->brdfLUT->getHandle());
        }
        renderer->set("u_material.brdf_lut", slots);
        slots++;

        renderer->set("u_lightCount", lightsManager_comp.lights.size());
        for (int i = 0; i < lightsManager_comp.lights.size(); i++)
        {
            auto light_entity = lightsManager_comp.lights[i];
            auto &lightComp   = reg.get<LightComponent>(light_entity);
            auto &lightTrans  = reg.get<TransformComponent>(light_entity);
            renderer->set("u_lights[" + std::to_string(i) + "].position", lightTrans.getPosition());
            renderer->set("u_lights[" + std::to_string(i) + "].color", lightComp.color);
            renderer->set("u_lights[" + std::to_string(i) + "].intensity", lightComp.intensity);
        }

        renderer->uploadUniforms(shader);

        for (auto &primitive : mesh_comp.primitives)
        {
            primitive->draw();
        }

        drawCalls++;
    }

    renderBackground(renderer, camera);
    renderer->getFramebuffer()->unbind();
}

} // namespace Airwave