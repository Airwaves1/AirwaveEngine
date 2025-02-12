#include "core/application.hpp"
#include "core/log.hpp"

#include "utils/texture_utils.hpp"

#include "ecs/systems/transform_system.hpp"
#include "ecs/systems/camera_system.hpp"
#include "ecs/systems/render_system.hpp"
#include "ecs/systems/input_system.hpp"
#include "ecs/systems/light_system.hpp"

#include "resource/resource_manager.hpp"
#include "resource/shader_resource.hpp"
#include "resource/texture_resource.hpp"

#include "core/common.hpp"

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

    // 预加载
    preLoad();

    // 创建场景
    m_scene = std::make_unique<AwScene>(this);
    m_scene->addSystem<InputSystem>(0);
    m_scene->addSystem<TransformSystem>(0);
    m_scene->addSystem<CameraSystem>(0);
    m_scene->addSystem<LightSystem>(0);
    m_scene->addSystem<RenderSystem>(2);

    auto &reg        = m_scene->getRegistry();
    auto adminEntity = m_scene->getAdminEntity();

    m_scene->addComponent<RendererComponent>(adminEntity);
    m_scene->addComponent<InputComponent>(adminEntity);
    m_scene->addComponent<LightsManagerComponent>(adminEntity);

    // 初始化
    onInit();

    // 事件
    // m_eventObserver = std::make_shared<EventObserver>();
}

void Application::quit()
{
    // 销毁
    onDestory();
}

void Application::preLoad()
{
    try
    {
        // 预编译shader
        // basic shader
        auto basic = RES.load<ShaderResource>("shaders/shader_lib/basic.glsl");
        // pbr shader
        auto pbr = RES.load<ShaderResource>("shaders/shader_lib/pbr.glsl");
        // background shader
        auto background = RES.load<ShaderResource>("shaders/shader_lib/background.glsl");

        // // 生成BRDF LUT
        // auto brdfLUT         = TextureUtils::generateBRDFLUT(m_renderer.get());
        // auto brdfLUTResource = std::make_shared<TextureResource>(brdfLUT);
        // RES.add<TextureResource>("brdf_lut", brdfLUTResource);

        // 加载资源
        onPreLoad();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Failed to preload resources: {0}", e.what());
    }

    LOG_INFO("Preload resources done");
}

void Application::mainLoop()
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