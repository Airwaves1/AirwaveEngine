#pragma once

#include "opengl/gl_framebuffer.hpp"
#include "opengl/gl_vertex_array.hpp"
#include "opengl/gl_shader.hpp"

namespace Airwave
{

class FullScreenQuad
{
  public:
    FullScreenQuad();
    ~FullScreenQuad() {}

    void setRenderTarget(const std::shared_ptr<Framebuffer> &framebuffer = nullptr);
    void setShader(const std::shared_ptr<Shader> &shader);

    void render();

    void renderToScreen();

  private:
    std::shared_ptr<VertexArray> m_vertexArray = nullptr;
    std::shared_ptr<Shader> m_shader = nullptr;

    std::shared_ptr<Framebuffer> m_framebuffer = nullptr;
    
};

} // namespace Airwave
