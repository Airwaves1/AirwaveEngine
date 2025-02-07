#include "core/application.hpp"
#include "core/log.hpp"

#include "ecs/systems/transform_system.hpp"
#include "ecs/systems/camera_system.hpp"
#include "ecs/systems/render_system.hpp"
#include "ecs/systems/input_system.hpp"
#include "ecs/systems/light_system.hpp"
#include "resource/resource_manager.hpp"


namespace Airwave
{
Application::~Application() {}
void Application::start(int argc, char **argv)
{
    // 记录开始的时间
    m_startTimePoint = std::chrono::steady_clock::now();

    // 初始化日志
    Log::Init();

    // 配置应用程序
    onConfigurate(m_config);

    // 创建窗口
    m_window = std::make_unique<Window>(m_config.width, m_config.height, m_config.title);

    // 创建渲染器
    m_renderer = std::make_unique<Renderer>(this);

    // 创建场景
    m_scene = std::make_unique<AwScene>(this);
    m_scene->addSystem<InputSystem>(0);
    m_scene->addSystem<TransformSystem>(0);
    m_scene->addSystem<CameraSystem>(0);
    m_scene->addSystem<LightSystem>(0);
    m_scene->addSystem<RenderSystem>(2);

    auto adminEntity = m_scene->getAdminEntity();
    if (adminEntity)
    {
        adminEntity->addComponent<RendererComponent>();
        adminEntity->addComponent<InputComponent>();
        adminEntity->addComponent<LightsManagerComponent>();
    }

    // 初始化
    onInit();

    // 事件
    // m_eventObserver = std::make_shared<EventObserver>();
}

void Airwave::Application::quit()
{
    // 销毁
    onDestory();
}

void Airwave::Application::mainLoop()
{

    while (!m_window->shouldClose())
    {
        // 处理事件
        m_window->pollEvents();

        // 计算时间
        auto now             = std::chrono::steady_clock::now();
        float deltaTime      = std::chrono::duration<float>(now - m_lastFrameTimePoint).count();
        m_lastFrameTimePoint = now;
        m_frameIndex++;
        m_deltaTime = deltaTime;

        // 更新和渲染
        if (!b_pause)
        {
            onUpdate(deltaTime);

            m_scene->update(deltaTime);

            onRender();
        }

        onImGuiRender();

        m_window->swapBuffers();
    }
}

} // namespace Airwave