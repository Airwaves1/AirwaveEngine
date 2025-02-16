#include "rendering/renderer.hpp"

#include "core/application.hpp"
#include "core/event/event.hpp"
#include "core/event/event_observer.hpp"

namespace Airwave
{

Renderer::Renderer(Application *appContext) : m_appContext(appContext)
{
    FramebufferSpecification spec;
    spec.width       = m_appContext->getWindowWidth();
    spec.height      = m_appContext->getWindowHeight();
    spec.enableDepth = true;

    m_framebuffer = std::make_unique<Framebuffer>(spec);

    TextureSpecification colorSpec;
    colorSpec.width          = spec.width;
    colorSpec.height         = spec.height;
    colorSpec.textureType    = TextureType::TEXTURE_2D;
    colorSpec.format         = TextureFormat::RGBA;
    colorSpec.internalFormat = TextureInternalFormat::RGBA16F;
    colorSpec.wrapS          = TextureWrap::CLAMP_TO_EDGE;
    colorSpec.wrapT          = TextureWrap::CLAMP_TO_EDGE;
    colorSpec.minFilter      = TextureFilter::LINEAR;
    colorSpec.magFilter      = TextureFilter::LINEAR;
    colorSpec.generateMipmap = false;
    colorSpec.enableMSAA     = false;

    auto color = std::make_shared<Texture>(colorSpec);
    m_framebuffer->attachColorTexture(color);

    // gBuffer
    FramebufferSpecification gBufferSpec;
    gBufferSpec.width       = spec.width;
    gBufferSpec.height      = spec.height;
    gBufferSpec.enableDepth = true;
    m_gBuffer               = std::make_unique<Framebuffer>(gBufferSpec);

    // RT0: position(RGBA16F)
    TextureSpecification positionSpec;
    positionSpec.width           = spec.width;
    positionSpec.height          = spec.height;
    positionSpec.textureType     = TextureType::TEXTURE_2D;
    positionSpec.internalFormat  = TextureInternalFormat::RGB16F;
    positionSpec.format          = TextureFormat::RGBA;
    positionSpec.textureDataType = TextureDataType::FLOAT16;
    positionSpec.wrapS           = TextureWrap::CLAMP_TO_EDGE;
    positionSpec.wrapT           = TextureWrap::CLAMP_TO_EDGE;
    positionSpec.minFilter       = TextureFilter::LINEAR;
    positionSpec.magFilter       = TextureFilter::LINEAR;
    auto position                = std::make_shared<Texture>(positionSpec);
    m_gBuffer->attachColorTexture(position, 0);

    // RT1: normal(RGB10_A2)
    TextureSpecification normalSpec;
    normalSpec.width           = spec.width;
    normalSpec.height          = spec.height;
    normalSpec.textureType     = TextureType::TEXTURE_2D;
    normalSpec.internalFormat  = TextureInternalFormat::RGB10_A2;
    normalSpec.format          = TextureFormat::RGBA;
    normalSpec.textureDataType = TextureDataType::UINT_2_10_10_10_REV;
    normalSpec.wrapS           = TextureWrap::CLAMP_TO_EDGE;
    normalSpec.wrapT           = TextureWrap::CLAMP_TO_EDGE;
    normalSpec.minFilter       = TextureFilter::LINEAR;
    normalSpec.magFilter       = TextureFilter::LINEAR;
    auto normal                = std::make_shared<Texture>(normalSpec);
    m_gBuffer->attachColorTexture(normal, 1);

    // RT2: albedo(SRGB8) + metallic(8)
    TextureSpecification albedoSpec;
    albedoSpec.width           = spec.width;
    albedoSpec.height          = spec.height;
    albedoSpec.textureType     = TextureType::TEXTURE_2D;
    albedoSpec.internalFormat  = TextureInternalFormat::SRGB8_ALPHA8;
    albedoSpec.format          = TextureFormat::RGBA;
    albedoSpec.textureDataType = TextureDataType::UINT8;
    albedoSpec.wrapS           = TextureWrap::CLAMP_TO_EDGE;
    albedoSpec.wrapT           = TextureWrap::CLAMP_TO_EDGE;
    albedoSpec.minFilter       = TextureFilter::LINEAR;
    albedoSpec.magFilter       = TextureFilter::LINEAR;
    auto albedo                = std::make_shared<Texture>(albedoSpec);
    m_gBuffer->attachColorTexture(albedo, 2);

    // RT3: parameter(RGBA8) roughness(8) + ao(8) +...
    TextureSpecification paramSpec;
    paramSpec.width          = spec.width;
    paramSpec.height         = spec.height;
    paramSpec.textureType    = TextureType::TEXTURE_2D;
    paramSpec.internalFormat = TextureInternalFormat::RGBA8;
    paramSpec.format         = TextureFormat::RGBA;
    paramSpec.textureDataType = TextureDataType::UINT8;
    paramSpec.wrapS          = TextureWrap::CLAMP_TO_EDGE;
    paramSpec.wrapT          = TextureWrap::CLAMP_TO_EDGE;
    paramSpec.minFilter      = TextureFilter::LINEAR;
    paramSpec.magFilter      = TextureFilter::LINEAR;
    auto roughness           = std::make_shared<Texture>(paramSpec);
    m_gBuffer->attachColorTexture(roughness, 3);

    if (!m_gBuffer->isComplete())
    {
        LOG_ERROR("GBuffer incomplete");
    }

    m_eventObserver = std::make_unique<EventObserver>();
    m_eventObserver->subscribe<WindowResizeEvent>(
        [this](const WindowResizeEvent &event)
        {
            glViewport(0, 0, event.getWidth(), event.getHeight());
            m_framebuffer->resize(event.getWidth(), event.getHeight());
            m_gBuffer->resize(event.getWidth(), event.getHeight());
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
