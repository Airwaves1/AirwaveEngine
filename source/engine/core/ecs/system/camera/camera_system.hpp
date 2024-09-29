#pragma once

#include "ecs/system/aw_system.hpp"
#include "utils/log.hpp"

namespace Airwave
{
class CameraSystem : public AwSystem
{
  public:
    CameraSystem() = default;

    virtual ~CameraSystem() = default;

    virtual void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) override
    {
        LOG_DEBUG("CameraSystem onUpdate");

    }
};

} // namespace Airwave
