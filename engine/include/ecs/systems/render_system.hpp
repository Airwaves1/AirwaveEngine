#pragma once

#include "ecs/aw_system.hpp"
#include <glm/glm.hpp>

namespace Airwave
{
class Renderer;
class CameraComponent;

class RenderSystem : public AwSystem
{
  public:
    RenderSystem()  = default;
    ~RenderSystem() = default;

  protected:
    void onUpdate(float deltaTime) override;

  private:
    uint64_t drawCalls = 0;
    void renderBackground(Renderer *renderer, CameraComponent &camera);
    void renderScene(CameraComponent &camera);

    // void forwardRender(Renderer *renderer, CameraComponent &camera);
    // void deferredRender(Renderer *renderer, CameraComponent &camera);
};
} // namespace Airwave