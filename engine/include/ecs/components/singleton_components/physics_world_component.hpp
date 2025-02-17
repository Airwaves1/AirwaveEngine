#pragma once

#include "ecs/aw_component.hpp"
#include <btBulletDynamicsCommon.h>

namespace Airwave
{
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
};

} // namespace Airwave