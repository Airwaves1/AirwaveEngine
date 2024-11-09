#include "full_screen_quad.hpp"
#include "resource/shader_lib.hpp"
#include "resource/texture_lib.hpp"
#include <vector>

namespace Airwave
{

FullScreenQuad::FullScreenQuad()
{

    std::vector<float> vertices = {
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f,
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,
    };

    std::string vertexSrc = R"(
        #version 410 core

        layout(location = 0) in vec2 a_position;
        layout(location = 1) in vec2 a_texCoord;

        out vec2 v_texCoord;

        void main()
        {
            gl_Position = vec4(a_position, 0.0, 1.0);
            v_texCoord = a_texCoord;
        }
    )";

    std::string fragmentSrc = R"(
        #version 410 core

        layout(location = 0) out vec4 color;

        in vec2 v_texCoord;

        uniform sampler2D u_t_diffuse;
        uniform sampler2D u_t_depth;

        void main()
        {

            float gamma = 2.2;

            float exposure = 1.0;

            vec4 texColor = texture(u_t_diffuse, v_texCoord);

            // 曝光色调映射
            vec3 mapped = vec3(1.0) - exp(-texColor.rgb * exposure);

            // gamma校正
            mapped = pow(mapped, vec3(1.0 / gamma));

            color = vec4(mapped, texColor.a);
        }
    )";

    m_shader = SHADER_LIB.load("FullScreenQuad", vertexSrc, fragmentSrc, false);
    m_shader->bind();
    m_shader->uploadUniformInt("u_t_diffuse", 0);
    m_shader->uploadUniformInt("u_t_depth", 1);

    m_vertexArray = std::make_shared<VertexArray>();
    {
        m_vertexArray->bind();
        std::shared_ptr<VertexBuffer> vertexBuffer =
            std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
        vertexBuffer->setLayout(
            {{ShaderDataType::Float2, "a_position"}, {ShaderDataType::Float2, "a_texCoord"}});
        m_vertexArray->addVertexBuffer(vertexBuffer);

        std::shared_ptr<IndexBuffer> indexBuffer =
            std::make_shared<IndexBuffer>(indices.data(), indices.size());
        m_vertexArray->setIndexBuffer(indexBuffer);
        m_vertexArray->unbind();
    }
}

void FullScreenQuad::setRenderTarget(const std::shared_ptr<Framebuffer> &framebuffer)
{
    if (framebuffer->getSpecification().enableMSAA)
    {
        if (!m_framebuffer)
        {
            FramebufferSpecification spec;
            spec.colorAttachmentCount   = framebuffer->getSpecification().colorAttachmentCount;
            spec.depthAttachmentCount   = 1;
            spec.stencilAttachmentCount = 0;
            spec.enableMSAA             = false;
            spec.samples                = 1;
            m_framebuffer               = std::make_shared<Framebuffer>(framebuffer->getWidth(),
                                                                        framebuffer->getHeight(), spec);
        }

        m_framebuffer->resize(framebuffer->getWidth(), framebuffer->getHeight());

        framebuffer->resolveMultisample(m_framebuffer);
    }
    else
    {
        m_framebuffer = framebuffer;
    }
}

void FullScreenQuad::setShader(const std::shared_ptr<Shader> &shader) { m_shader = shader; }

void FullScreenQuad::render()
{
    if (!m_shader)
    {
        LOG_ERROR("No shader attached to FullScreenQuad");
        return;
    }

    if (!m_framebuffer)
    {
        LOG_ERROR("No framebuffer attached to FullScreenQuad");
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    m_framebuffer->bind();

    m_shader->bind();

    if (m_framebuffer->getSpecification().depthAttachmentCount > 0)
    {
        m_framebuffer->getDepthAttachment()->bind(1);
    }

    m_framebuffer->getColorAttachment()->bind(0);

    m_vertexArray->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    m_vertexArray->unbind();

    m_framebuffer->unbind();

    glEnable(GL_DEPTH_TEST);
}

void FullScreenQuad::renderToScreen()
{
    if (!m_shader)
    {
        LOG_ERROR("No shader attached to FullScreenQuad");
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    m_shader->bind();
    auto colorAttachment = m_framebuffer->getColorAttachment(0);
    if (colorAttachment)
    {
        colorAttachment->bind(0);
    }
    else
    {
        LOG_ERROR("No color attachment found in framebuffer");
    }
    auto depthAttachment = m_framebuffer->getDepthAttachment();
    if (depthAttachment)
    {
        depthAttachment->bind(1);
    }

    m_vertexArray->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
} // namespace Airwave
