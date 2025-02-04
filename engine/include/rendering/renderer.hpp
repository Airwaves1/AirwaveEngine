#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "rendering/opengl/gl_framebuffer.hpp"

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

  private:
    RendererParams m_rendererParams;

    Application *m_appContext;

    std::unique_ptr<Framebuffer> m_framebuffer;

    std::unique_ptr<EventObserver> m_eventObserver;
};

} // namespace Airwave
