// event_dispatcher.hpp
#pragma once
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <mutex>
#include <functional>

namespace Airwave {

class EventDispatcher {
public:
    using Callback = std::function<void(Event&)>;

    struct Handler {
        const void* owner;     // 直接保存原始指针
        Callback callback;
    };

    static EventDispatcher& GetInstance() {
        static EventDispatcher instance;
        return instance;
    }

    // 注册事件监听器
    template <typename T>
    void addListener(const void* owner, std::function<void(T&)> callback) {
        std::lock_guard lock(mutex_);
        handlers_[typeid(T)].push_back({
            owner,
            [callback](Event& e) { callback(static_cast<T&>(e)); }
        });
    }

    // 移除特定对象的所有监听器
    void removeListener(const void* owner) {
        std::lock_guard lock(mutex_);
        for (auto& [type, list] : handlers_) {
            auto it = std::remove_if(list.begin(), list.end(),
                [owner](const Handler& h) { return h.owner == owner; });
            list.erase(it, list.end());
        }
    }

    // 分发事件
    void dispatch(Event& event) {
        std::lock_guard lock(mutex_);
        const auto& type = typeid(event);
        if (auto it = handlers_.find(type); it != handlers_.end()) {
            for (const auto& handler : it->second) {
                handler.callback(event);
            }
        }
    }

private:
    std::mutex mutex_;
    std::unordered_map<std::type_index, std::vector<Handler>> handlers_;
};

} // namespace Airwave