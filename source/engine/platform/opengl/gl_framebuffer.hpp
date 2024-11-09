#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <unordered_map>
#include "gl_texture.hpp"

namespace Airwave
{
enum class Target : GLenum
{
    FRAMEBUFFER      = GL_FRAMEBUFFER,
    READ_FRAMEBUFFER = GL_READ_FRAMEBUFFER,
    DRAW_FRAMEBUFFER = GL_DRAW_FRAMEBUFFER,
};

struct FramebufferSpecification
{

    uint32_t colorAttachmentCount             = 1;
    TextureInternalFormat colorInternalFormat = TextureInternalFormat::RGBA8;
    TextureFormat colorFormat                 = TextureFormat::RGBA;

    uint32_t depthAttachmentCount   = 1;
    uint32_t stencilAttachmentCount = 0;
    bool enableMSAA                 = false;
    uint32_t samples                = 1;

    FramebufferSpecification(
        uint32_t colorAttachmentCount             = 1,
        TextureInternalFormat colorInternalFormat = TextureInternalFormat::RGBA8,
        TextureFormat colorFormat = TextureFormat::RGBA, uint32_t depthAttachmentCount = 1,
        uint32_t stencilAttachmentCount = 0, bool enableMSAA = false, uint32_t samples = 1)
        : colorAttachmentCount(colorAttachmentCount), colorInternalFormat(colorInternalFormat),
          colorFormat(colorFormat), depthAttachmentCount(depthAttachmentCount),
          stencilAttachmentCount(stencilAttachmentCount), enableMSAA(enableMSAA), samples(samples)
    {
    }
};

class Framebuffer
{
  public:
    Framebuffer() = default;
    Framebuffer(uint32_t width, uint32_t height, const FramebufferSpecification &spec);
    ~Framebuffer();

    void bind();
    void unbind();

    uint32_t getHandle() const { return m_handle; }

    void resize(uint32_t width, uint32_t height);

    void attachColorTexture(uint32_t index, std::shared_ptr<Texture> texture);

    void attachDepthTexture(std::shared_ptr<Texture> texture);

    void attachStencilTexture(std::shared_ptr<Texture> texture);

    void attachDepthStencilTexture(std::shared_ptr<Texture> texture);

    std::shared_ptr<Texture> getColorAttachment(uint32_t index = 0) const;

    std::shared_ptr<Texture> getDepthAttachment() const;

    std::shared_ptr<Texture> getStencilAttachment() const;

    const FramebufferSpecification &getSpecification() const { return m_spec; }

    uint32_t getWidth() const { return m_width; }

    uint32_t getHeight() const { return m_height; }

    void setMSAA(bool enableMSAA, uint32_t samples);

    void resolveMultisample(const std::shared_ptr<Framebuffer>& target);

  private:
    void invalidate(); // 用于重置framebuffer

    uint32_t m_handle              = 0;
    uint32_t m_renderbuffer_handle = 0;

    uint32_t m_width, m_height;
    std::vector<std::shared_ptr<Texture>> m_colorAttachments;
    std::shared_ptr<Texture> m_depthAttachment;
    std::shared_ptr<Texture> m_stencilAttachment;
    FramebufferSpecification m_spec;
};

} // namespace Airwave
