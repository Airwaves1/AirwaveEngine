#include "rendering/opengl/gl_framebuffer.hpp"
#include "core/log.hpp"
#include <glad/glad.h>
namespace Airwave
{

Framebuffer::Framebuffer(FramebufferSpecification spec) {}

Framebuffer::~Framebuffer() {}

void Framebuffer::invalidate() {
    if(m_handle != 0) {
        glDeleteFramebuffers(1, &m_handle);
    }
    if(m_colorAttachments.size() > 0) {
        glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
    }
    if(m_depthAttachment != 0) {
        glDeleteTextures(1, &m_depthAttachment);
    }

    m_handle = 0;
    m_depthAttachment = 0;
    m_colorAttachments.clear();

    glGenFramebuffers(1, &m_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

    bool enableMSAA = m_spec.enableMSAA;

    if(m_spec.attachments.size() > 0) {
        m_colorAttachments.resize(m_spec.attachments.size());
        glGenTextures(m_spec.attachments.size(), m_colorAttachments.data());

        for(size_t i = 0; i < m_spec.attachments.size(); i++) {
            glBindTexture(GL_TEXTURE_2D, m_colorAttachments[i]);

            if(enableMSAA) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_spec.samples, static_cast<GLint>(m_spec.attachments[i].internalFormat), m_spec.width, m_spec.height, GL_FALSE);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.attachments[i].internalFormat), m_spec.width, m_spec.height, 0, static_cast<GLenum>(m_spec.attachments[i].format), static_cast<GLenum>(m_spec.attachments[i].dataType), nullptr);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.attachments[i].wrapS));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.attachments[i].wrapT));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_spec.attachments[i].minFilter));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_spec.attachments[i].magFilter));

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorAttachments[i], 0);
        }
    }

}

} // namespace Airwave
