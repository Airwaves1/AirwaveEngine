#pragma once
#include <glm/glm.hpp>

namespace Airwave
{
struct FirstPersonControllerComponent
{
    constexpr static const char *getComponentName() { return "FirstPersonControllerComponent"; }

    float moveSpeed       = 0.005f;
    float lookSensitivity = 0.005f;
    float jumpForce       = 5.0f;

    float maxPitch = 89.0f;  // Maximum pitch angle for looking up/down
    float minPitch = -89.0f; // Minimum pitch angle for looking up/down

    nlohmann::json to_json() const
    {
        return nlohmann::json{{"moveSpeed", moveSpeed}, {"lookSensitivity", lookSensitivity}, {"jumpForce", jumpForce}};
    }
    void from_json(const nlohmann::json &j)
    {
        moveSpeed       = j.value("moveSpeed", 0.005f);
        lookSensitivity = j.value("lookSensitivity", 0.005f);
        jumpForce       = j.value("jumpForce", 5.0f);
    }

    // 运行时的状态,不需要序列化
    bool isJumping   = false; // 是否正在跳跃
    bool isGrounded  = false; // 是否在地面上
    bool isMoving    = false; // 是否正在移动
    bool isLooking   = false; // 是否正在查看
    bool isSprinting = false; // 是否正在奔跑
    bool isCrouching = false; // 是否正在蹲下

    float pitch = 0.0f; // 当前的俯仰角度
    float yaw   = 0.0f; // 当前的偏航角度
    float roll  = 0.0f; // 当前的滚转角度

    glm::vec3 velocity  = glm::vec3(0.0f); // 当前的速度
    glm::vec3 direction = glm::vec3(0.0f); // 当前的方向


};

} // namespace Airwave
