#pragma once

#include "ecs/aw_system.hpp"
#include "core/common.hpp"

namespace Airwave
{
class Renderer;
class CameraComponent;
class Material;

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

    void forwardRender(Renderer *renderer, CameraComponent &camera);
    void deferredRender(Renderer *renderer, CameraComponent &camera);

    void uploadMaterialUniforms(Renderer *renderer, Material *material, int &slots);
};
} // namespace Airwave