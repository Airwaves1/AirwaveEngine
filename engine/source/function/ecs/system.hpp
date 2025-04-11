#pragma once

#include <memory>

namespace Airwave
{
class World;
class System
{
  public:
    virtual ~System() = default;

    virtual void onUpdate(std::weak_ptr<World> world, float delta_time) = 0;

    int priority() const { return m_priority; }

  protected:
    System()       = default;
    int m_priority = 0; // 系统优先级，数值越小优先级越高
    friend class World;
};

} // namespace Airwave