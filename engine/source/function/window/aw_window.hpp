#pragma once
#include <string>
#include <array>

struct GLFWwindow;

namespace Airwave
{
struct AwWindowConfig
{
    std::string title  = "Airwave Engine";
    int width          = 1280;
    int height         = 720;
    bool is_focus_mode = false;
    bool is_fullscreen = false;
};

class AwWindow
{
  public:
    AwWindow() = default;

    AwWindow(const AwWindow &)            = delete;
    AwWindow &operator=(const AwWindow &) = delete;
    AwWindow(AwWindow &&)                 = delete;
    AwWindow &operator=(AwWindow &&)      = delete;

    ~AwWindow();

    void initialize(const AwWindowConfig &config);
    void pollEvents() const;
    void swapBuffers() const;
    bool shouldClose() const;
    void swapBuffer() const;
    GLFWwindow *getHandle() const { return m_window; }
    std::array<int, 2> getSize() const { return {m_width, m_height}; }

    void setTitle(const std::string &title);
    void setupWindowCallbacks();
    

  private:
    GLFWwindow *m_window = nullptr;
    std::string m_title  = "Airwave Engine";
    int m_width          = 1280;
    int m_height         = 720;
    bool m_is_focus_mode = false;
};
} // namespace Airwave
