#pragma once

#include <glm/glm.hpp>

namespace Airwave
{
enum ColliderType
{
    Sphere,
    Box,
    Capsule,
    Mesh
};

class RigidBodyComponent
{
  public:
    float mass                = 1.0f;      // 质量
    glm::vec3 velocity        = {0, 0, 0}; // 速度
    glm::vec3 angularVelocity = {0, 0, 0}; // 角速度
    bool isKinematic          = false;     // 是否是动力学刚体
    float linearDamping       = 0.01f;     // 线性阻尼
    float angularDamping      = 0.05f;     // 角阻尼
};

} // namespace Airwave
