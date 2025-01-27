
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "core/window.hpp"
#include "core/log.hpp"
#include "core/event/event.hpp"
#include "core/event/event_dispatcher.hpp"

namespace Airwave
{
Window::Window(uint32_t width, uint32_t height, const std::string &title)
{
    if (!glfwInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
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

    // 暂时隐藏窗口
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_handle)
    {
        LOG_ERROR("Failed to create GLFW window");
        return;
    }
    m_width  = width;
    m_height = height;
    m_title  = title;

    // 设置窗口居中
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(m_handle, (mode->width - width) / 2, (mode->height - height) / 2);

    // 将窗口设置为当前上下文
    glfwMakeContextCurrent(m_handle);

    // 注册回调事件
    setupWindowCallbacks();

    // 加载OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return;
    }

    // 做完所有准备工作后就显示窗口了
    glfwShowWindow(m_handle);
}

Window::~Window()
{
    if (m_handle)
    {
        glfwDestroyWindow(m_handle);
    }

    glfwTerminate();
}

bool Window::shouldClose() const { return glfwWindowShouldClose(m_handle); }

void Window::pollEvents() { glfwPollEvents(); }

void Window::swapBuffers() const { glfwSwapBuffers(m_handle); }
void Window::setupWindowCallbacks()
{
    glfwSetWindowUserPointer(m_handle, this);

    glfwSetFramebufferSizeCallback(m_handle,
                                   [](GLFWwindow *window, int width, int height)
                                   {
                                       WindowResizeEvent event(width, height);
                                       EventDispatcher::GetInstance().dispatch(event);
                                   });

    glfwSetWindowCloseCallback(m_handle,
                               [](GLFWwindow *window)
                               {
                                   WindowCloseEvent event;
                                   EventDispatcher::GetInstance().dispatch(event);
                               });

    glfwSetKeyCallback(m_handle,
                       [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
                           if (action == GLFW_PRESS)
                           {
                               KeyPressEvent event(key, 0);
                               EventDispatcher::GetInstance().dispatch(event);
                           }
                           else if (action == GLFW_RELEASE)
                           {
                               KeyReleaseEvent event(key);
                               EventDispatcher::GetInstance().dispatch(event);
                           }
                       });

    glfwSetCursorPosCallback(m_handle,
                             [](GLFWwindow *window, double xpos, double ypos)
                             {
                                 MouseMoveEvent event(xpos, ypos);
                                 EventDispatcher::GetInstance().dispatch(event);
                             });

    glfwSetMouseButtonCallback(m_handle,
                               [](GLFWwindow *window, int button, int action, int mods)
                               {
                                   if (action == GLFW_PRESS)
                                   {
                                       MouseButtonPressEvent event(button, 0.0f, 0.0f); // 默认坐标
                                       EventDispatcher::GetInstance().dispatch(event);
                                   }
                                   else if (action == GLFW_RELEASE)
                                   {
                                       MouseButtonReleaseEvent event(button, 0.0f, 0.0f);
                                       EventDispatcher::GetInstance().dispatch(event);
                                   }
                               });

    glfwSetScrollCallback(m_handle,
                          [](GLFWwindow *window, double xoffset, double yoffset)
                          {
                              MouseScrollEvent event(xoffset, yoffset);
                              EventDispatcher::GetInstance().dispatch(event);
                          });
}

} // namespace Airwave
