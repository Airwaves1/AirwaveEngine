#pragma once

#include "function/render/gl_resource.hpp"
#include <vector>
#include <string>
#include "uniform/uniform.hpp"

namespace Airwave
{
enum class ShaderType : GLenum
{
    VERTEX   = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    COMPUTE  = GL_COMPUTE_SHADER
};

class Shader : public IGLResource
{
  public:
    Shader(const std::string &resource_path = "");
    ~Shader() override;

    virtual void load() override;
    virtual void reload() override;
    virtual void release() override;

    GLuint compileShader(ShaderType type, const std::string &source);
    GLuint linkProgram(const std::vector<GLuint> &shaders);

    void bind() const;
    void unbind() const;

    void setUniform(const std::string &name, const Uniform::UniformData &data)
    {
        m_uniforms.Set(name, data);
    }

    void commitUniforms()
    {
        m_uniforms.Commit(m_handle);
    }

  private:
    static Uniform m_uniforms;
};

} // namespace Airwave
