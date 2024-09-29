#pragma once
#include "event/event_dispatcher.hpp"
#include "event/event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "event/application_event.hpp"
namespace Airwave
{
class EventDispatcher;
class EventObserver : public std::enable_shared_from_this<EventObserver>
{
  public:
    EventObserver() = default;

    // 析构时自动从事件分发器中移除
    virtual ~EventObserver()
    {
        if (auto self = weak_from_this().lock()) // 检查 weak_ptr 是否有效
        {
            EventDispatcher::GetInstance().removeObserver(self);
        }
    }

    // 订阅事件
    template <typename T> void onEvent(const std::function<void(const T &)> &callback)
    {
        auto self = shared_from_this(); // 确保在 lambda 中持有当前对象的引用
        EventDispatcher::GetInstance().addObserver<T>(
            self,                            // 传递共享指针
            [self, callback](const T &event)
            {
                callback(event); // 直接调用回调函数
            });
    }
};
} // namespace Airwave