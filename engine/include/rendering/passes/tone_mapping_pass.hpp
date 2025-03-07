#pragma once
#include "core/common.hpp"
#include "utils/geometry_utils.hpp"
#include "rendering/passes/pass.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/primitive.hpp"
namespace Airwave
{
class ToneMappingPass : public Pass
{
  public:
    ToneMappingPass(std::shared_ptr<Shader> shader = nullptr) : m_shader(shader)
    {
        TextureSpecification spec;
        spec.format          = TextureFormat::RGBA;
        spec.internalFormat  = TextureInternalFormat::RGBA16F;
        spec.textureDataType = TextureDataType::FLOAT;
        spec.width           = 1920;
        spec.height          = 1080;
        m_renderTarget       = std::make_shared<Texture>(spec);

        m_quad = GeometryUtils::CreateQuad(1.0f, 1.0f, 1, 1);

        glGenFramebuffers(1, &m_testfbo);
    }

    ~ToneMappingPass() {}

    virtual void render(AwScene *scene, Renderer *renderer, std::shared_ptr<Framebuffer> frambuffer, std::shared_ptr<Texture> readBuffer,
                        std::shared_ptr<Texture> writeBuffer) override
    {
        if (!m_shader) return;
        uint32_t fbo = frambuffer->getHandle();

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTarget->getHandle(), 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(0, 0, m_renderTarget->getSpec().width, m_renderTarget->getSpec().height);

        renderer->bindShader(m_shader->getHandle());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderer->getFramebuffer()->getColorAttachment(0)->getHandle());
        renderer->set("u_texture", 0);
        renderer->set("u_exposure", 1.0f);
        renderer->set("u_tonemapMode", 3);
        renderer->uploadUniforms(m_shader->getHandle());

        m_quad->draw();
    }

    void setSize(uint32_t width, uint32_t height) override
    {
        if (m_renderTarget) m_renderTarget->resize(width, height);
    }

    // private:
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Texture> m_renderTarget;
    std::shared_ptr<Primitive> m_quad;
    uint32_t m_testfbo = 0;
};

} // namespace Airwave
