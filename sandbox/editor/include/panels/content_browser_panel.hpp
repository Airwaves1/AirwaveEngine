#pragma once

#include <memory>
#include <string>
#include <filesystem>

#include "panels/panel.hpp"
#include <imgui.h>

namespace Airwave
{
class ContentBrowserPanel : public Panel
{
  public:
    ContentBrowserPanel(Editor *editor, const std::string &title, const std::string& baseDir, bool startOpened = true)
        : Panel(editor, title, startOpened), m_baseDir(baseDir), m_currentDir(m_baseDir)
    {
    }

    void onImGuiRender() override
    {
        ImGui::Begin(m_title.c_str());

        if (ImGui::Button("<-") && m_currentDir != m_baseDir)
        {
            m_currentDir = m_currentDir.parent_path();
        }
        ImGui::SameLine();
        ImGui::Text("当前路径: %s", m_currentDir.string().c_str());

        for (auto &entry : std::filesystem::directory_iterator(m_currentDir))
        {
            const auto &path     = entry.path();
            std::string filename = path.filename().string();

            if (entry.is_directory())
            {
                if (ImGui::Button(filename.c_str()))
                {
                    m_currentDir = path;
                }
            }
            else
            {
                ImGui::Text("%s", filename.c_str());
            }
        }

        ImGui::End();
    }

  private:
    std::filesystem::path m_baseDir;
    std::filesystem::path m_currentDir;
};

} // namespace Airwave
