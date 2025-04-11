#pragma once

#include "function/ecs/system.hpp"
#include "function/ecs/world.hpp"

#include "function/ecs/component/camera_component.hpp"
#include "function/ecs/component/transform_component.hpp"
#include "function/ecs/component/mesh_component.hpp"

namespace Airwave
{
class RenderSystem : public System
{
  public:
    RenderSystem()           = default;
    ~RenderSystem() override = default;

    void onUpdate(std::weak_ptr<World> world, float delta_time) override
    {
        auto w = world.lock();
        if (!w)
        {
            return;
        }

        const auto &registry = w->getRegistry();
        auto camera_view     = registry.view<CameraComponent>();

        for (auto camera_entity : camera_view)
        {
            auto &camera_comp = camera_view.get<CameraComponent>(camera_entity);
            auto &transform   = registry.get<TransformComponent>(camera_entity);

            camera_comp.viewMatrix = transform.getWorldMatrixInverse();

            auto mesh_view = registry.view<MeshComponent, TransformComponent>();

            glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (auto mesh_entity : mesh_view)
            {
                auto &mesh           = mesh_view.get<MeshComponent>(mesh_entity);
                auto &mesh_transform = mesh_view.get<TransformComponent>(mesh_entity);

                // glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                // glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
                auto &view = camera_comp.getViewMatrix();
                auto &proj = camera_comp.getProjectionMatrix();

                // 设置着色器和纹理
                mesh.shader->bind();
                mesh.shader->setUniform("u_model", mesh_transform.getWorldMatrix());
                // mesh.shader->setUniform("u_view", view);
                // mesh.shader->setUniform("u_projection", proj);
                mesh.shader->setUniform("u_view", view);
                mesh.shader->setUniform("u_projection", proj);

                // 绘制网格
                mesh.shader->commitUniforms();
                // 绑定 VAO 和 EBO
                glBindVertexArray(mesh.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }
    }
};

} // namespace Airwave
