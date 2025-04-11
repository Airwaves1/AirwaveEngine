#pragma once

#include "function/ecs/system.hpp"

#include <GLFW/glfw3.h>

#include "function/ecs/component/transform_component.hpp"
#include "function/ecs/component/first_person_controller.hpp"
#include "function/ecs/component/single_component/input_component.hpp"
#include "function/ecs/world.hpp"

namespace Airwave
{
class MovementSystem : public System
{
  public:
    MovementSystem()           = default;
    ~MovementSystem() override = default;

    void onUpdate(std::weak_ptr<World> world, float delta_time) override
    {
        auto w = world.lock();
        if (!w)
        {
            return;
        }

        auto &registry = w->getRegistry();

        // auto input = w->tryGetSingletonComponent<InputComponent>();
        auto input = registry.ctx().find<InputComponent>();
        if (input)
        {
            auto view = registry.view<FirstPersonControllerComponent, TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto &controller = view.get<FirstPersonControllerComponent>(entity);
                auto &transform  = view.get<TransformComponent>(entity);

                glm::vec3 position = transform.getPosition();

                // 处理鼠标输入
                if (input->isKeyPressed(GLFW_MOUSE_BUTTON_1))
                {
                    controller.pitch -= input->mouseDelta.y * controller.lookSensitivity;
                    controller.yaw -= input->mouseDelta.x * controller.lookSensitivity;

                    // 限制俯仰角度
                    if (controller.pitch > controller.maxPitch) controller.pitch = controller.maxPitch;
                    if (controller.pitch < controller.minPitch) controller.pitch = controller.minPitch;

                    transform.setRotation(glm::angleAxis(glm::radians(controller.yaw), glm::vec3(0, 1, 0)) *
                                          glm::angleAxis(glm::radians(controller.pitch), glm::vec3(1, 0, 0)));
                }

                // 处理键盘输入
                if (input->isKeyPressed(GLFW_KEY_W)) position += transform.getForward() * controller.moveSpeed;
                if (input->isKeyPressed(GLFW_KEY_S)) position -= transform.getForward() * controller.moveSpeed;
                if (input->isKeyPressed(GLFW_KEY_A)) position -= transform.getRight() * controller.moveSpeed;
                if (input->isKeyPressed(GLFW_KEY_D)) position += transform.getRight() * controller.moveSpeed;
                if (input->isKeyPressed(GLFW_KEY_E)) position += transform.getUp() * controller.moveSpeed;
                if (input->isKeyPressed(GLFW_KEY_Q)) position -= transform.getUp() * controller.moveSpeed;
                transform.setPosition(position);
            }
        }
    }
};

} // namespace Airwave
