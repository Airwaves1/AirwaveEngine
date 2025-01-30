#pragma once

#include "panels/panel.hpp"
#include <imgui.h>

namespace Airwave
{
class MenuBarPanel : public Panel
{
  public:
    MenuBarPanel(Editor *editor, const std::string &title, bool startOpened = true) : Panel(editor, title, startOpened) {}

    void onImGuiRender() override
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    onNewScene();
                }
                if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
                {
                    onOpenScene();
                }
                if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                {
                    onSaveScene();
                }
                if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
                {
                    onSaveAs();
                }
                if (ImGui::MenuItem("Exit", "Ctrl+Q"))
                {
                    onExit();
                }   
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z"))
                {
                    onUndo();
                }
                if (ImGui::MenuItem("Redo", "Ctrl+Y"))
                {
                    onRedo();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Scene Hierarchy", nullptr, false, true))
                {
                    // Do something
                }
                if (ImGui::MenuItem("Properties", nullptr, false, true))
                {
                    // Do something
                }
                if (ImGui::MenuItem("Content Browser", nullptr, false, true))
                {
                    // Do something
                }
                if (ImGui::MenuItem("Viewport", nullptr, false, true))
                {
                    // Do something
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void onNewScene()
    {
        // Do something
    }

    void onOpenScene()
    {
        // Do something
    }

    void onSaveScene()
    {
        // Do something
    }

    void onSaveAs()
    {
        // Do something
    }

    void onExit()
    {
        // Do something
    }

    void onUndo()
    {
        // Do something
    }

    void onRedo()
    {
        // Do something
    }

    void onSceneHierarchy()
    {
        // Do something
    }

    void onProperties()
    {
        // Do something
    }

    void onContentBrowser()
    {
        // Do something
    }

    void onViewport()
    {
        // Do something
    }

  private:
    std::string m_currentDir;
};

} // namespace Airwave
