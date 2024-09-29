#include "event_dispatcher.hpp"

namespace Airwave
{
void EventDispatcher::removeObserver(std::shared_ptr<EventObserver> observer)
{
    for (auto &handlers : m_observerHandlers)
    {
        handlers.second.erase(std::remove_if(handlers.second.begin(), handlers.second.end(),
                                             [observer](const EventHandler &handler)
                                             { return handler.observer == observer; }),
                              handlers.second.end());
    }
}

void EventDispatcher::dispatch(Event &event)
{
    auto it = m_observerHandlers.find(event.GetEventType());
    if (it != m_observerHandlers.end())
    {
        for (auto &handler : it->second)
        {
            handler.callback(event);
        }
    }
}
} // namespace Airwave
