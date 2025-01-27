#pragma once

#include <string>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace Airwave
{
class Window
{
  public:
    Window(uint32_t width, uint32_t height, const std::string &title);

    ~Window();

    // 窗口是否需要关闭
    bool shouldClose() const;

    // 处理事件
    void pollEvents();

    // 交换缓冲区
    void swapBuffers() const;

    // 获取窗口句柄
    GLFWwindow *getHandle() const { return m_handle; }

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }

  private:
    Window() = default;
    // 设置窗口回调
    void setupWindowCallbacks();

    // 窗口句柄
    GLFWwindow *m_handle{nullptr};

    uint32_t m_width{0};
    uint32_t m_height{0};
    std::string m_title;
};
} // namespace Airwave