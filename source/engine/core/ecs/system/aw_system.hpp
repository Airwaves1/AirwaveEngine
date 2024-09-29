#pragma once

#include "ecs/scene.hpp"

namespace Airwave
{
class AwSystem
{
  public:
    AwSystem() = default;

    virtual ~AwSystem() = default;

    virtual void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) = 0;

  protected:
    int m_priority;
};

} // namespace Airwave
