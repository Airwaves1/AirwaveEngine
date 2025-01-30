#pragma once

#include "ecs/aw_component.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/aw_system.hpp"
#include <glm/glm.hpp>

namespace Airwave
{
class TrackballController : public AwComponent
{
  public:
    TrackballController() = default;

    // 是否启用轨迹球模式
    bool b_useTrackball = true;

    // 目标点
    glm::vec3 target = {0.0f, 0.0f, 0.0f}; // 目标点

    // 相机距离参数
    float radius = 1.0f;      // 当前轨迹球半径
    float minDistance = 0.1f; // 最小距离
    float maxDistance = 100.0f; // 最大距离

    // 交互速度参数
    float rotationSpeed = 0.01f; // 旋转速度
    float zoomSpeed = 0.5f;     // 缩放速度
    float panSpeed = 0.01f;      // 平移速度

    // 鼠标状态
    bool b_mousePressed = false;          // 鼠标是否按下
    glm::vec2 lastMousePosition = {0.0f, 0.0f}; // 上一次鼠标位置
};
} // namespace Airwave
