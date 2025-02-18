#pragma once

#include "panels/panel.hpp"
#include <imgui.h>
#include "rendering/texture.hpp"
#include "ecs/components/singleton_components/input_component.hpp"
#include "ecs/components/singleton_components/physics_world_component.hpp"
#include "ecs/components/camera_component.hpp"
#include "ecs/components/rigid_body_component.hpp"

namespace Airwave
{
class ViewportPanel : public Panel
{
  public:
    ViewportPanel(Editor *editor, const std::string &title, bool startOpened = true) : Panel(editor, title, startOpened) {}

    void onImGuiRender() override
    {
        auto app      = m_editor->getContext();
        auto scene    = app->getScene();
        auto renderer = app->getRenderer();
        auto &reg     = app->getScene()->getRegistry();
        auto admin    = app->getAdminEntity();

        ImGui::Begin(m_title.c_str());
        ImVec2 availableSize = ImGui::GetContentRegionAvail(); // 获取当前窗口可用大小
        auto fbo             = m_editor->getContext()->getRenderer()->getFramebuffer();
        auto textureID       = fbo->getColorAttachment()->getHandle();

        ImGui::Image((void *)(intptr_t)textureID, availableSize, ImVec2(0, 1), ImVec2(1, 0));

        // 如果窗口大小发生变化，更新反应区域
        ImVec2 itemRectMin     = ImGui::GetItemRectMin(); // 获取当前控件的左上角位置, 以窗口左上角为原点
        ImVec2 itemRectMax     = ImGui::GetItemRectMax(); // 获取当前控件的右下角位置, 以窗口左上角为原点
        ImVec2 mainViewportPos = ImGui::GetMainViewport()->Pos;
        ImVec2 viewMin         = ImVec2(itemRectMin.x - mainViewportPos.x, itemRectMin.y - mainViewportPos.y);
        ImVec2 viewMax         = ImVec2(itemRectMax.x - mainViewportPos.x, itemRectMax.y - mainViewportPos.y);



        //
        auto camera_view   = reg.view<CameraComponent, TransformComponent>();
        auto camera_entity = camera_view.front();
        auto &camera_comp  = scene->getComponent<CameraComponent>(camera_entity);

        // 更新渲染器视口
        camera_comp.setAspectRatio(availableSize.x / availableSize.y);
        renderer->setViewport(0.0, 0.0, availableSize.x, availableSize.y);
        renderer->getGBuffer()->resize(availableSize.x, availableSize.y);
        renderer->getFramebuffer()->resize(availableSize.x, availableSize.y);

        if (reg.all_of<InputComponent>(admin))
        {
            auto &input     = reg.get<InputComponent>(admin);
            input.reactArea = glm::vec4(viewMin.x, viewMin.y, viewMax.x, viewMax.y);
        }

        // 射线检测
        if (ImGui::IsItemHovered())
        {

            auto physics_comp = scene->getComponent<PhysicsWorldComponent>(admin);
            // 检测鼠标点击
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                ImVec2 imgui_mousePos  = ImGui::GetMousePos();  // 获取鼠标的屏幕坐标
                ImVec2 local_pos = ImVec2(imgui_mousePos.x - itemRectMin.x, imgui_mousePos.y - itemRectMin.y);

                LOG_DEBUG("imgui_mousePos: {0}, {1}", imgui_mousePos.x, imgui_mousePos.y);
                LOG_DEBUG("localPos: {0}, {1}", local_pos.x , local_pos.y);

                glm::vec2 mousePos = glm::vec2(local_pos.x, local_pos.y);

                auto ray           = camera_comp.screenPointToRay(mousePos, glm::vec4(0, 0, availableSize.x, availableSize.y));
                auto &physics_comp = scene->getComponent<PhysicsWorldComponent>(admin);

                auto ray_info = physics_comp.raycast(ray.first, ray.second, 1000.0f);
                if (ray_info.hasHit)
                {
                    auto entity = ray_info.entity;
                    auto &rigid = reg.get<RigidBodyComponent>(entity);
                    auto &tag   = reg.get<TagComponent>(entity);

                    LOG_INFO("Hit entity: {0}, tag: {1}", static_cast<uint32_t>(entity), tag.name);
                }
            }
        }

        ImGui::End();
    }

  private:
    glm::vec4 m_lastSize = glm::vec4(0.0f, 0.0f, 1200.0f, 800.0f); // 反应区域
};

} // namespace Airwave
