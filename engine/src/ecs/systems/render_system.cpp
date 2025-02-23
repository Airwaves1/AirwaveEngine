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

    auto renderer = m_scene->getApplication()->getRenderer();

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

    // forwardRender(renderer, camera);
    deferredRender(renderer, camera);

    renderBackground(renderer, camera);

    renderer->getFramebuffer()->unbind();
}

void RenderSystem::forwardRender(Renderer *renderer, CameraComponent &camera)
{
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

        mat_comp.material->computeShaderKeyWord();

        auto shader = mat_comp.material->shader->getHandle();
        if (shader == 0) continue;

        renderer->bindShader(shader);

        renderer->set("u_worldMatrix", transform_comp.getWorldMatrix());
        renderer->set("u_viewMatrix", camera.getWorldInverseMatrix());
        renderer->set("u_projectionMatrix", camera.getProjectionMatrix());
        renderer->set("u_normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform_comp.getWorldMatrix()))));
        renderer->set("u_cameraPosition", camera.getCameraPosition());

        int slots = 0;

        // albedo, normal, roughness, metallic, ao
        uploadMaterialUniforms(renderer, mat_comp.material.get(), slots);

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
            renderer->set("u_material.prefilterMap", slots);
            slots++;
        }

        if (mat_comp.material->brdfLUT)
        {
            glActiveTexture(GL_TEXTURE0 + slots);
            glBindTexture(GL_TEXTURE_2D, mat_comp.material->brdfLUT->getHandle());
            renderer->set("u_material.brdf_lut", slots);
            slots++;
        }

        renderer->set("u_lightCount", static_cast<int>(lightsManager_comp.lights.size()));
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
            drawCalls++;
        }
    }
}

void RenderSystem::deferredRender(Renderer *renderer, CameraComponent &camera)
{
    auto &reg          = m_scene->getRegistry();
    auto adminEntity   = m_scene->getAdminEntity();
    auto &rendererComp = reg.get<RendererComponent>(adminEntity);

    auto &emptyMap      = rendererComp.emptyMap;
    auto &defaultNormal = rendererComp.defaultNormal;

    auto &lightsManager_comp = reg.get<LightsManagerComponent>(adminEntity);
    auto renderObjects       = reg.view<MaterialComponent, MeshComponent, TransformComponent>();

    auto g_buffer = renderer->getGBuffer();

    // geometry pass
    g_buffer->bind();

    renderer->enable(GL_DEPTH_TEST);
    renderer->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer->clear();

    auto g_buffer_shader = rendererComp.geometryPassShader->getHandle();
    if (g_buffer_shader == 0) return;
    renderer->bindShader(g_buffer_shader);

    renderer->set("u_viewMatrix", camera.getWorldInverseMatrix());
    renderer->set("u_projectionMatrix", camera.getProjectionMatrix());

    for (auto entity : renderObjects)
    {
        auto &tag_comp       = reg.get<TagComponent>(entity);
        auto &mesh_comp      = reg.get<MeshComponent>(entity);
        auto &mat_comp       = reg.get<MaterialComponent>(entity);
        auto &transform_comp = reg.get<TransformComponent>(entity);

        if (!mat_comp.materialRenderParams.visible) continue;

        renderer->set("u_worldMatrix", transform_comp.getWorldMatrix());
        renderer->set("u_normalMatrix", glm::transpose(glm::inverse(glm::mat3(transform_comp.getWorldMatrix()))));

        const auto &material = mat_comp.material;

        renderer->set("u_material.albedo", material->color);
        renderer->set("u_material.roughness", material->roughness);
        renderer->set("u_material.metallic", material->metallic);
        renderer->set("u_material.ao", material->ao);

        int slots = 0;

        // albedo
        glActiveTexture(GL_TEXTURE0 + slots);
        if (material->albedoMap && material->albedoMap->getHandle() != 0)
        {
            glBindTexture(GL_TEXTURE_2D, material->albedoMap->getHandle());
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.albedoMap", slots);
        slots++;

        // normal
        glActiveTexture(GL_TEXTURE0 + slots);
        if (material->normalMap && material->normalMap->getHandle() != 0)
        {
            glBindTexture(GL_TEXTURE_2D, material->normalMap->getHandle());
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, defaultNormal->getHandle());
        }
        renderer->set("u_material.normalMap", slots);
        slots++;

        // metallicRoughness
        glActiveTexture(GL_TEXTURE0 + slots);
        if (material->metallicRoughnessMap && material->metallicRoughnessMap->getHandle() != 0)
        {
            glBindTexture(GL_TEXTURE_2D, material->metallicRoughnessMap->getHandle());
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.metallicRoughnessMap", slots);
        slots++;

        // roughness
        glActiveTexture(GL_TEXTURE0 + slots);
        if (material->roughnessMap && material->roughnessMap->getHandle() != 0)
        {
            glBindTexture(GL_TEXTURE_2D, material->roughnessMap->getHandle());
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.roughnessMap", slots);
        slots++;

        // metallic
        glActiveTexture(GL_TEXTURE0 + slots);
        if (material->metallicMap && material->metallicMap->getHandle() != 0)
        {
            glBindTexture(GL_TEXTURE_2D, material->metallicMap->getHandle());
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.metallicMap", slots);
        slots++;

        // ao
        glActiveTexture(GL_TEXTURE0 + slots);
        if (material->aoMap && material->aoMap->getHandle() != 0)
        {
            glBindTexture(GL_TEXTURE_2D, material->aoMap->getHandle());
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, emptyMap->getHandle());
        }
        renderer->set("u_material.aoMap", slots);
        slots++;

        renderer->uploadUniforms(g_buffer_shader);
        for (auto &primitive : mesh_comp.primitives)
        {
            primitive->draw();

            drawCalls++;
        }

        for (int i = slots - 1; i >= 0; i--)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    renderer->bindShader(0);
    g_buffer->unbind();
    // end geometry pass

    // lighting pass
    renderer->getFramebuffer()->bind();

    renderer->enable(GL_DEPTH_TEST);
    renderer->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer->clear();

    auto lighting_pass_shader = rendererComp.lightingPassShader->getHandle();
    if (lighting_pass_shader == 0) return;

    renderer->bindShader(lighting_pass_shader);

    auto g_position = g_buffer->getColorAttachment(0);
    auto g_normal   = g_buffer->getColorAttachment(1);
    auto g_albedo   = g_buffer->getColorAttachment(2);
    auto g_material = g_buffer->getColorAttachment(3);

    if (g_position == nullptr || g_normal == nullptr || g_albedo == nullptr || g_material == nullptr)
    {
        LOG_ERROR("RenderSystem::deferredRender: g_buffer attachment is nullptr");
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_position->getHandle());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_normal->getHandle());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_albedo->getHandle());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, g_material->getHandle());

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, rendererComp.irradianceMap->getHandle());

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, rendererComp.prefilterMap->getHandle());

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, rendererComp.brdfLUT->getHandle());

    renderer->set("g_position", 0);
    renderer->set("g_normal", 1);
    renderer->set("g_albedo", 2);
    renderer->set("g_parameters", 3);
    renderer->set("u_irradianceMap", 4);
    renderer->set("u_prefilterMap", 5);
    renderer->set("u_brdf_lut", 6);

    renderer->set("u_lightCount", static_cast<int>(lightsManager_comp.lights.size()));
    for (int i = 0; i < lightsManager_comp.lights.size(); i++)
    {
        auto light_entity = lightsManager_comp.lights[i];
        auto &lightComp   = reg.get<LightComponent>(light_entity);
        auto &lightTrans  = reg.get<TransformComponent>(light_entity);
        // 0: Directional, 1: Point, 2: Spot
        renderer->set("u_lights[" + std::to_string(i) + "].type", static_cast<int>(lightComp.type));
        renderer->set("u_lights[" + std::to_string(i) + "].position", lightTrans.getPosition());
        renderer->set("u_lights[" + std::to_string(i) + "].color", lightComp.color);
        renderer->set("u_lights[" + std::to_string(i) + "].intensity", lightComp.intensity);
    }
    renderer->set("u_cameraPosition", camera.getCameraPosition());

    renderer->uploadUniforms(lighting_pass_shader);

    rendererComp.quad->draw();
    // end lighting pass

    // end deferred render

    // copy depth buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, g_buffer->getHandle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderer->getFramebuffer()->getHandle());

    const auto &src_spec = g_buffer->getSpecification();
    const auto &dst_spec = renderer->getFramebuffer()->getSpecification();
    glBlitFramebuffer(0, 0, src_spec.width, src_spec.height, 0, 0, dst_spec.width, dst_spec.height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->getFramebuffer()->getHandle());

}

void RenderSystem::uploadMaterialUniforms(Renderer *renderer, Material *material, int &slots)
{
    renderer->set("u_material.albedo", material->color);
    renderer->set("u_material.roughness", material->roughness);
    renderer->set("u_material.metallic", material->metallic);
    renderer->set("u_material.ao", material->ao);

    if (material->albedoMap)
    {
        glActiveTexture(GL_TEXTURE0 + slots);
        glBindTexture(GL_TEXTURE_2D, material->albedoMap->getHandle());
        renderer->set("u_material.albedoMap", slots);
        slots++;
    }

    if (material->normalMap)
    {
        glActiveTexture(GL_TEXTURE0 + slots);
        glBindTexture(GL_TEXTURE_2D, material->normalMap->getHandle());
        renderer->set("u_material.normalMap", slots);
        slots++;
    }

    if (material->metallicRoughnessMap)
    {
        glActiveTexture(GL_TEXTURE0 + slots);
        glBindTexture(GL_TEXTURE_2D, material->metallicRoughnessMap->getHandle());
        renderer->set("u_material.metallicRoughnessMap", slots);
        slots++;
    }

    if (material->roughnessMap)
    {
        glActiveTexture(GL_TEXTURE0 + slots);
        glBindTexture(GL_TEXTURE_2D, material->roughnessMap->getHandle());
        renderer->set("u_material.roughnessMap", slots);
        slots++;
    }

    if (material->metallicMap)
    {
        glActiveTexture(GL_TEXTURE0 + slots);
        glBindTexture(GL_TEXTURE_2D, material->metallicMap->getHandle());
        renderer->set("u_material.metallicMap", slots);
        slots++;
    }

    if (material->aoMap)
    {
        glActiveTexture(GL_TEXTURE0 + slots);
        glBindTexture(GL_TEXTURE_2D, material->aoMap->getHandle());
        renderer->set("u_material.aoMap", slots);
        slots++;
    }
}

} // namespace Airwave
