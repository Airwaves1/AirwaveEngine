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

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Framebuffer incomplete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
bool Framebuffer::isComplete() const
{
    bind();
    bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    unbind();
    return complete;
}

void Framebuffer::attachColorTexture(std::shared_ptr<Texture> texture, uint32_t index)
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texture->getHandle(), 0);
    m_colorAttachments[index] = texture;

    uint32_t attachmentCount = m_colorAttachments.size();
    std::vector<GLenum> drawBuffers;
    for (uint32_t i = 0; i < attachmentCount; i++)
    {
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(attachmentCount, drawBuffers.data());
}

void Framebuffer::attachDepthTexture(std::shared_ptr<Texture> texture)
{
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getHandle(), 0);
    m_depthAttachment = texture;

}

void Framebuffer::resize(uint32_t width, uint32_t height)
{
    if (m_spec.width == width && m_spec.height == height)
    {
        return;
    }
    m_spec.width  = width;
    m_spec.height = height;

    // 释放资源重新创建和附加
    if (m_depthAttachment)
    {
        m_depthAttachment->resize(width, height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment->getHandle(), 0);
    }

    uint32_t attachmentCount = m_colorAttachments.size();
    for (uint32_t i = 0; i < attachmentCount; i++)
    {
        m_colorAttachments[i]->resize(width, height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorAttachments[i]->getHandle(), 0);
    }

    if (!isComplete())
    {
        LOG_ERROR("Framebuffer incomplete");
    }
}

void Framebuffer::clear() const
{
    bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &texture : m_colorAttachments)
    {
        texture.second->bind();
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

void Airwave::Framebuffer::dispose()
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

} // namespace Airwave
