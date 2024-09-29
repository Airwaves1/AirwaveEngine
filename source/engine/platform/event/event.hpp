#pragma once

#include <string>
#include <sstream>
#include <functional>

#define BIT(x) 1 << x 

#define EVENT_CLASS_TYPE(type) \
static EventType GetStaticType() { return EventType::type; }\
const EventType GetEventType() const override { return GetStaticType(); }\
const char* GetName() const override { return #type; }

#define EVENT_CLASS_GET_CATEGORY(category) int GetCategoryFlag() const override { return category;} 


namespace Airwave
{
// 事件的详细类型
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResized, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// 事件的粗略种类
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
        EventCategoryInput			= BIT(1),
        EventCategoryKeyboard		= BIT(2),
        EventCategoryMouse			= BIT(3),
        EventCategoryMouseButton	= BIT(4)
	};


	class Event
	{
	public:
		virtual const char* GetName() const = 0;
		virtual const EventType GetEventType() const = 0;
		virtual int GetCategoryFlag() const = 0;
		virtual std::string toString() const { return GetName(); }
		inline bool IsInCategory(EventCategory type) 
		{
			return GetCategoryFlag() & type;
		}

		bool IsHandled() { return m_Handled; }
		void MarkHandled() { m_Handled = true; }

	protected:
		bool m_Handled = false;//用来标记这个事件是否被处理了
	};

}
