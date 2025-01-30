#pragma once

#include "ecs/aw_component.hpp"

#include "core/event/event.hpp"
#include "core/event/event_observer.hpp"

#include <unordered_map>
#include <glm/glm.hpp>

namespace Airwave
{
class InputComponent : public AwComponent
{
  public:
    glm::vec4 reactArea = glm::vec4(0.0f, 0.0f, 1200.0f, 800.0f); // 反应区域

    InputComponent()
        : mousePosition(0.0f), mouseDelta(0.0f), mouseScroll(0.0f), mouseButtons{}, keyStates{},
          keyHeldDurations{}, mouseHeldDurations{}
    {
        m_eventObserver = std::make_shared<EventObserver>();

        m_eventObserver->subscribe<KeyPressEvent>(
            [this](KeyPressEvent &event)
            {
                if (mousePosition.x < reactArea.x || mousePosition.x > reactArea.z ||
                    mousePosition.y < reactArea.y || mousePosition.y > reactArea.w)
                    return;
                setKeyState(event.getKey(), true);
            });
        m_eventObserver->subscribe<KeyReleaseEvent>(
            [this](KeyReleaseEvent &event)
            {
                if (mousePosition.x < reactArea.x || mousePosition.x > reactArea.z ||
                    mousePosition.y < reactArea.y || mousePosition.y > reactArea.w)
                    return;
                setKeyState(event.getKey(), false);
            });

        m_eventObserver->subscribe<MouseMoveEvent>(
            [this](MouseMoveEvent &event)
            { setMousePosition(glm::vec2(event.getX(), event.getY())); });

        m_eventObserver->subscribe<MouseScrollEvent>(
            [this](MouseScrollEvent &event)
            {
                if (mousePosition.x < reactArea.x || mousePosition.x > reactArea.z ||
                    mousePosition.y < reactArea.y || mousePosition.y > reactArea.w)
                    return;
                setMouseScroll(glm::vec2(event.getXOffset(), event.getYOffset()));
            });

        m_eventObserver->subscribe<MouseButtonPressEvent>(
            [this](MouseButtonPressEvent &event)
            {
                if (mousePosition.x < reactArea.x || mousePosition.x > reactArea.z ||
                    mousePosition.y < reactArea.y || mousePosition.y > reactArea.w)
                    return;
                setMouseButton(event.getButton(), true);
            });

        m_eventObserver->subscribe<MouseButtonReleaseEvent>(
            [this](MouseButtonReleaseEvent &event)
            {
                if (mousePosition.x < reactArea.x || mousePosition.x > reactArea.z ||
                    mousePosition.y < reactArea.y || mousePosition.y > reactArea.w)
                    return;
                setMouseButton(event.getButton(), false);
            });
    }

    // 键盘状态
    std::unordered_map<int, bool> keyStates;           // 按键是否按下
    std::unordered_map<int, float> keyHeldDurations;   // 累计按键按下时间
    std::unordered_map<int, bool> mouseButtons;        // 鼠标按键状态
    std::unordered_map<int, float> mouseHeldDurations; // 累计鼠标按键按下时间

    // 鼠标状态
    glm::vec2 mousePosition; // 当前鼠标位置
    glm::vec2 mouseDelta;    // 鼠标移动的增量
    glm::vec2 mouseScroll;   // 鼠标滚轮

    // 查询按键是否按下
    bool isKeyPressed(int key) const
    {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second;
    }

    // 查询鼠标按键是否按下
    bool isMouseButtonPressed(int button) const
    {
        auto it = mouseButtons.find(button);
        return it != mouseButtons.end() && it->second;
    }

    // 查询是否长按
    bool isKeyHeld(int key, float requiredDuration) const
    {
        auto it = keyHeldDurations.find(key);
        return it != keyHeldDurations.end() && it->second >= requiredDuration;
    }

    bool isMouseButtonHeld(int button, float requiredDuration) const
    {
        auto it = mouseHeldDurations.find(button);
        return it != mouseHeldDurations.end() && it->second >= requiredDuration;
    }

    // 在系统更新中调用，用于更新累计时间
    void updateDurations(float deltaTime)
    {
        for (auto &[key, isPressed] : keyStates)
        {
            if (isPressed)
                keyHeldDurations[key] += deltaTime;
            else
                keyHeldDurations[key] = 0.0f; // 重置未按下的按键
        }

        for (auto &[button, isPressed] : mouseButtons)
        {
            if (isPressed)
                mouseHeldDurations[button] += deltaTime;
            else
                mouseHeldDurations[button] = 0.0f; // 重置未按下的鼠标键
        }
    }

  private:
    void setKeyState(int key, bool isPressed)
    {
        keyStates[key] = isPressed;
        if (!isPressed) keyHeldDurations[key] = 0.0f;
    }

    void setMouseButton(int button, bool isPressed)
    {
        mouseButtons[button] = isPressed;
        if (!isPressed) mouseHeldDurations[button] = 0.0f;
    }

    void setMousePosition(const glm::vec2 &newPosition)
    {
        mouseDelta    = newPosition - mousePosition;
        mousePosition = newPosition;
    }

    void setMouseScroll(const glm::vec2 &scroll) { mouseScroll = scroll; }

    std::shared_ptr<EventObserver> m_eventObserver;

    friend class InputSystem;
};
} // namespace Airwave
