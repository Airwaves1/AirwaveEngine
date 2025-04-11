#pragma once

#include <string>
#include <typeindex>
#include <functional>
#include <queue>
#include <unordered_map>
#include <mutex>
#include "event.hpp"

namespace Airwave
{
class EventBus
{
  public:
    template <typename T> using EventCallback = std::function<void(const T &)>; // 允许接受具体的事件类型

    static EventBus &getInstance()
    {
        static EventBus instance;
        return instance;
    }

    template <typename T> void subscribe(const EventCallback<T> &callback)
    {
        std::type_index eventType = typeid(T);
        m_listeners[eventType].push_back(
            [callback](Event &event)
            {
                callback(static_cast<const T &>(event)); // 安全转换
            });
    }

    template <typename T> void unsubscribe(const EventCallback<T> &callback)
    {
        std::type_index eventType = typeid(T);
        auto &callbacks           = m_listeners[eventType];
        callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    }

    void postEvent(Event &event)
    {
        auto eventType = event.getType();
        auto it        = m_listeners.find(eventType);
        if (it != m_listeners.end())
        {
            for (auto &callback : it->second)
            {
                callback(event);
            }
        }
    }

    void queueEvent(std::unique_ptr<Event> event)
    {
        m_eventQueue.push(std::move(event));
    }

    void dispatchEvents()
    {
        while (!m_eventQueue.empty())
        {
            auto event = std::move(m_eventQueue.front());
            m_eventQueue.pop();

            if (event)
            {
                auto eventType = event->getType();
                auto it        = m_listeners.find(eventType);
                if (it != m_listeners.end())
                {
                    for (auto &callback : it->second)
                    {
                        callback(*event);
                    }
                }
            }
        }
    }

  private:
    EventBus()  = default;
    ~EventBus() = default;

    EventBus(const EventBus &)            = delete;
    EventBus &operator=(const EventBus &) = delete;

    std::unordered_map<std::type_index, std::vector<std::function<void(Event &)>>> m_listeners;
    std::queue<std::unique_ptr<Event>> m_eventQueue;
    std::mutex m_mutex;
};

} // namespace Airwave
