#include "rendering/framebuffer.hpp"
#include "core/log.hpp"
#include "core/common.hpp"
namespace Airwave
{
Framebuffer::Framebuffer(FramebufferSpecification spec) : m_spec(spec)
{
    glGenFramebuffers(1, &m_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    if (m_spec.enableDepth)
    {
        TextureSpecification depthSpec;
        depthSpec.width          = m_spec.width;
        depthSpec.height         = m_spec.height;
        depthSpec.textureType    = TextureType::TEXTURE_2D;
        depthSpec.format         = TextureFormat::DEPTH;
        depthSpec.internalFormat = TextureInternalFormat::DEPTH24;
        depthSpec.wrapS          = TextureWrap::CLAMP_TO_EDGE;
        depthSpec.wrapT          = TextureWrap::CLAMP_TO_EDGE;
        depthSpec.minFilter      = TextureFilter::NEAREST;
        depthSpec.magFilter      = TextureFilter::NEAREST;
        depthSpec.generateMipmap = false;
        depthSpec.enableMSAA     = false;

        m_depthAttachment = std::make_shared<Texture>(depthSpec);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment->getHandle(), 0);
    }

    TextureSpecification colorSpec;
    colorSpec.width          = m_spec.width;
    colorSpec.height         = m_spec.height;
    colorSpec.textureType    = TextureType::TEXTURE_2D;
    colorSpec.format         = TextureFormat::RGBA;
    colorSpec.internalFormat = TextureInternalFormat::RGBA16F;
    colorSpec.wrapS          = TextureWrap::CLAMP_TO_EDGE;
    colorSpec.wrapT          = TextureWrap::CLAMP_TO_EDGE;
    colorSpec.minFilter      = TextureFilter::LINEAR;
    colorSpec.magFilter      = TextureFilter::LINEAR;
    colorSpec.generateMipmap = false;
    colorSpec.enableMSAA     = false;

    auto color = std::make_shared<Texture>(colorSpec);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->getHandle(), 0);

    m_colorAttachments.push_back(color);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Framebuffer incomplete");
    }
}

Framebuffer::~Framebuffer()
{
    if (m_handle != 0)
    {
        glDeleteFramebuffers(1, &m_handle);
    }

    if (m_depthAttachment)
    {
        m_depthAttachment.reset();
    }

    m_colorAttachments.clear();
}

void Framebuffer::attachColorTexture(std::shared_ptr<Texture> texture, uint32_t index)
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texture->getHandle(), 0);
    m_colorAttachments.push_back(texture);
    unbind();
}

void Framebuffer::attachDepthTexture(std::shared_ptr<Texture> texture)
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getHandle(), 0);
    m_depthAttachment = texture;
    unbind();
}

void Framebuffer::resize(uint32_t width, uint32_t height)
{
    m_spec.width  = width;
    m_spec.height = height;

    for (auto &texture : m_colorAttachments)
    {
        texture->resize(width, height);
    }

    if (m_depthAttachment)
    {
        m_depthAttachment->resize(width, height);
    }
}

void Framebuffer::clear() const
{
    bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &texture : m_colorAttachments)
    {
        texture->bind();
        glClear(GL_COLOR_BUFFER_BIT);
    }

    if (m_depthAttachment)
    {
        m_depthAttachment->bind();
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    unbind();
}

void Framebuffer::blitToScreen(uint32_t width, uint32_t height) const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, m_spec.width, m_spec.height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Airwave
