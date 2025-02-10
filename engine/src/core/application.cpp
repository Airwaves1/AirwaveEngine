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

void Application::quit()
{
    // 销毁
    onDestory();
}

void Application::preLoad()
{

    // 预编译shader

    // basic shader
    RES.load<ShaderResource>("basic", PROJECT_ROOT_DIR "/assets/shaders/shader_lib/vert/basic.vert",
                             PROJECT_ROOT_DIR "/assets/shaders/shader_lib/frag/basic.frag");

    // pbr shader
    RES.load<ShaderResource>("pbr", PROJECT_ROOT_DIR "/assets/shaders/shader_lib/vert/pbr.vert",
                             PROJECT_ROOT_DIR "/assets/shaders/shader_lib/frag/pbr.frag");

    // background shader
    RES.load<ShaderResource>("background", PROJECT_ROOT_DIR "/assets/shaders/shader_lib/vert/background.vert",
                             PROJECT_ROOT_DIR "/assets/shaders/shader_lib/frag/background.frag");

    // 空白纹理和默认法线贴图
    TextureSpecification spec;
    spec.internalFormat        = TextureInternalFormat::RGBA;
    spec.width                 = 1;
    spec.height                = 1;
    std::vector<uint8_t> data  = {255, 255, 255, 255};
    std::vector<uint8_t> data2 = {128, 128, 255, 255};
    auto emptyTexture          = std::make_shared<TextureResource>(spec, data);
    auto defaultNormal         = std::make_shared<TextureResource>(spec, data2);

    RES.add("empty_texture", emptyTexture);
    RES.add("default_normal", defaultNormal);

    // 生成BRDF LUT
    auto brdfLUT = TextureUtils::generateBRDFLUT(m_renderer.get());
    RES.add("brdf_lut", brdfLUT);
    // 加载资源
    onPreLoad();
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