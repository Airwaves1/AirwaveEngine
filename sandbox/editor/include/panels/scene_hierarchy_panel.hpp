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
    SceneHierarchyPanel(Editor *editor, const std::string &title, bool startOpened = true)
        : Panel(editor, title, startOpened)
    {
    }

    virtual ~SceneHierarchyPanel() = default;

    void onImGuiRender() override
    {
        auto scene       = m_editor->getContext()->getScene();
        auto &registry   = scene->getRegistry();
        auto adminEntity = scene->getAdminEntity();
        ImGui::Begin(m_title.c_str(), &m_isOpen);

        // 获取所有实体并按层次结构绘制
        drawEntityHierarchy(scene, registry, adminEntity);

        ImGui::End();
    }

  private:
    // 递归绘制实体层次结构
    void drawEntityHierarchy(AwScene *scene, entt::registry &registry, AwEntity *awEntity)
    {
        auto view = registry.view<TagComponent, HierarchyComponent>();

        for (auto entity : view)
        {
            auto [tag, hierarchy] = view.get<TagComponent, HierarchyComponent>(entity);

            // 只绘制当前层级的实体
            if (scene->getEntityParent(scene->getEntity(entity)) == awEntity)
            {
                AwEntity *currentEntity = scene->getEntity(entity);
                bool isSelected         = (m_editor->getSelectedEntity() == currentEntity);

                ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
                if (isSelected) nodeFlags |= ImGuiTreeNodeFlags_Selected;

                // 创建一个树节点
                bool isOpen = ImGui::TreeNodeEx(tag.name.c_str(), nodeFlags);

                // 处理点击事件
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen()) 
                {
                    m_editor->setSelectedEntity(currentEntity);
                    if (m_editor->onSelectedEntityChanged)
                    {
                        m_editor->onSelectedEntityChanged();
                    }
                }

                // 如果当前节点展开，递归绘制子实体
                if (isOpen)
                {
                    drawEntityHierarchy(scene, registry, currentEntity);
                    ImGui::TreePop();
                }
            }
        }
    }
};
} // namespace Airwave
