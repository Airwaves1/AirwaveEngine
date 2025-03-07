#pragma once

#include "core/common.hpp"
#include "core/log.hpp"
#include "rendering/passes/pass.hpp"
namespace Airwave
{
class PostProcessEffect
{
  public:
    PostProcessEffect()
    {
        FramebufferSpecification spec;
        spec.width       = 1920;
        spec.height      = 1080;

        framebuffer = std::make_shared<Framebuffer>(spec);
    }
    virtual ~PostProcessEffect() = default;

    std::vector<std::shared_ptr<Pass>> passes = {};

    std::shared_ptr<Texture> readBuffer  = nullptr;
    std::shared_ptr<Texture> writeBuffer = nullptr;

    std::shared_ptr<Framebuffer> framebuffer = nullptr;

    std::shared_ptr<Pass> addPass(std::shared_ptr<Pass> pass)
    {
        passes.push_back(pass);
        return pass;
    }

    void render(AwScene *scene, Renderer *renderer)
    {
        framebuffer->bind();
        for (auto &pass : passes)
        {
            pass->render(scene, renderer, framebuffer, readBuffer, writeBuffer);
        }

        framebuffer->unbind();

        renderToScreen(renderer);
    }

    void setSize(uint32_t width, uint32_t height)
    {
        auto spec = framebuffer->getSpecification();
        if (spec.width == width && spec.height == height)
        {
            return;
        }

        framebuffer->resize(width, height);

        for (auto &pass : passes)
        {
            pass->setSize(width, height);
        }
    }

    void renderToScreen(Renderer *renderer)
    {
        if (!renderer) return;

        auto main_framebuffer = renderer->getFramebuffer(); // 获取主帧缓冲区
        if (!framebuffer || !main_framebuffer)
        {
            std::cerr << "Error: Invalid framebuffer" << std::endl;
            return;
        }

        auto read_spec  = framebuffer->getSpecification();
        auto write_spec = main_framebuffer->getSpecification();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->getHandle());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, main_framebuffer->getHandle());

        if (glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERROR("PostProcessEffect::renderToScreen: read framebuffer is not complete");
            return;
        }

        if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERROR("PostProcessEffect::renderToScreen: draw framebuffer is not complete");
            return;
        }


        // 清空默认帧缓冲区，避免残留
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, main_framebuffer->getHandle());
        glClear(GL_COLOR_BUFFER_BIT);

        // 进行 Blit 操作
        glBlitFramebuffer(0, 0, read_spec.width, read_spec.height, 0, 0, write_spec.width, write_spec.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // 解绑帧缓冲区，避免影响后续渲染
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};

} // namespace Airwave
