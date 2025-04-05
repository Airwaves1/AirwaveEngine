#pragma once

#include <string>
#include <typeindex>

namespace Airwave
{
class Event
{
  public:
    virtual ~Event()                        = default;
    virtual std::string getName() const     = 0;
    virtual std::type_index getType() const = 0;
};

class WindowResizeEvent : public Event
{
  public:
    WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}

    std::string getName() const override { return "WindowResizeEvent"; }
    std::type_index getType() const override { return typeid(WindowResizeEvent); }

    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }

  private:
    int m_Width;
    int m_Height;
};

class WindowCloseEvent : public Event
{
  public:
    WindowCloseEvent() {}

    std::string getName() const override { return "WindowCloseEvent"; }
    std::type_index getType() const override { return typeid(WindowCloseEvent); }
};

class WindowFocusEvent : public Event
{
  public:
    WindowFocusEvent(bool focused) : m_Focused(focused) {}

    std::string getName() const override { return "WindowFocusEvent"; }
    std::type_index getType() const override { return typeid(WindowFocusEvent); }

    bool isFocused() const { return m_Focused; }

  private:
    bool m_Focused;
};

class KeyEvent : public Event
{
  public:
    KeyEvent(int keycode) : m_Keycode(keycode) {}

    std::string getName() const override { return "KeyEvent"; }
    std::type_index getType() const override { return typeid(KeyEvent); }

    int getKeycode() const { return m_Keycode; }

  private:
    int m_Keycode;
};

class KeyPressEvent : public KeyEvent
{
  public:
    KeyPressEvent(int keycode) : KeyEvent(keycode) {}

    std::string getName() const override { return "KeyPressEvent"; }
    std::type_index getType() const override { return typeid(KeyPressEvent); }
};

class KeyRepeatEvent : public KeyEvent
{
  public:
    KeyRepeatEvent(int keycode, float repeatTime) : KeyEvent(keycode), m_repeatTime(repeatTime) {}

    std::string getName() const override { return "KeyRepeatEvent"; }
    std::type_index getType() const override { return typeid(KeyRepeatEvent); }

  private:
    float m_repeatTime = 0.0f; // Repeat time in seconds
};

class KeyReleaseEvent : public KeyEvent
{
  public:
    KeyReleaseEvent(int keycode) : KeyEvent(keycode) {}

    std::string getName() const override { return "KeyReleaseEvent"; }
    std::type_index getType() const override { return typeid(KeyReleaseEvent); }
};

class MouseEvent : public Event
{
  public:
    MouseEvent(int x, int y) : m_X(x), m_Y(y) {}

    std::string getName() const override { return "MouseEvent"; }
    std::type_index getType() const override { return typeid(MouseEvent); }

    int getX() const { return m_X; }
    int getY() const { return m_Y; }

  private:
    int m_X;
    int m_Y;
};

class MouseMoveEvent : public MouseEvent
{
  public:
    MouseMoveEvent(int x, int y) : MouseEvent(x, y) {}

    std::string getName() const override { return "MouseMoveEvent"; }
    std::type_index getType() const override { return typeid(MouseMoveEvent); }
};

class MouseButtonEvent : public MouseEvent
{
  public:
    MouseButtonEvent(int x, int y, int button) : MouseEvent(x, y), m_Button(button) {}

    std::string getName() const override { return "MouseButtonEvent"; }
    std::type_index getType() const override { return typeid(MouseButtonEvent); }

    int getButton() const { return m_Button; }

  private:
    int m_Button;
};

class MouseButtonPressEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressEvent(int x, int y, int button) : MouseButtonEvent(x, y, button) {}

    std::string getName() const override { return "MouseButtonPressEvent"; }
    std::type_index getType() const override { return typeid(MouseButtonPressEvent); }
};

class MouseButtonRepeatEvent : public MouseButtonEvent
{
  public:
    MouseButtonRepeatEvent(int x, int y, int button, int repeatTime) : MouseButtonEvent(x, y, button), m_reapeatTime(repeatTime) {}

    std::string getName() const override { return "MouseButtonRepeatEvent"; }
    std::type_index getType() const override { return typeid(MouseButtonRepeatEvent); }

  private:
    int m_reapeatTime = 0.0f; // Repeat time in seconds
};

class MouseButtonDoubleClickEvent : public MouseButtonEvent
{
  public:
    MouseButtonDoubleClickEvent(int x, int y, int button) : MouseButtonEvent(x, y, button) {}

    std::string getName() const override { return "MouseButtonDoubleClickEvent"; }
    std::type_index getType() const override { return typeid(MouseButtonDoubleClickEvent); }
};

class MouseButtonReleaseEvent : public MouseButtonEvent
{
  public:
    MouseButtonReleaseEvent(int x, int y, int button) : MouseButtonEvent(x, y, button) {}

    std::string getName() const override { return "MouseButtonReleaseEvent"; }
    std::type_index getType() const override { return typeid(MouseButtonReleaseEvent); }
};

class MouseScrollEvent : public MouseEvent
{
  public:
    MouseScrollEvent(int x, int y, int offsetX, int offsetY) : MouseEvent(x, y), m_OffsetX(offsetX), m_OffsetY(offsetY) {}

    std::string getName() const override { return "MouseScrollEvent"; }
    std::type_index getType() const override { return typeid(MouseScrollEvent); }

    int getOffsetX() const { return m_OffsetX; }
    int getOffsetY() const { return m_OffsetY; }

  private:
    int m_OffsetX;
    int m_OffsetY;
};

} // namespace Airwave
