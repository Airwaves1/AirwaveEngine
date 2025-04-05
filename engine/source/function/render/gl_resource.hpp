#pragma once

#include "core/resource/resource.hpp"
#include <glad/glad.h>

namespace Airwave
{
class IGLResource : public IResource
{
  public:
    IGLResource(GLenum gl_type, const std::string &resource_path) : IResource(resource_path), m_gl_type(gl_type), m_handle(0) {}
    virtual ~IGLResource() = default;
    virtual void release() = 0;
    virtual GLuint getHandle() const { return m_handle; }
    virtual GLenum getGLType() const { return m_gl_type; }

  protected:
    GLenum m_gl_type = GL_NONE;
    GLuint m_handle  = 0;
};

} // namespace Airwave
