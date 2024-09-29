#pragma once

#include "event/event.hpp"

namespace Airwave
{
enum MouseButton
{
    MOUSE_BUTTON_LEFT = 0,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE
};

class MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(float xPos, float yPos) : m_xPos(xPos), m_yPos(yPos) {}
    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_GET_CATEGORY(EventCategoryInput | EventCategoryMouse)

    std::string toString() const override
    {
        std::stringstream a;
        a << "MouseMovedEvent: xPos = " << getX() << ", yPos = " << getY();
        return a.str();
    }

    inline float getX() const { return m_xPos; }
    inline float getY() const { return m_yPos; }

  private:
    float m_xPos, m_yPos;
};

class MouseButtonEvent : public Event
{
  public:
    EVENT_CLASS_GET_CATEGORY(EventCategoryInput | EventCategoryMouseButton)
    MouseButtonEvent(MouseButton button, float xPos, float yPos) : m_Button(button), m_XPos(xPos), m_YPos(yPos) {}
    virtual ~MouseButtonEvent() {}
    inline MouseButton GetMouseButton() const { return m_Button; };
    inline float GetXPos() const { return m_XPos; }
    inline float GetYPos() const { return m_YPos; }

  protected:
    MouseButton m_Button;
    float m_XPos, m_YPos;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressedEvent(MouseButton button, float xPos, float yPos) : MouseButtonEvent(button, xPos, yPos) {}
    EVENT_CLASS_TYPE(MouseButtonPressed)

    MouseButton GetMouseButton() const { return m_Button; }

    std::string toString() const override
    {
        std::stringstream a;
        a << "MouseButtonPressed";
        return a.str();
    }

  private:
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
  public:
    MouseButtonReleasedEvent(MouseButton button, float xPos, float yPos) : MouseButtonEvent(button, xPos, yPos) {}
    EVENT_CLASS_TYPE(MouseButtonReleased)

    std::string toString() const override
    {
        std::stringstream a;
        a << "MouseButtonReleased";
        return a.str();
    }

  private:
};

class MouseScrolledEvent : public Event
{
  public:
    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_GET_CATEGORY(EventCategoryInput | EventCategoryMouseButton)

    MouseScrolledEvent(float xOffset, float yOffset) : m_OffsetX(xOffset), m_OffsetY(yOffset) {}
    std::string toString() const override
    {
        std::stringstream a;
        a << "MouseScrolled: xOffset = " << getOffsetX() << ", yOffset = " << getOffsetY();
        return a.str();
    }

    inline float getOffsetX() const { return m_OffsetX; }
    inline float getOffsetY() const { return m_OffsetY; }

  private:
    float m_OffsetX, m_OffsetY;
};

} // namespace Airwave
