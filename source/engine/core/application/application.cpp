#include "application.hpp"
#include "utils/log.hpp"

//----------imgui----------
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>

namespace Airwave
{

void Application::start(int argc, char **argv)
{
    // 记录开始时间
    m_startTimePoint = std::chrono::steady_clock::now();

    // 初始化日志
    Log::Init();

    // 解析命令行参数
    parseArgs(argc, argv);

    // 配置应用程序
    onConfigurate(m_config);

    // 创建窗口
    m_window = std::make_unique<AwWindow>(m_config.width, m_config.height, m_config.title);

    // 初始化
    onInit();

    // 初始化ImGui
    if (m_config.enableImGui)
    {
        // imgui
        // 创建 ImGui 上下文
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // 配置 ImGui 样式
        ImGui::StyleColorsLight();

        // 初始化 ImGui 的 GLFW 平台/窗口绑定
        ImGui_ImplGlfw_InitForOpenGL(m_window->getHandle(), true);

        // 初始化 ImGui 的 OpenGL3
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    // 事件处理 TODO
    m_eventObserver = std::make_shared<EventObserver>();
    handleEvent();
}

void Application::quit()
{
    // 销毁 ImGui
    if (m_config.enableImGui)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // 销毁
    onDestory();
}

void Application::mainLoop()
{
    while (!m_window->shouldClose())
    {
        // 事件处理
        m_window->pollEvents();

        // 计算时间
        auto now             = std::chrono::steady_clock::now();
        float deltaTime      = std::chrono::duration<float>(now - m_lastFrameTimePoint).count();
        m_lastFrameTimePoint = now;
        m_frameIndex++;

        // 更新和渲染
        if (!b_pause)
        {
            onUpdate(deltaTime);

            onRender();
        }

        // ImGui 渲染
        if (m_config.enableImGui)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            onImGuiRender();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        // 交换缓冲区
        m_window->swapBuffers();
    }
}

void Application::handleEvent()
{
    m_eventObserver->onEvent<WindowResizeEvent>(
        [this](const WindowResizeEvent &event)
        {
            m_config.width  = static_cast<uint32_t>(event.getWindowWidth());
            m_config.height = static_cast<uint32_t>(event.getWindowHeight());

            glViewport(0, 0, m_config.width, m_config.height);
        });
}

void Application::parseArgs(int argc, char **argv) {}

} // namespace Airwave
