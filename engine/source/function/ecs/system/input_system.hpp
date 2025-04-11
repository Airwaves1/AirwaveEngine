#pragma once

#include "function/global/global_context.hpp"
#include "function/ecs/system.hpp"
#include "function/ecs/component/single_component/input_component.hpp"
#include "function/ecs/world.hpp"

#include "core/event/event.hpp"
#include "core/event/event_bus.hpp"
namespace Airwave
{
class InputSystem : public System
{
  public:
    InputSystem()
    {
        EventBus::getInstance().subscribe<KeyPressEvent>(
            [](const KeyPressEvent &event)
            {
                auto &reg = GlobalContext.world->getRegistry();
                reg.view<InputComponent>().each(
                    [&](auto entity, InputComponent &input)
                    {
                        input.inputStates[event.getKeycode()]    = true;
                        input.inputHeldTimes[event.getKeycode()] = 0.0f;
                    });
            });
        EventBus::getInstance().subscribe<KeyReleaseEvent>(
            [](const KeyReleaseEvent &event)
            {
                auto &reg = GlobalContext.world->getRegistry();
                reg.view<InputComponent>().each(
                    [&](auto entity, InputComponent &input)
                    {
                        input.inputStates[event.getKeycode()] = false;
                        input.inputHeldTimes.erase(event.getKeycode());
                    });
            });
        EventBus::getInstance().subscribe<MouseMoveEvent>(
            [](const MouseMoveEvent &event)
            {
                auto &reg = GlobalContext.world->getRegistry();
                reg.view<InputComponent>().each(
                    [&](auto entity, InputComponent &input)
                    {
                        glm::vec2 newMousePosition = glm::vec2(event.getX(), event.getY());
                        input.mouseDelta           = newMousePosition - input.mousePosition;
                        input.mousePosition        = newMousePosition;
                    });
            });
        EventBus::getInstance().subscribe<MouseScrollEvent>(
            [](const MouseScrollEvent &event)
            {
                auto &reg = GlobalContext.world->getRegistry();
                reg.view<InputComponent>().each([&](auto entity, InputComponent &input)
                                                { input.mouseScroll = glm::vec2(event.getOffsetX(), event.getOffsetY()); });
            });

        EventBus::getInstance().subscribe<MouseButtonPressEvent>(
            [](const MouseButtonPressEvent &event)
            {
                auto &reg = GlobalContext.world->getRegistry();
                reg.view<InputComponent>().each(
                    [&](auto entity, InputComponent &input)
                    {
                        input.inputStates[event.getButton()]     = true;
                        input.inputHeldTimes[event.getButton()]  = 0.0f;
                        input.mouseDelta                         = glm::vec2(0.0f); // 重置鼠标增量
                    });
            });
        EventBus::getInstance().subscribe<MouseButtonReleaseEvent>(
            [](const MouseButtonReleaseEvent &event)
            {
                auto &reg = GlobalContext.world->getRegistry();
                reg.view<InputComponent>().each(
                    [&](auto entity, InputComponent &input)
                    {
                        input.inputStates[event.getButton()] = false;
                        input.inputHeldTimes.erase(event.getButton());
                    });
            });
    }
    ~InputSystem() override = default;

    void onUpdate(std::weak_ptr<World> world, float delta_time) override {}
};

} // namespace Airwave
