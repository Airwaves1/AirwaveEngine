#include "rendering/opengl/gl_framebuffer.hpp"
#include "core/log.hpp"
namespace Airwave
{
Framebuffer::Framebuffer(uint32_t width, uint32_t height, const FramebufferSpecification &spec)
{
    m_width  = width;
    m_height = height;
    m_spec   = spec;

    invalidate();
}

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &m_handle); }

void Framebuffer::invalidate()
{
    if (m_handle)
    { // 删除之前的附件
        if (m_colorAttachments.size() > 0)
        {
            for (auto &colorAttachment : m_colorAttachments)
            {
                GLuint handle = colorAttachment->getHandle();
                glDeleteTextures(1, &handle);
            }
        }

        if (m_depthAttachment)
        {
            GLuint handle = m_depthAttachment->getHandle();
            glDeleteTextures(1, &handle);
        }

        if (m_stencilAttachment)
        {
            GLuint handle = m_stencilAttachment->getHandle();
            glDeleteTextures(1, &handle);
        }

        if (m_renderbuffer_handle)
        {
            glDeleteRenderbuffers(1, &m_renderbuffer_handle);
        }

        glDeleteFramebuffers(1, &m_handle);
    }

    glGenFramebuffers(1, &m_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    // 创建颜色附件
    m_colorAttachments.clear();
    m_colorAttachments.reserve(m_spec.colorAttachmentCount);
    for (uint32_t i = 0; i < m_spec.colorAttachmentCount; i++)
    {
        TextureSpecification colorSpec;

        colorSpec.enableMSAA     = m_spec.enableMSAA;
        colorSpec.samples        = m_spec.samples;
        colorSpec.internalFormat = m_spec.colorInternalFormat;
        colorSpec.format         = m_spec.colorFormat;

        auto colorAttachment = std::make_shared<Texture>(m_width, m_height, colorSpec);
        if (m_spec.enableMSAA)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                                   GL_TEXTURE_2D_MULTISAMPLE, colorAttachment->getHandle(), 0);
        }
        else
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                                   colorAttachment->getHandle(), 0);
        }
        m_colorAttachments.push_back(colorAttachment);
    }

    // 创建深度附件
    // 创建并附加深度和/或模板附件
    if (m_spec.depthAttachmentCount > 0 || m_spec.stencilAttachmentCount > 0)
    {
        if (m_spec.depthAttachmentCount > 0 && m_spec.stencilAttachmentCount > 0)
        {
            // 使用renderbuffer作为深度模板附件
            if (m_spec.enableMSAA)
            {
                glGenRenderbuffers(1, &m_renderbuffer_handle);
                glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_handle);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_spec.samples,
                                                 GL_DEPTH24_STENCIL8, m_width, m_height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                          GL_RENDERBUFFER, m_renderbuffer_handle);
            }
            else
            {
                TextureSpecification spec;
                spec.enableMSAA     = false;
                spec.samples        = 1;
                spec.internalFormat = TextureInternalFormat::DEPTH24_STENCIL8;
                spec.format         = TextureFormat::DEPTH;

                if (m_depthAttachment)
                {
                    m_depthAttachment.reset();
                }
                m_depthAttachment = std::make_shared<Texture>(m_width, m_height, spec);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                                       m_depthAttachment->getHandle(), 0);
            }
        }
        else if (m_spec.depthAttachmentCount > 0)
        {
            if (m_spec.enableMSAA)
            {
                glGenRenderbuffers(1, &m_renderbuffer_handle);
                glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_handle);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_spec.samples,
                                                 GL_DEPTH_COMPONENT24, m_width, m_height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                                          m_renderbuffer_handle);
            }
            else
            {
                TextureSpecification spec;
                spec.enableMSAA     = false;
                spec.samples        = 1;
                spec.internalFormat = TextureInternalFormat::DEPTH24;
                spec.format         = TextureFormat::DEPTH;

                if (m_depthAttachment)
                {
                    m_depthAttachment.reset();
                }
                m_depthAttachment = std::make_shared<Texture>(m_width, m_height, spec);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                                       m_depthAttachment->getHandle(), 0);
            }
        }
        else if (m_spec.stencilAttachmentCount > 0)
        {
            if (m_spec.enableMSAA)
            {
                glGenRenderbuffers(1, &m_renderbuffer_handle);
                glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_handle);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_spec.samples, GL_STENCIL_INDEX8,
                                                 m_width, m_height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                          m_renderbuffer_handle);
            }
            else
            {
                TextureSpecification spec;
                spec.enableMSAA     = false;
                spec.samples        = 1;
                spec.internalFormat = TextureInternalFormat::DEPTH24_STENCIL8;
                spec.format         = TextureFormat::DEPTH;

                if (m_stencilAttachment)
                {
                    m_stencilAttachment.reset();
                }
                m_stencilAttachment = std::make_shared<Texture>(m_width, m_height, spec);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                                       m_stencilAttachment->getHandle(), 0);
            }
        }
    }

    // 检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Framebuffer is incomplete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_handle); }

void Framebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::resize(uint32_t width, uint32_t height)
{
    if (m_width == width || m_height == height)
    {
        return;
    }
    m_width  = width;
    m_height = height;

    invalidate(); // 重新创建帧缓冲
}

void Framebuffer::attachColorTexture(uint32_t index, std::shared_ptr<Texture> texture) {
    if (index >= m_colorAttachments.size())
    {
        LOG_WARN("Color attachment index out of range");
        return;
    }

    m_colorAttachments[index] = texture;
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    auto spec = texture->getSpecification();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texture->getHandle(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::attachDepthTexture(std::shared_ptr<Texture> texture) {}

void Framebuffer::attachStencilTexture(std::shared_ptr<Texture> texture) {}

void Framebuffer::attachDepthStencilTexture(std::shared_ptr<Texture> texture) {}

void Framebuffer::setMSAA(bool enableMSAA, uint32_t samples)
{
    m_spec.enableMSAA = enableMSAA;
    m_spec.samples    = samples;

    invalidate();
}

void Framebuffer::resolveMultisample(const std::shared_ptr<Framebuffer>& target)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->getHandle());

    // 解析颜色附件和深度附件时，要注意分开解析,因为深度附件似乎必须只能用GL_NEAREST
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Texture> Framebuffer::getColorAttachment(uint32_t index) const
{
    if (index >= m_colorAttachments.size())
    {
        LOG_WARN("Color attachment index out of range");
        return std::shared_ptr<Texture>();
    }

    return m_colorAttachments[index];
}

std::shared_ptr<Texture> Framebuffer::getDepthAttachment() const { return m_depthAttachment; }

std::shared_ptr<Texture> Framebuffer::getStencilAttachment() const { return m_stencilAttachment; }

} // namespace Airwave