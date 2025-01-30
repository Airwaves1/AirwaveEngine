#pragma once

#include <functional>
#include "panels/panel.hpp"

namespace Airwave
{
class DebugPanel : public Panel
{
  public:
    std::function<void()> onDrawDebugInfo;

    DebugPanel(Editor *editor) : Panel(editor, "Debug") {}

    void onImGuiRender() override
    {
        ImGui::Begin(m_title.c_str());

        if (m_editor->onDrawDebugInfo)
        {
            m_editor->onDrawDebugInfo();
        }

        ImGui::End();
    }

  private:
};

} // namespace Airwave
