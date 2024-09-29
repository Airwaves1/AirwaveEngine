#pragma once
#include "event/event.hpp"

namespace Airwave
{

class WindowCloseEvent : public Event
{
  public:
    WindowCloseEvent() {}
    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_GET_CATEGORY(EventCategoryApplication)

    std::string toString() const override
    {
        std::stringstream a;
        a << "Window Close";
        return a.str();
    }

  protected:
};

class WindowResizeEvent : public Event
{
  public:
    WindowResizeEvent(int width, int height) : m_Height(height), m_Width(width) {}
    EVENT_CLASS_TYPE(WindowResized)
    EVENT_CLASS_GET_CATEGORY(EventCategoryApplication)

    inline int getWindowWidth() const{ return m_Width; }
    inline int getWindowHeight() const{ return m_Height; }
    std::string toString() const override
    {
        std::stringstream a;
        a << "Window Resize: width = " << m_Width << ", height = " << m_Height;
        return a.str();
    }

  protected:
    int m_Height, m_Width;
};

class WindowFocusEvent : public Event
{
  public:
	WindowFocusEvent() {}
	EVENT_CLASS_TYPE(WindowFocus)
	EVENT_CLASS_GET_CATEGORY(EventCategoryApplication)

	std::string toString() const override
	{
		std::stringstream a;
		a << "Window Focus";
		return a.str();
	}

  protected:
};

class WindowLostFocusEvent : public Event
{
  public:
	WindowLostFocusEvent() {}
	EVENT_CLASS_TYPE(WindowLostFocus)
	EVENT_CLASS_GET_CATEGORY(EventCategoryApplication)

	std::string toString() const override
	{
		std::stringstream a;
		a << "Window Lost Focus";
		return a.str();
	}

  protected:
};

class WindowMoveEvent : public Event
{
  public:
	WindowMoveEvent(int x, int y) : m_X(x), m_Y(y) {}
	EVENT_CLASS_TYPE(WindowMoved)
	EVENT_CLASS_GET_CATEGORY(EventCategoryApplication)

	inline int GetX() { return m_X; }
	inline int GetY() { return m_Y; }

	std::string toString() const override
	{
		std::stringstream a;
		a << "Window Move: x = " << m_X << ", y = " << m_Y;
		return a.str();
	}

  protected:
	int m_X, m_Y;
};



class AppTickEvent : public Event
{
  public:
    AppTickEvent() {}

    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_GET_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
  public:
    AppUpdateEvent() {}

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_GET_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
  public:
    AppRenderEvent() {}

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_GET_CATEGORY(EventCategoryApplication)
};
} // namespace Airwave
