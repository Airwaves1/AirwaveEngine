#pragma once

#include "panels/panel.hpp"
#include <imgui.h>
#include "rendering/texture.hpp"
#include "ecs/components/singleton_components/input_component.hpp"
#include "ecs/components/singleton_components/physics_world_component.hpp"
#include "ecs/components/camera_component.hpp"
#include "ecs/components/rigid_body_component.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>

#include "ImGuizmo/ImGuizmo.h"

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

        // // 如果窗口大小发生变化，更新反应区域
        ImVec2 itemRectMin     = ImGui::GetItemRectMin();       // 获取当前控件的左上角位置, 以窗口左上角为原点
        ImVec2 itemRectMax     = ImGui::GetItemRectMax();       // 获取当前控件的右下角位置, 以窗口左上角为原点
        ImVec2 mainViewportPos = ImGui::GetMainViewport()->Pos; // 获取主视口的位置
        ImVec2 viewMin = ImVec2(itemRectMin.x - mainViewportPos.x, itemRectMin.y - mainViewportPos.y); // 获取当前控件的左上角位置,
                                                                                                       // 以主视口左上角为原点
        ImVec2 viewMax = ImVec2(itemRectMax.x - mainViewportPos.x, itemRectMax.y - mainViewportPos.y); // 获取当前控件的右下角位置,
                                                                                                       // 以主视口左上角为原点

        // //
        auto camera_view   = reg.view<CameraComponent, TransformComponent>();
        auto camera_entity = camera_view.front();
        auto &camera_comp  = scene->getComponent<CameraComponent>(camera_entity);

        // 更新渲染器视口
        camera_comp.setAspectRatio(availableSize.x / availableSize.y);
        renderer->setViewport(0.0, 0.0, availableSize.x, availableSize.y);
        renderer->getGBuffer()->resize(availableSize.x, availableSize.y);
        renderer->getFramebuffer()->resize(availableSize.x, availableSize.y);

        // 更新输入组件的反应区域
        if (reg.all_of<InputComponent>(admin))
        {
            auto &input     = reg.get<InputComponent>(admin);
            input.reactArea = glm::vec4(viewMin.x, viewMin.y, viewMax.x, viewMax.y);
        }

        // 处理Gizmo操作
        auto selected_entity = m_editor->getSelectedEntity();
        if (selected_entity != entt::null && reg.all_of<TransformComponent>(selected_entity))
        {
            ImGuizmo::SetOrthographic(false);
            ImDrawList *drawList = ImGui::GetWindowDrawList();
            ImGuizmo::SetDrawlist(drawList);
            ImGuizmo::SetRect(itemRectMin.x, itemRectMin.y, availableSize.x, availableSize.y);

            // 强制状态重置
            static glm::mat4 prevModel(1.0f);

            // camera
            glm::mat4 view = camera_comp.getWorldInverseMatrix();
            glm::mat4 proj = camera_comp.getProjectionMatrix();

            auto &transform = reg.get<TransformComponent>(selected_entity);
            glm::mat4 model = transform.getWorldMatrix();

            // snappping
            ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(model),
                                 nullptr, nullptr);

            if (ImGuizmo::IsUsing())
            {
                m_isUsingGizmo = true;

                if (reg.all_of<RigidBodyComponent>(selected_entity))
                {
                    auto &rigid      = reg.get<RigidBodyComponent>(selected_entity);
                    rigid.isActivate = false;
                }

                LOG_DEBUG("IsUsing");
                glm::vec3 translation, rotation, scale;
                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model), glm::value_ptr(translation), glm::value_ptr(rotation),
                                                      glm::value_ptr(scale));

                transform.setPosition(translation);
                transform.setRotation(glm::quat(glm::radians(rotation)));
                transform.setScale(scale);

                transform.updateWorldMatrix();
            }
            else
            {
                m_isUsingGizmo = false;

                if (reg.all_of<RigidBodyComponent>(selected_entity))
                {
                    auto &rigid      = reg.get<RigidBodyComponent>(selected_entity);
                    rigid.isActivate = true;
                }

            }

            if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_W)))
            {
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            }
            if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_R)))
            {
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            }
            if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_E)))
            {
                mCurrentGizmoOperation = ImGuizmo::SCALE;
            }

        }

        // 射线检测
        if (ImGui::IsItemHovered() && !m_isUsingGizmo)
        {

            auto physics_comp = scene->getComponent<PhysicsWorldComponent>(admin);
            // 检测鼠标点击
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                ImVec2 imgui_mousePos = ImGui::GetMousePos(); // 获取鼠标的屏幕坐标
                ImVec2 local_pos      = ImVec2(imgui_mousePos.x - itemRectMin.x, imgui_mousePos.y - itemRectMin.y);

                glm::vec2 mousePos = glm::vec2(local_pos.x, local_pos.y);

                auto ray           = camera_comp.screenPointToRay(mousePos, glm::vec4(0, 0, availableSize.x, availableSize.y));
                auto &physics_comp = scene->getComponent<PhysicsWorldComponent>(admin);

                auto ray_info = physics_comp.raycast(ray.first, ray.second, 1000.0f);
                if (ray_info.hasHit)
                {
                    auto entity = ray_info.entity;
                    auto &rigid = reg.get<RigidBodyComponent>(entity);
                    auto &tag   = reg.get<TagComponent>(entity);

                    m_editor->setSelectedEntity(entity);
                }
            }
        }

        ImGui::End();
    }

  private:
    glm::mat4 testModel  = glm::mat4(1.0f);
    glm::vec4 m_lastSize = glm::vec4(0.0f, 0.0f, 1200.0f, 800.0f); // 反应区域

    ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE mCurrentGizmoMode           = ImGuizmo::WORLD;
    bool m_isUsingGizmo                        = false;
};

} // namespace Airwave
