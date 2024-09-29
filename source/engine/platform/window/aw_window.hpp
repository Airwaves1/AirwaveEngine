#pragma once

#include <string>
#include "event/event_observer.hpp"
#include "event/event.hpp"

struct GLFWwindow;
namespace Airwave
{
class AwWindow
{
  public:
    AwWindow() = delete;
    AwWindow(uint32_t width, uint32_t height, std::string title);
    ~AwWindow();

    bool shouldClose() const;

    void pollEvents();

    void swapBuffers();

    GLFWwindow *getHandle() const { return m_handle; }

    void getMousePosition(double &x, double &y);
    bool isMouseDown(MouseButton button = MouseButton::MOUSE_BUTTON_LEFT);
    bool isMouseUp(MouseButton button = MouseButton::MOUSE_BUTTON_LEFT);
    bool isKeyDown(int key);
    bool isKeyUp(int key);

  private:
    void setupWindowCallbacks();

    GLFWwindow *m_handle{nullptr};

    uint32_t m_width{0};
    uint32_t m_height{0};
    std::string m_title;
};

} // namespace Airwave
