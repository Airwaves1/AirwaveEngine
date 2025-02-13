#include "rendering/renderer.hpp"

#include "core/application.hpp"
#include "core/event/event.hpp"
#include "core/event/event_observer.hpp"

namespace Airwave
{

Renderer::Renderer(Application *appContext) : m_appContext(appContext)
{
    FramebufferSpecification spec;
    spec.width  = m_appContext->getWindowWidth();
    spec.height = m_appContext->getWindowHeight();
    spec.enableDepth = true;

    m_framebuffer = std::make_unique<Framebuffer>(spec);

    m_eventObserver = std::make_unique<EventObserver>();
    m_eventObserver->subscribe<WindowResizeEvent>(
        [this](const WindowResizeEvent &event)
        {
            glViewport(0, 0, event.getWidth(), event.getHeight());
            m_framebuffer->resize(event.getWidth(), event.getHeight());
        });

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}
Renderer::~Renderer() {}

void Renderer::clear()
{
    glClearColor(m_rendererParams.clearColor.r, m_rendererParams.clearColor.g, m_rendererParams.clearColor.b, 1.0f);
    glClear(m_rendererParams.clearMask);
}

void Renderer::bindTexture2D(uint32_t texture, uint32_t unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Renderer::bindTextureCubeMap(uint32_t texture, uint32_t unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

} // namespace Airwave
