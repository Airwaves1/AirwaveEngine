#include "rendering/renderer.hpp"

#include "core/application.hpp"
#include "core/event/event.hpp"
#include "core/event/event_observer.hpp"

namespace Airwave
{

Renderer::Renderer(Application *appContext) : m_appContext(appContext)
{
    FramebufferSpecification spec;
    // spec.enableMSAA          = true;
    // spec.samples             = 4;
    spec.colorInternalFormat = TextureInternalFormat::RGBA16F;
    spec.colorFormat         = TextureFormat::RGBA;

    m_framebuffer   = std::make_unique<Framebuffer>(m_appContext->getWindowWidth(),
                                                    m_appContext->getWindowHeight(), spec);
    m_eventObserver = std::make_unique<EventObserver>();
    m_eventObserver->subscribe<WindowResizeEvent>(
        [this](const WindowResizeEvent &event)
        { m_framebuffer->resize(event.getWidth(), event.getHeight()); });
}
Renderer::~Renderer() {}

void Renderer::clear()
{
    glClearColor(m_rendererParams.clearColor.r, m_rendererParams.clearColor.g,
                 m_rendererParams.clearColor.b, 1.0f);
    glClear(m_rendererParams.clearMask);
}

} // namespace Airwave
