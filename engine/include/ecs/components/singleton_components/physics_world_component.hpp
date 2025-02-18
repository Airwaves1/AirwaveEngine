#pragma once

#include "ecs/aw_component.hpp"
#include <entt/entt.hpp>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

namespace Airwave
{
struct RaycastHit
{
    bool hasHit         = false;
    glm::vec3 hitPoint  = glm::vec3(0.0f);
    glm::vec3 hitNormal = glm::vec3(0.0f);

    entt::entity entity = entt::null;

    float hitFraction = 1.0f; // 碰撞点距离
};

class PhysicsWorldComponent : public AwComponent
{
  public:
    btDefaultCollisionConfiguration *collisionConfiguration; // 碰撞配置
    btCollisionDispatcher *dispatcher;                       // 碰撞分发
    btBroadphaseInterface *overlappingPairCache;             // 碰撞检测
    btSequentialImpulseConstraintSolver *solver;             // 约束求解
    btDiscreteDynamicsWorld *dynamicsWorld;                  // 动力学世界
    bool enableDebugDraw = false;                            // 是否启用调试绘制

    PhysicsWorldComponent()
    {
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher             = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache   = new btDbvtBroadphase();
        solver                 = new btSequentialImpulseConstraintSolver();
        dynamicsWorld          = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
    }

    ~PhysicsWorldComponent()
    {
        // delete dynamicsWorld;
        // delete solver;
        // delete overlappingPairCache;
        // delete dispatcher;
        // delete collisionConfiguration;
    }

    RaycastHit raycast(const glm::vec3 &origin, const glm::vec3 &direction, float distance)
    {
        btVector3 btOrigin(origin.x, origin.y, origin.z);
        btVector3 btDirection(direction.x, direction.y, direction.z);

        // 射线检测回调
        btCollisionWorld::ClosestRayResultCallback rayCallback(btOrigin, btOrigin + btDirection * distance);

        // 执行射线检测
        dynamicsWorld->rayTest(btOrigin, btOrigin + btDirection * distance, rayCallback);

        RaycastHit hitInfo;
        if (rayCallback.hasHit())
        {
            hitInfo.hasHit     = true;
            hitInfo.hitPoint   = glm::vec3(rayCallback.m_hitPointWorld.getX(), rayCallback.m_hitPointWorld.getY(), rayCallback.m_hitPointWorld.getZ());
            hitInfo.hitNormal  = glm::vec3(rayCallback.m_hitNormalWorld.getX(), rayCallback.m_hitNormalWorld.getY(), rayCallback.m_hitNormalWorld.getZ());
            hitInfo.hitFraction = rayCallback.m_closestHitFraction;

            auto entity = static_cast<entt::entity>(rayCallback.m_collisionObject->getUserIndex());
            hitInfo.entity = entity;
        }

        return hitInfo;
    }
    
};

} // namespace Airwave