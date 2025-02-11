#pragma once

#include "panels/panel.hpp"
#include <imgui.h>
#include <entt/entt.hpp>

#include "ecs/aw_scene.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/tag_component.hpp"
#include "ecs/components/hierarchy_component.hpp"

namespace Airwave
{
class SceneHierarchyPanel : public Panel
{
  public:
    SceneHierarchyPanel(Editor *editor, const std::string &title, bool startOpened = true) : Panel(editor, title, startOpened) {}

    virtual ~SceneHierarchyPanel() = default;

    void onImGuiRender() override
    {
        auto scene       = m_editor->getContext()->getScene();
        auto &registry   = scene->getRegistry();
        auto adminEntity = scene->getAdminEntity();
        ImGui::Begin(m_title.c_str(), &m_isOpen);

        // 获取所有实体并按层次结构绘制
        drawEntityHierarchy(scene, adminEntity, false);

        ImGui::End();
    }

  private:
    // 递归绘制实体层次结构
    void drawEntityHierarchy(AwScene *scene, entt::entity entity, bool drawCurrentLevel = true)
    {
        // 判断当前entity是否具有tag和hierarchy组件
        if (scene->getRegistry().all_of<TagComponent, HierarchyComponent>(entity))
        {
            auto &tag       = scene->getRegistry().get<TagComponent>(entity);
            auto &hierarchy = scene->getRegistry().get<HierarchyComponent>(entity);

            if (drawCurrentLevel)
            {
                // 创建一个树节点
                bool isSelect                = m_editor->getSelectedEntity() == entity;
                ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
                if (isSelect) nodeFlags |= ImGuiTreeNodeFlags_Selected;
                bool isOpen = ImGui::TreeNodeEx(tag.name.c_str(), nodeFlags);

                // 处理点击事件
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen())
                {
                    m_editor->setSelectedEntity(entity);
                    if (m_editor->onSelectedEntityChanged)
                    {
                        m_editor->onSelectedEntityChanged();
                    }
                }

                // 如果当前节点展开，递归绘制子实体
                if (isOpen)
                {
                    for (auto &child : hierarchy.getChildren())
                    {
                        drawEntityHierarchy(scene, child);
                    }
                    ImGui::TreePop();
                }
            }
            else
            {
                // 递归绘制子实体
                for (auto &child : hierarchy.getChildren())
                {
                    drawEntityHierarchy(scene, child);
                }
            }
        }
    }
};
} // namespace Airwave