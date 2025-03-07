#pragma once
#include "core/common.hpp"
#include "ecs/aw_scene.hpp"
#include "rendering/renderer.hpp"
#include "rendering/texture.hpp"
#include "rendering/framebuffer.hpp"
#include <memory>

namespace Airwave
{
class Pass
{
  public:
    Pass()  = default;
    ~Pass() = default;

    virtual void render(AwScene *scene, Renderer *renderer, std::shared_ptr<Framebuffer>frambuffer, std::shared_ptr<Texture> readBuffer,
                        std::shared_ptr<Texture> writeBuffer) = 0;

    virtual void setSize(uint32_t width, uint32_t height) {}
};
} // namespace Airwave
