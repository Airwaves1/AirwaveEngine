#pragma once

#include <GLFW/glfw3.h>
#include "ecs/aw_system.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/aw_component.hpp"

#include "ecs/components/camera_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/singleton_components/input_component.hpp"
#include "fps_controller.hpp"

namespace Airwave
{
class FPSControllerSystem : public AwSystem
{
  public:
    virtual void onUpdate(float deltaTime) override
    {
        auto &reg         = m_scene->getRegistry();
        auto admin_entity = m_scene->getAdminEntity();
        auto view         = reg.view<FPSControllerComponent, TransformComponent>();

        auto &input = reg.get<InputComponent>(admin_entity);

        for (auto entity : view)
        {
            auto &fpsController = view.get<FPSControllerComponent>(entity);
            auto &transform     = view.get<TransformComponent>(entity);

            if (!fpsController.b_useFPSController) continue;

            // 处理鼠标输入
            if (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
            {
                if (fpsController.firstMouse)
                {
                    fpsController.lastMousePosition = input.mousePosition;
                    fpsController.firstMouse        = false;
                }

                glm::vec2 mouseDelta            = input.mousePosition - fpsController.lastMousePosition;
                fpsController.lastMousePosition = input.mousePosition;

                mouseDelta *= fpsController.sensitivity;

                fpsController.yaw += mouseDelta.x;
                fpsController.pitch -= mouseDelta.y;

                if (fpsController.pitch > 89.0f) fpsController.pitch = 89.0f;
                if (fpsController.pitch < -89.0f) fpsController.pitch = -89.0f;

                // updateVectors(fpsController.yaw, fpsController.pitch, transform);
                glm::quat rotation = calculateRotation(fpsController.yaw, fpsController.pitch, fpsController);
                transform.setRotation(rotation);
            }
            else
            {
                fpsController.firstMouse = true;
            }

            // 处理键盘输入
            glm::vec3 position = transform.getPosition();
            float speed = input.isKeyPressed(GLFW_KEY_LEFT_SHIFT) ? fpsController.moveSpeed * fpsController.fastMultiplier : fpsController.moveSpeed;

            if (input.isKeyPressed(GLFW_KEY_W)) position += transform.getForward() * speed * deltaTime;
            if (input.isKeyPressed(GLFW_KEY_S)) position -= transform.getForward() * speed * deltaTime;
            if (input.isKeyPressed(GLFW_KEY_A)) position -= transform.getRight() * speed * deltaTime;
            if (input.isKeyPressed(GLFW_KEY_D)) position += transform.getRight() * speed * deltaTime;
            if (input.isKeyPressed(GLFW_KEY_SPACE)) position += transform.getUp() * speed * deltaTime;
            if (input.isKeyPressed(GLFW_KEY_Q)) position -= transform.getUp() * speed * deltaTime;
            if (input.isKeyPressed(GLFW_KEY_E)) position += glm::vec3(0.0f, 1.0f, 0.0f) * speed * deltaTime;
            if (input.isKeyPressed(GLFW_KEY_R)) position = glm::vec3(0.0f);
            transform.setPosition(position);
        }
    }

  private:

    glm::quat calculateRotation(float yaw, float pitch, FPSControllerComponent &fpsController)
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front   = glm::normalize(front);

        // 计算右向量和上向量
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up    = glm::normalize(glm::cross(right, front));

        fpsController.front = front;
        fpsController.right = right;
        fpsController.up    = up;

        return glm::quatLookAt(front, up);
    }
};

} // namespace Airwave
