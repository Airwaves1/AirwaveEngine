#include "application.hpp"
#include <stdexcept>
#include <iostream>
#include "core/event/event.hpp"
#include "core/event/event_bus.hpp"
#include "core/log/log.hpp"
#include "platform/file/file_utils.hpp"
#include "function/global/global_context.hpp"
#include "core/timer/timer.hpp"

namespace Airwave
{

void Airwave::Application::init(int argc, char **argv)
{
    PROFILE_SCOPE("Application::init");

    m_config_path = getConfigPath(argc, argv);

    GlobalContext.startRuntimeSystem(m_config_path);

    if (GlobalContext.window)
    {
        EventBus::getInstance().subscribe<WindowCloseEvent>([this](const WindowCloseEvent &event) { m_alive.store(false); });
    }

    // 初始化时间点
    m_lastFrameTimePoint = std::chrono::steady_clock::now();
    m_startTimePoint     = m_lastFrameTimePoint;

    onInit();
}

void Application::mainLoop()
{
    while (m_alive.load())
    {
        PROFILE_SCOPE("Application::mainLoop");
        // 计算帧时间
        auto now             = std::chrono::steady_clock::now();
        auto delta_time      = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastFrameTimePoint).count();
        m_lastFrameTimePoint = now;
        int fps = calculateFPS(delta_time / 1000.f);
        GlobalContext.window->setTitle("Airwave Engine - FPS: " + std::to_string(fps));
        GlobalContext.update(delta_time / 1000.f);
        onUpdate(delta_time / 1000.f);
    }
}

void Application::shutdown()
{
    onShutdown(); // 调用用户自定义的关闭函数

    GlobalContext.shutdownRuntimeSystem();

    Timer::PrintReport(); // 打印性能报告
}

std::string Application::getConfigPath(int argc, char **argv) const
{
    std::string config_path = "EngineConfig.json"; // 默认路径
    if (argc > 1)
    {
        std::string arg_path = argv[1];
        if (FileUtils::exists(arg_path)) // 确保文件存在
        {
            config_path = arg_path;
        }
        else
        {
            std::cerr << "Config file not found: " << arg_path << std::endl;
            throw std::runtime_error("Config file not found: " + arg_path);
        }
    }
    return config_path;
}

float Application::calculateFPS(float delta_time)
{
    constexpr float s_fps_alpha = 0.1f; // 平滑系数
    m_frame_count++;

    if (m_frame_count == 1)
    {
        m_average_duration = delta_time;
    }
    else
    {
        m_average_duration = m_average_duration * (1 - s_fps_alpha) + delta_time * s_fps_alpha;
    }

    m_fps = static_cast<int>(1.f / std::max(m_average_duration, 0.0001f)); // 避免除零错误
    return m_fps;
}

} // namespace Airwave
