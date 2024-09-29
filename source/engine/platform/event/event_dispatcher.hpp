#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include "event/event.hpp"
#include "utils/log.hpp"

namespace Airwave
{
class EventObserver;

// 定义事件回调类型为指向函数的指针，
// 这些函数都接受一个Event类型的参数
using EventCallback = std::function<void(Event &)>;

struct EventHandler
{
    std::shared_ptr<EventObserver> observer;
    EventCallback callback;
};

// 负责管理和分发事件。维护了一个事件到处理程序的映射，并处理事件的注册、注销以及分发。
class EventDispatcher
{
  public:
    EventDispatcher(const EventDispatcher &)            = delete;
    EventDispatcher &operator=(const EventDispatcher &) = delete;
    ~EventDispatcher()                                  = default;

    static EventDispatcher &GetInstance()
    {
        static EventDispatcher instance;
        return instance;
    }

    // 注册观察事件处理函数
    template <typename EventType>
    void addObserver(std::shared_ptr<EventObserver> observer,
                     const std::function<void(const EventType &)> &callback)
    {
        if (!observer || !callback)
        {
            LOG_WARN("Invalid observer or callback");
            return;
        }

        // Lambda 包装器，用于将基类事件对象转换为具体的事件类型
        EventCallback eventCallback = [callback](Event &baseEvent)
        {
            const auto &event = static_cast<EventType &>(baseEvent);
            callback(event);
        };

        EventHandler handler{observer, eventCallback};
        m_observerHandlers[EventType::GetStaticType()].push_back(handler);
    }

    // 销毁观察者
    void removeObserver(std::shared_ptr<EventObserver> observer);

    // 分发事件
    void dispatch(Event &event);

  private:
    EventDispatcher() = default;

    // 使用unordered_map来存储每种事件类型的观察者和处理函数
    std::unordered_map<EventType, std::vector<EventHandler>> m_observerHandlers;
};

} // namespace Airwave
