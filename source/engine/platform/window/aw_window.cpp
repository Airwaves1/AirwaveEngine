#include "aw_window.hpp"

#include "utils/log.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "event/event_dispatcher.hpp"

namespace Airwave
{
AwWindow::AwWindow(uint32_t width, uint32_t height, std::string title)
{

    if (!glfwInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
        return;
    }
#ifdef __linux__
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

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor()); // 获取主显示器的视频模式
    glfwSetWindowPos(m_handle, (mode->width - width) / 2, (mode->height - height) / 2); // 设置窗口位置

    glfwMakeContextCurrent(m_handle); // 将窗口的上下文设置为当前上下文

    setupWindowCallbacks();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        return;
    }

    glfwShowWindow(m_handle); // 显示窗口
}

AwWindow::~AwWindow()
{
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

bool AwWindow::shouldClose() const { return glfwWindowShouldClose(m_handle); }

void AwWindow::pollEvents() { glfwPollEvents(); }

void AwWindow::swapBuffers() { glfwSwapBuffers(m_handle); }

void AwWindow::getMousePosition(double &x, double &y) { glfwGetCursorPos(m_handle, &x, &y); }

bool AwWindow::isMouseDown(MouseButton button)
{
    return glfwGetMouseButton(m_handle, static_cast<int>(button)) == GLFW_PRESS;
}

bool AwWindow::isMouseUp(MouseButton button)
{
    return glfwGetMouseButton(m_handle, static_cast<int>(button)) == GLFW_RELEASE;
}

bool AwWindow::isKeyDown(int key) { return glfwGetKey(m_handle, key) == GLFW_PRESS; }

bool AwWindow::isKeyUp(int key) { return glfwGetKey(m_handle, key) == GLFW_RELEASE; }

void AwWindow::setupWindowCallbacks()
{

    glfwSetWindowUserPointer(m_handle, this);

    glfwSetFramebufferSizeCallback(m_handle,
                                   [](GLFWwindow *window, int width, int height)
                                   {
                                       AwWindow *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
                                       if (awWindow)
                                       {
                                           WindowResizeEvent event(width, height);
                                           EventDispatcher::GetInstance().dispatch(event);
                                       }
                                   });

    glfwSetWindowFocusCallback(m_handle,
                               [](GLFWwindow *window, int focused)
                               {
                                   auto *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
                                   if (awWindow)
                                   {
                                       if (focused)
                                       {
                                           WindowFocusEvent event{};
                                           EventDispatcher::GetInstance().dispatch(event);
                                       }
                                       else
                                       {
                                           WindowLostFocusEvent event{};
                                           EventDispatcher::GetInstance().dispatch(event);
                                       }
                                   }
                               });

    glfwSetWindowCloseCallback(m_handle,
                               [](GLFWwindow *window)
                               {
                                   auto *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
                                   if (awWindow)
                                   {
                                       WindowCloseEvent event{};
                                       EventDispatcher::GetInstance().dispatch(event);
                                   }
                               });

    glfwSetWindowPosCallback(m_handle,
                             [](GLFWwindow *window, int xpos, int ypos)
                             {
                                 auto *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
                                 if (awWindow)
                                 {
                                     WindowMoveEvent event(xpos, ypos);
                                     EventDispatcher::GetInstance().dispatch(event);
                                 }
                             });

    glfwSetKeyCallback(m_handle,
                       [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
                           auto *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
                           if (awWindow)
                           {
                               switch (action)
                               {
                                   case GLFW_PRESS:
                                   {
                                       KeyPressedEvent event(key, 0);
                                       EventDispatcher::GetInstance().dispatch(event);
                                       break;
                                   }
                                   case GLFW_RELEASE:
                                   {
                                       KeyReleasedEvent event(key);
                                       EventDispatcher::GetInstance().dispatch(event);
                                       break;
                                   }
                                   case GLFW_REPEAT:
                                   {
                                       KeyPressedEvent event(key, 1);
                                       EventDispatcher::GetInstance().dispatch(event);
                                       break;
                                   }
                               }
                           }
                       });

    glfwSetCharCallback(m_handle,
                        [](GLFWwindow *window, unsigned int keycode)
                        {
                            auto *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
                            if (awWindow)
                            {
                                KeyTypedEvent event(keycode);
                                EventDispatcher::GetInstance().dispatch(event);
                            }
                        });

    glfwSetMouseButtonCallback(
        m_handle,
        [](GLFWwindow *window, int button, int action, int mods)
        {
            auto *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
            if (awWindow)
            {
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        double x, y;
                        glfwGetCursorPos(window, &x, &y);
                        MouseButtonPressedEvent event(static_cast<MouseButton>(button), static_cast<float>(x),
                                                      static_cast<float>(y));
                        EventDispatcher::GetInstance().dispatch(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        double x, y;
                        glfwGetCursorPos(window, &x, &y);
                        MouseButtonReleasedEvent event(static_cast<MouseButton>(button), static_cast<float>(x),
                                                       static_cast<float>(y));
                        EventDispatcher::GetInstance().dispatch(event);
                        break;
                    }
                }
            }
        });

    glfwSetCursorPosCallback(m_handle,
                             [](GLFWwindow *window, double xpos, double ypos)
                             {
                                 auto *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
                                 if (awWindow)
                                 {
                                     MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
                                     EventDispatcher::GetInstance().dispatch(event);
                                 }
                             });

    glfwSetScrollCallback(m_handle,
                          [](GLFWwindow *window, double xoffset, double yoffset)
                          {
                              auto *awWindow = static_cast<AwWindow *>(glfwGetWindowUserPointer(window));
                              if (awWindow)
                              {
                                  MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
                                  EventDispatcher::GetInstance().dispatch(event);
                              }
                          });
}

} // namespace Airwave
