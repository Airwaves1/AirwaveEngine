#pragma once

#include "ecs/aw_scene.hpp"

namespace Airwave
{
class AwSystem
{
  public:
    virtual ~AwSystem() = default;

  protected:
    AwSystem() = default;

    virtual void onInit() {}
    virtual void onUpdate(float deltaTime) = 0;
    virtual void afterAllSystemsUpdated (float deltaTime) {}

  protected:
    AwScene *m_scene;
    friend class AwScene;
};
} // namespace Airwave
