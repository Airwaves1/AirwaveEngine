#pragma once

#include <string>
#include <any>
namespace Airwave
{

class Event
{
  public:
    virtual ~Event()                    = default;
    virtual std::string getType() const = 0;        // get the type of the event
    virtual std::any getData() const { return {}; } // get the data of the event
};

class WindowCloseEvent : public Event
{
  public:
    std::string getType() const override { return "WindowClose"; }
};

class WindowResizeEvent : public Event
{
  public:
    WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

    std::string getType() const override { return "WindowResize"; }

    inline uint32_t getWidth() const { return m_width; }
    inline uint32_t getHeight() const { return m_height; }

  private:
    uint32_t m_width;
    uint32_t m_height;
};

class KeyEvent : public Event
{
  public:
    std::string getType() const override { return "KeyEvent"; }

    inline int getKey() const { return m_key; }

  protected:
    KeyEvent(int key) : m_key(key) {}

    int m_key;
};

class KeyPressEvent : public KeyEvent
{
  public:
    KeyPressEvent(int keycode, int keyRepeat) : KeyEvent(keycode), m_keyRepeat(keyRepeat) {}

    std::string getType() const override { return "KeyPress"; }

    inline int getRepeat() const { return m_keyRepeat; }

  private:
    int m_keyRepeat;
};

class KeyReleaseEvent : public KeyEvent
{
  public:
    KeyReleaseEvent(int keycode) : KeyEvent(keycode) {}

    std::string getType() const override { return "KeyRelease"; }
};

class MouseMoveEvent : public Event
{
  public:
    MouseMoveEvent(float x, float y) : m_x(x), m_y(y) {}

    std::string getType() const override { return "MouseMove"; }

    inline float getX() const { return m_x; }
    inline float getY() const { return m_y; }

  private:
    float m_x;
    float m_y;
};

class MouseScrollEvent : public Event
{
  public:
    MouseScrollEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

    std::string getType() const override { return "MouseScroll"; }

    inline float getXOffset() const { return m_xOffset; }
    inline float getYOffset() const { return m_yOffset; }

  private:
    float m_xOffset;
    float m_yOffset;
};

class MouseButton : public Event
{
  public:
    MouseButton(int button, float x, float y) : m_button(button), m_x(x), m_y(y) {}

    std::string getType() const override { return "MouseButton"; }

    inline int getButton() const { return m_button; }
    inline float getX() const { return m_x; }
    inline float getY() const { return m_y; }

  private:
    int m_button;
    float m_x;
    float m_y;
};

class MouseButtonPressEvent : public MouseButton
{
  public:
    MouseButtonPressEvent(int button, float x, float y) : MouseButton(button, x, y) {}

    std::string getType() const override { return "MouseButtonPress"; }
};

class MouseButtonReleaseEvent : public MouseButton
{
  public:
    MouseButtonReleaseEvent(int button, float x, float y) : MouseButton(button, x, y) {}

    std::string getType() const override { return "MouseButtonRelease"; }
};

} // namespace Airwave
