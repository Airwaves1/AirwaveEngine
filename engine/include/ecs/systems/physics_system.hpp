#pragma once

#include <entt/entt.hpp>
#include "ecs/aw_system.hpp"

namespace Airwave
{

class PhysicsSystem : public AwSystem
{
  public:
    PhysicsSystem()  = default;
    ~PhysicsSystem() = default;

    void createRigidBody(AwScene *scene, entt::entity entity);
    void onUpdate(float deltaTime) override;
    void processCollisionEvent();
};

} // namespace Airwave
