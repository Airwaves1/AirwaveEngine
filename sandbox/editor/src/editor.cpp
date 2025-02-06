#include "editor.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "panels/panel.hpp"
#include "panels/scene_hierarchy_panel.hpp"
#include "panels/properties_panel.hpp"
#include "panels/content_browser_panel.hpp"
#include "panels/viewport_panel.hpp"
#include "panels/menu_bar_panel.hpp"
#include "panels/debug_panel.hpp"

namespace Airwave
{
Editor::Editor(Application *context) : m_context(context)
{
    // 初始化ImGui上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();

    // 启用关键功能
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // 样式
    ImGui::StyleColorsDark();
    setThemeStyle();

    ImGuiStyle &style    = ImGui::GetStyle();
    style.WindowRounding = 2.0f;
    style.FrameRounding  = 2.0f;

    // 字体
    setFontStyle();
    io.FontGlobalScale = 1.5f;
    // 加粗

    // 初始化平台后端
    ImGui_ImplGlfw_InitForOpenGL(m_context->getWindow()->getHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // 添加默认面板
    addPanel<SceneHierarchyPanel>(this, "Scene Hierarchy");
    addPanel<PropertiesPanel>(this, "Properties");
    addPanel<ContentBrowserPanel>(this, "Content Browser", PROJECT_ROOT_DIR);
    addPanel<ViewportPanel>(this, "Viewport");
    addPanel<MenuBarPanel>(this, "Menu Bar");
    addPanel<DebugPanel>(this);
}

Editor::~Editor()
{
    // 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Editor::setThemeStyle()
{
    auto &colors              = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    // Headers
    colors[ImGuiCol_Header]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab]                = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg]          = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive]    = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

void Editor::setFontStyle()
{
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Clear();
    // const char* fontPath = PROJECT_ROOT_DIR
    // "/assets/fonts/Nunito_Sans/NunitoSans-VariableFont_YTLC,opsz,wdth,wght.ttf"; const char*
    const char *fontPath = PROJECT_ROOT_DIR "/assets/fonts/Nunito_Sans/static/"
                                            "NunitoSans_10pt-Regular.ttf";
    // const char *fontPath = PROJECT_ROOT_DIR "/assets/fonts/Noto_Sans,Nunito_Sans/Noto_Sans/"
    //                                         "static/NotoSans_ExtraCondensed-SemiBold.ttf";

    ImFontConfig fontConfig;
    // fontConfig.OversampleH = 4;
    // fontConfig.OversampleV = 4;
    fontConfig.PixelSnapH           = true;
    fontConfig.FontDataOwnedByAtlas = true; // 让Atlas管理内存
    // fontConfig.FontDataOwnedByAtlas = true;
    // fontConfig.MergeMode   = true;

    io.Fonts->AddFontFromFileTTF(fontPath, 16.0f, &fontConfig,
                                 io.Fonts->GetGlyphRangesChineseFull()); // 支持中文);
    io.Fonts->Build();
}

void Editor::beginNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Editor::endNewFrame() { ImGui::EndFrame(); }

void Editor::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::beginDockSpace()
{
    // 获取主视口信息
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    // 设置DockSpace占据整个窗口
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    // 隐藏DockSpace的标题栏和边框
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // 开始DockSpace
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("MainDockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(2);

    // 创建Dock节点
    ImGuiID dockspace_id = ImGui::GetID("MainDock");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
}

void Editor::endDockSpace() { ImGui::End(); }

void Editor::dealMultiViewport()
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Editor::onUpdate(float deltaTime)
{
    for (auto &panel : m_panels)
    {
        panel->onUpdate(deltaTime);
    }
}

void Editor::renderPanels()
{
    for (auto &panel : m_panels)
    {
        panel->onImGuiRender();
    }
}

} // namespace Airwave
