#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "core/event/event.hpp"
#include "core/event/event_bus.hpp"

namespace Airwave
{
struct InputComponent
{
    // constexpr static const char *name = "InputComponent";
    constexpr static const char *getComponentName() { return "InputComponent"; }

    InputComponent()
    {
        EventBus::getInstance().subscribe<KeyPressEvent>(
            [this](const KeyPressEvent &event)
            {
                inputStates[event.getKeycode()]    = true;
                inputHeldTimes[event.getKeycode()] = 0.0f;
            });
        EventBus::getInstance().subscribe<KeyReleaseEvent>(
            [this](const KeyReleaseEvent &event)
            {
                inputStates[event.getKeycode()] = false;
                inputHeldTimes.erase(event.getKeycode());
            });
        EventBus::getInstance().subscribe<MouseMoveEvent>(
            [this](const MouseMoveEvent &event)
            {
                glm::vec2 newMousePosition = glm::vec2(event.getX(), event.getY());
                mouseDelta                 = newMousePosition - mousePosition;
                mousePosition              = newMousePosition;
            });
        EventBus::getInstance().subscribe<MouseScrollEvent>([this](const MouseScrollEvent &event)
                                                            { mouseScroll = glm::vec2(event.getOffsetX(), event.getOffsetY()); });

        EventBus::getInstance().subscribe<MouseButtonPressEvent>(
            [this](const MouseButtonPressEvent &event)
            {
                inputStates[event.getButton()]    = true;
                inputHeldTimes[event.getButton()] = 0.0f;
            });
        EventBus::getInstance().subscribe<MouseButtonReleaseEvent>(
            [this](const MouseButtonReleaseEvent &event)
            {
                inputStates[event.getButton()] = false;
                inputHeldTimes.erase(event.getButton());
            });
    }

    void updateHeldTimes(float deltaTime)
    {
        for (auto &[key, isPress] : inputStates)
        {
            if (isPress)
                inputHeldTimes[key] += deltaTime;
            else
                inputHeldTimes[key] = 0.0f; // 重置保持时间
        }
    }

    bool isKeyPressed(int keycode) const
    {
        auto it = inputStates.find(keycode);
        return it != inputStates.end() && it->second;
    }

    bool isKeyHeld(int keycode, float requiredHeldTime = 0.5) const
    {
        auto it = inputHeldTimes.find(keycode);
        return it != inputHeldTimes.end() && it->second >= requiredHeldTime;
    }

    std::unordered_map<int, bool> inputStates;     // 输入状态
    std::unordered_map<int, float> inputHeldTimes; // 输入保持时间

    glm::vec2 mousePosition; // 鼠标位置
    glm::vec2 mouseDelta;    // 鼠标移动增量
    glm::vec2 mouseScroll;   // 鼠标滚轮增量
};

} // namespace Airwave
