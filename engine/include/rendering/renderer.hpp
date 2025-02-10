#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "rendering/opengl/gl_framebuffer.hpp"
#include "rendering/opengl/gl_uniforms.hpp"
#include "core/common.hpp"
namespace Airwave
{
class Application;
class EventObserver;

class Renderer
{
  public:
    struct RendererParams
    {
        glm::vec3 clearColor = {0.1f, 0.1f, 0.1f};
        GLenum clearMask     = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    };

    uint64_t drawCalls = 0;

    Renderer(Application *appContext);
    ~Renderer();

    void clear();
    void setClearColor(const glm::vec3 &color) { m_rendererParams.clearColor = color; }
    void setClearMask(GLenum mask) { m_rendererParams.clearMask = mask; }

    void enable(GLenum cap) { glEnable(cap); }
    void disable(GLenum cap) { glDisable(cap); }

    void setDepthFunc(DepthFunc func) { glDepthFunc(static_cast<GLenum>(func)); }
    void setDepthMask(bool flag) { glDepthMask(flag); }

    void setCullFace(CullFace face) { glCullFace(static_cast<GLenum>(face)); }

    void setViewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }

    Framebuffer *getFramebuffer() { return m_framebuffer.get(); }

    Application *getApplication() { return m_appContext; }

    void bindShader(GLuint program) { glUseProgram(program); }
    void bindTexture2D(uint32_t texture, uint32_t unit);
    void unBindTexture2D() { glBindTexture(GL_TEXTURE_2D, 0); }
    void bindTextureCubeMap(uint32_t texture, uint32_t unit);
    void unBindTextureCubeMap() { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

    void set(const std::string &name,const UniformValue &value) { m_uniforms.set(name, value); }
    const GLUniforms &getUniforms() const { return m_uniforms; }
    void uploadUniforms(GLuint program) { m_uniforms.upload(program); }

  private:
    RendererParams m_rendererParams;

    GLUniforms m_uniforms;

    Application *m_appContext;

    std::unique_ptr<Framebuffer> m_framebuffer;

    std::unique_ptr<EventObserver> m_eventObserver;
};

} // namespace Airwave
