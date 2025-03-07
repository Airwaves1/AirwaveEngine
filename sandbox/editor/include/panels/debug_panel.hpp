#pragma once 
 
#include <functional>
#include <chrono>
#include "panels/panel.hpp" 
#include "ecs/components/singleton_components/renderer_component.hpp"
 
namespace Airwave 
{
class DebugPanel : public Panel 
{
  public:
    std::function<void()> onDrawDebugInfo;
 
    DebugPanel(Editor *editor) : Panel(editor, "Debug")
    {
        frameCount = 0;
        fpsTimer = std::chrono::high_resolution_clock::now();
        fps = 0.0;
    }
 
    void onImGuiRender() override 
    {
        auto app = m_editor->getContext();
 
        ImGui::Begin(m_title.c_str());
 
        if (m_editor->onDrawDebugInfo)
        {
            m_editor->onDrawDebugInfo();
        }
 
        // 更新FPS计数器 
        frameCount++;
 
        // 计算时间差 
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto timeDifference = std::chrono::duration_cast<std::chrono::seconds>(currentTime - fpsTimer);
 
        // 如果时间差达到1秒，则计算FPS 
        if (timeDifference.count()  >= 1)
        {
            fps = static_cast<double>(frameCount) / timeDifference.count(); 
            frameCount = 0;
            fpsTimer = currentTime;
        }

        auto adminEntity = app->getScene()->getAdminEntity();
        auto &renderer_comp = app->getScene()->getComponent<RendererComponent>(adminEntity);
        auto& profiler = renderer_comp.profiler;
        
        ImGui::Text("CPU Frame Time: %.2f ms", profiler->getCPUFrameTime());
        ImGui::Text("GPU Frame Time: %.2f ms", profiler->getGPUFrameTime());
        ImGui::Text("FPS: %d", profiler->getFPS());

        
        // 显示FPS和Draw Calls 
        ImGui::Text("imgui_FPS: %.2f", fps);
        ImGui::Text("Draw Calls: %llu", app->getRenderer()->drawCalls);
 
        ImGui::End();
    }
 
  private:
    int frameCount = 0;
    std::chrono::high_resolution_clock::time_point fpsTimer;
    double fps = 0.0;
};
 
} // namespace Airwave 