#pragma once

#include "ecs/aw_system.hpp"
#include "ecs/aw_scene.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/components/singleton_components/input_component.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Airwave
{
class InputSystem : public AwSystem
{
  public:
    InputSystem() = default;

    void onUpdate(float deltaTime) override
    {
        auto adminEntity = m_scene->getAdminEntity();
        if (adminEntity->hasComponent<InputComponent>())
        {
            auto &input = adminEntity->getComponent<InputComponent>();
            input.updateDurations(deltaTime);
        }
    }

    void afterAllSystemsUpdated(float deltaTime) override
    {
        auto adminEntity = m_scene->getAdminEntity();
        if (adminEntity->hasComponent<InputComponent>())
        {
            auto &input       = adminEntity->getComponent<InputComponent>();
            input.mouseDelta  = glm::vec2(0.0f);
            input.mouseScroll = glm::vec2(0.0f);
        }
    }
};
} // namespace Airwave
