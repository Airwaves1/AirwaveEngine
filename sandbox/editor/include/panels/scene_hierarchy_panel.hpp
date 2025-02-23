#pragma once

#include "panels/panel.hpp"
#include <imgui.h>
#include <entt/entt.hpp>

#include "ecs/aw_scene.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/tag_component.hpp"
#include "ecs/components/hierarchy_component.hpp"

#include "core/log.hpp"

namespace Airwave
{
class SceneHierarchyPanel : public Panel
{
  public:
    SceneHierarchyPanel(Editor *editor, const std::string &title, bool startOpened = true) : Panel(editor, title, startOpened)
    {
        m_editor->onSelectedEntityChanged = [&]()
        {
            buildSelectedPath(m_editor->getContext()->getScene(), m_editor->getSelectedEntity());
            m_shouldScrollToSelected = true; // 设置滚动标志
        };
    }

    virtual ~SceneHierarchyPanel() = default;

    void onImGuiRender() override
    {
        auto scene       = m_editor->getContext()->getScene();
        auto &registry   = scene->getRegistry();
        auto adminEntity = scene->getAdminEntity();
        ImGui::Begin(m_title.c_str(), &m_isOpen);

        bool hasScrolled = false;
        drawEntityHierarchy(scene, scene->getAdminEntity(), hasScrolled);

        // 如果未找到目标但需要滚动，则重置标志
        if (m_shouldScrollToSelected && !hasScrolled)
        {
            m_shouldScrollToSelected = false;
        }

        ImGui::End();

    }

  private:
    bool m_shouldScrollToSelected = false; // 滚动标志
    std::vector<entt::entity> m_selectionPath;
    void buildSelectedPath(AwScene *scene, entt::entity target)
    {
        m_selectionPath.clear();
        if (!scene->isValideEntity(target)) return;

        entt::entity current = target;
        while (current != entt::null)
        {
            m_selectionPath.insert(m_selectionPath.begin(), current); // 反向插入路径
            if (!scene->getRegistry().valid(current)) break;

            auto &hierarchy = scene->getComponent<HierarchyComponent>(current);
            current         = hierarchy.getParent();
        }
    }

    void drawEntityHierarchy(AwScene *scene, entt::entity entity, bool &hasScrolled)
    {
        if (!scene->getRegistry().valid(entity)) return;

        auto &reg = scene->getRegistry();
        if (reg.all_of<TagComponent, HierarchyComponent>(entity))
        {
            auto &tag       = reg.get<TagComponent>(entity);
            auto &hierarchy = reg.get<HierarchyComponent>(entity);

            bool inPath     = std::find(m_selectionPath.begin(), m_selectionPath.end(), entity) != m_selectionPath.end();
            bool isSelected = (entity == m_editor->getSelectedEntity());

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            if (inPath) flags |= ImGuiTreeNodeFlags_DefaultOpen;
            if (isSelected) flags |= ImGuiTreeNodeFlags_Selected;

            bool forceOpen = inPath && (entity != m_selectionPath.front());
            bool nodeOpen =
                ImGui::TreeNodeEx((void *)(intptr_t)entity, flags | (forceOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0), "%s", tag.name.c_str());

            // 滚动聚焦逻辑
            if (isSelected && m_shouldScrollToSelected && !hasScrolled)
            {
                ImGui::SetScrollHereY(0.5f); // 垂直居中
                hasScrolled = true;
            }

            if (ImGui::IsItemClicked())
            {
                m_editor->setSelectedEntity(entity);
            }

            if (nodeOpen || forceOpen)
            {
                for (auto child : hierarchy.getChildren())
                {
                    bool childHasScrolled = false;
                    drawEntityHierarchy(scene, child, hasScrolled);
                }
                ImGui::TreePop();
            }
        }
    }
};
} // namespace Airwave