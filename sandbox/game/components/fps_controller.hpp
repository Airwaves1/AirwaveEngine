#pragma once

#include "ecs/aw_component.hpp"
#include <glm/glm.hpp>
namespace Airwave
{
class FPSControllerComponent : public AwComponent
{
  public:
    // 配置参数
    float moveSpeed      = 5.0f; // 基础移动速度
    float sensitivity    = 0.1f; // 鼠标灵敏度
    float fastMultiplier = 2.0f; // 加速移动时的倍率

    // 运行时状态
    float yaw   = -90.0f; // 偏航角
    float pitch = 0.0f;   // 俯仰角

    glm::vec3 front = {0.0f, 0.0f, -1.0f}; // 前向向量
    glm::vec3 up    = {0.0f, 1.0f, 0.0f};  // 上向向量
    glm::vec3 right = {1.0f, 0.0f, 0.0f};  // 右向向量

    bool firstMouse             = true;         // 首次鼠标输入标志
    glm::vec2 lastMousePosition = {0.0f, 0.0f}; // 上一次鼠标位置

    bool b_useFPSController = true; // 是否使用FPS控制器
};

} // namespace Airwave
