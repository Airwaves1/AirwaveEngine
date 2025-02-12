#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>
#include "rendering/opengl/gl_texture.hpp"
#include "core/common.hpp"
#include "resource/texture_resource.hpp"

namespace Airwave
{
enum class Target : GLenum
{
    FRAMEBUFFER      = GL_FRAMEBUFFER,
    READ_FRAMEBUFFER = GL_READ_FRAMEBUFFER,
    DRAW_FRAMEBUFFER = GL_DRAW_FRAMEBUFFER,
};

class Framebuffer
{
  public:
    Framebuffer(FramebufferSpecification spec);
    ~Framebuffer();


  private:
    void invalidate(); // 用于重置framebuffer

    uint32_t m_handle              = 0;

    std::vector<TextureResource> m_colorAttachments;

    FramebufferSpecification m_spec;
};

} // namespace Airwave
