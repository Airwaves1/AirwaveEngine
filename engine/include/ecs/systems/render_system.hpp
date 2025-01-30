#pragma once

#include "ecs/aw_system.hpp"

#include <glm/glm.hpp>

#include "ecs/components/camera_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/mesh_component.hpp"
#include "ecs/components/material_component.hpp"

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
        for (auto entity : cameraView)
        {
            auto &camera = cameraView.get<CameraComponent>(entity);
            if (!camera.enable)
            {
                continue;
            }
            renderScene(camera);
        }
    }

  private:
    void renderScene(CameraComponent &camera)
    {
        auto renderer = m_scene->getApplication()->getRenderer();

        renderer->getFramebuffer()->bind();
        renderer->setClearColor(glm::vec3(0.2f, 0.8f, 0.3f));
        renderer->clear();
        renderer->enable(GL_DEPTH_TEST);

        auto &reg          = m_scene->getRegistry();
        auto renderObjects = reg.view<MaterialComponent, MeshComponent, TransformComponent>();

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
            switch (material.m_type)
            {
                case MaterialType::Basic:
                    shader->setUniformFloat3("u_color", material.color);
                    if (material.diffuseMap)
                    {
                        material.diffuseMap->bind(slot);
                        shader->setUniformInt("u_diffuseMap", slot);
                        slot++;
                    }
                    break;
                case MaterialType::Phong:
                    break;
                case MaterialType::PBR:
                    break;
                default:
                    break;
            }

            mesh.draw();

            if (material.diffuseMap)
            {
                material.diffuseMap->unbind();
            }
        }

        renderer->getFramebuffer()->unbind();
    }
};
} // namespace Airwave