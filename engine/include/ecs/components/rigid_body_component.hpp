#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <btBulletCollisionCommon.h>
#include "ecs/aw_component.hpp"

namespace Airwave
{
enum ColliderType
{
    Sphere,
    Box,
    Capsule,
    Mesh
};

class RigidBodyComponent : public AwComponent
{
  public:
    float mass = 0.0; // 质量, 0表示静态物体
    std::unique_ptr<btRigidBody> rigidBody;           // 刚体
    std::shared_ptr<btCollisionShape> collisionShape; // 碰撞形状
    ColliderType colliderType = ColliderType::Sphere;
    glm::vec3 shapeSize       = glm::vec3(1.0f);
    bool isTrigger            = false;
};

} // namespace Airwave
