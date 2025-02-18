#pragma once

namespace Airwave
{
class AwScene;
class AwSystem
{
  public:
    virtual ~AwSystem() = default;

    virtual void onInit() {}
  protected:
    AwSystem() = default;

    virtual void onUpdate(float deltaTime) = 0;
    virtual void afterAllSystemsUpdated(float deltaTime) {}

  protected:
    AwScene *m_scene;
    friend class AwScene;
};
} // namespace Airwave
