#pragma once

#include <glad/glad.h>
#include <vector>
#include "core/common.hpp"

#include "rendering/texture.hpp"

namespace Airwave
{
class Framebuffer
{
  public:
    Framebuffer(FramebufferSpecification spec);
    ~Framebuffer();

    void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_handle); }
    void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    uint32_t getHandle() const { return m_handle; }

    void attachColorTexture(std::shared_ptr<Texture> texture, uint32_t index = 0);
    void attachDepthTexture(std::shared_ptr<Texture> texture);

    std::shared_ptr<Texture> getColorAttachment(uint32_t index = 0) const
    {
        if (index < m_colorAttachments.size())
        {
            return m_colorAttachments[index];
        }
        return nullptr;
    }

    void resize(uint32_t width, uint32_t height);

    void clear() const;

    void blitToScreen(uint32_t width, uint32_t height) const;

  private:
    uint32_t m_handle;

    std::vector<std::shared_ptr<Texture>> m_colorAttachments;
    std::shared_ptr<Texture> m_depthAttachment;

    FramebufferSpecification m_spec;
};

} // namespace Airwave