#include "aw_window.hpp"
#include "core/log/log.hpp"
#include "core/event/event.hpp"
#include "core/event/event_bus.hpp"
#include "platform/file/file_utils.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace Airwave
{

AwWindow::~AwWindow()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

void AwWindow::initialize(const AwWindowConfig &config)
{
    if (!glfwInit())
    {
        LOG_ERROR("Failed to initialize GLFW!");
        return;
    }

#ifdef __LINUX__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif _WIN32
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    //
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        LOG_ERROR("Failed to create GLFW window!");
        glfwTerminate();
        return;
    }

    m_title         = config.title;
    m_width         = config.width;
    m_height        = config.height;
    m_is_focus_mode = config.is_focus_mode;

    // 设置窗口居中
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(m_window, (mode->width - m_width) / 2, (mode->height - m_height) / 2);

    // 注册回调事件
    setupWindowCallbacks();

    // 将窗口设置为当前上下文
    glfwMakeContextCurrent(m_window);

    // 加载OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return;
    }

    // 做完所有准备工作后就显示窗口了
    glfwShowWindow(m_window);
}

void AwWindow::pollEvents() const { glfwPollEvents(); }

void AwWindow::swapBuffers() const { glfwSwapBuffers(m_window); }

bool AwWindow::shouldClose() const { return glfwWindowShouldClose(m_window); }

void AwWindow::swapBuffer() const { glfwSwapBuffers(m_window); }

void AwWindow::setTitle(const std::string &title) { glfwSetWindowTitle(m_window, title.c_str()); }

void AwWindow::setupWindowCallbacks()
{
    glfwSetWindowUserPointer(m_window, this);

    glfwSetWindowCloseCallback(m_window,
                               [](GLFWwindow *window)
                               {
                                   WindowCloseEvent event;
                                   EventBus::getInstance().postEvent(event);
                               });

    glfwSetWindowFocusCallback(m_window,
                               [](GLFWwindow *window, int focused)
                               {
                                   WindowFocusEvent event(focused == GLFW_TRUE);
                                   EventBus::getInstance().postEvent(event);
                               });

    glfwSetKeyCallback(m_window,
                       [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
                           static float repeatTime = 0;
                           if (action == GLFW_PRESS || action == GLFW_REPEAT) // GLFW_REPEAT 也是 action
                           {
                               if (action == GLFW_REPEAT)
                               {
                                   KeyRepeatEvent event(key, repeatTime);
                                   EventBus::getInstance().postEvent(event);
                               }
                               else
                               {
                                   KeyPressEvent event(key);
                                   EventBus::getInstance().postEvent(event);
                               }
                           }
                           else if (action == GLFW_RELEASE)
                           {
                               KeyReleaseEvent event(key);
                               EventBus::getInstance().postEvent(event);
                           }
                       });

    glfwSetMouseButtonCallback(m_window,
                               [](GLFWwindow *window, int button, int action, int mods)
                               {
                                   static double x, y;
                                   glfwGetCursorPos(window, &x, &y);

                                   if (action == GLFW_PRESS)
                                   {
                                       MouseButtonPressEvent event(static_cast<int>(x), static_cast<int>(y), button);
                                       EventBus::getInstance().postEvent(event);
                                   }
                                   else if (action == GLFW_RELEASE)
                                   {
                                       MouseButtonReleaseEvent event(static_cast<int>(x), static_cast<int>(y), button);
                                       EventBus::getInstance().postEvent(event);
                                   }
                               });

    glfwSetCursorPosCallback(m_window,
                             [](GLFWwindow *window, double x, double y)
                             {
                                // EventBus::getInstance().queueEvent(std::make_unique<MouseMoveEvent>(static_cast<int>(x), static_cast<int>(y)));
                                MouseMoveEvent event(static_cast<int>(x), static_cast<int>(y));
                                EventBus::getInstance().postEvent(event);
                             });
}

} // namespace Airwave
