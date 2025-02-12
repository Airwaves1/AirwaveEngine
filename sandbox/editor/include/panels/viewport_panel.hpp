#pragma once

#include "panels/panel.hpp"
#include <imgui.h>

#include "rendering/texture.hpp"
#include "ecs/components/singleton_components/input_component.hpp"

namespace Airwave
{
class ViewportPanel : public Panel
{
  public:
    ViewportPanel(Editor *editor, const std::string &title, bool startOpened = true) : Panel(editor, title, startOpened) {}

    void onImGuiRender() override
    {
        auto app   = m_editor->getContext();
        auto &reg  = app->getScene()->getRegistry();
        auto admin = app->getAdminEntity();

        ImGui::Begin(m_title.c_str());
        ImVec2 availableSize = ImGui::GetContentRegionAvail(); // 获取当前窗口可用大小
        auto fbo             = m_editor->getContext()->getRenderer()->getFramebuffer();
        auto textureID       = fbo->getColorAttachment()->getHandle();

        ImGui::Image((void *)(intptr_t)textureID, availableSize, ImVec2(0, 1), ImVec2(1, 0));

        // 如果窗口大小发生变化，更新反应区域
        ImVec2 itemRectMin     = ImGui::GetItemRectMin();
        ImVec2 itemRectMax     = ImGui::GetItemRectMax();
        ImVec2 mainViewportPos = ImGui::GetMainViewport()->Pos;
        ImVec2 viewMin         = ImVec2(itemRectMin.x - mainViewportPos.x, itemRectMin.y - mainViewportPos.y);
        ImVec2 viewMax         = ImVec2(itemRectMax.x - mainViewportPos.x, itemRectMax.y - mainViewportPos.y);

        if (reg.all_of<InputComponent>(admin))
        {
            auto &input     = reg.get<InputComponent>(admin);
            input.reactArea = glm::vec4(viewMin.x, viewMin.y, viewMax.x, viewMax.y);
        }


        ImGui::End();
    }

  private:
    glm::vec4 m_lastSize = glm::vec4(0.0f, 0.0f, 1200.0f, 800.0f); // 反应区域
};

} // namespace Airwave
