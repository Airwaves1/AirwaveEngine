// event_observer.hpp
#pragma once
#include "event_dispatcher.hpp"

namespace Airwave {

class EventObserver {
public:
    virtual ~EventObserver() {
        // 析构时自动注销所有监听器
        EventDispatcher::GetInstance().removeListener(this);
    }

    template <typename T>
    void subscribe(std::function<void(T&)> callback) {
        EventDispatcher::GetInstance().addListener<T>(this, callback);
    }
};

} // namespace Airwave