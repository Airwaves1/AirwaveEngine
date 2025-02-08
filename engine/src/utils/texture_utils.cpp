#include "utils/texture_utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/application.hpp"
#include "resource/resource_manager.hpp"
#include "rendering/opengl/gl_framebuffer.hpp"
#include "rendering/opengl/gl_shader.hpp"
#include "core/common.hpp"
#include "utils/shapes_vao.hpp"
#include "utils/geometry_utils.hpp"
#include "rendering/opengl/gl_vertex_array.hpp"

namespace Airwave
{
std::shared_ptr<Texture> TextureUtils::equirectangularToCubemap(Renderer *renderer,
                                                                std::shared_ptr<Texture> equirectangular,
                                                                uint32_t resolution, bool isHDR)
{
    auto &equirectSpec = equirectangular->getSpecification();
    if (equirectSpec.textureType != TextureType::TEXTURE_2D)
    {
        LOG_ERROR("TextureUtils::equirectangularToCubemap: equirectangular texture is not 2D");
        return nullptr;
    }

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[]    = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    auto vertexArray = ShapesVAO::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);

    auto shader = ResourceManager::GetInstance().loadShader("equirect",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/vert/"
                                                                             "cube_map.vert",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/frag/"
                                                                             "equirectangular_to_"
                                                                             "cubemap.frag");

    TextureSpecification cube_map_spec;
    cube_map_spec.isHDR       = isHDR;
    cube_map_spec.textureType = TextureType::TEXTURE_CUBE_MAP;
    cube_map_spec.wrapS       = TextureWrap::CLAMP_TO_EDGE;
    cube_map_spec.wrapT       = TextureWrap::CLAMP_TO_EDGE;
    cube_map_spec.wrapR       = TextureWrap::CLAMP_TO_EDGE;
    auto cube_map             = std::make_shared<Texture>(resolution, resolution, cube_map_spec);

    shader->bind();
    shader->setUniformInt("u_equirectangularMap", 0);
    shader->setUniformMat4("u_projectionMatrix", captureProjection);

    equirectangular->bind(0);

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, resolution, resolution);

    for (uint32_t i = 0; i < 6; i++)
    {
        shader->setUniformMat4("u_viewMatrix", captureViews[i]);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               cube_map->getHandle(), 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vertexArray->bind();
        glDrawElements(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto app        = renderer->getApplication();
    uint32_t width  = app->getWindowWidth();
    uint32_t height = app->getWindowHeight();

    glViewport(0, 0, width, height);

    return cube_map;
}

std::shared_ptr<Texture> TextureUtils::irradianceConvolution(Renderer *renderer, std::shared_ptr<Texture> envMap,
                                                             uint32_t resolution)
{
    auto &envMapSpec = envMap->getSpecification();
    if (envMapSpec.textureType != TextureType::TEXTURE_CUBE_MAP)
    {
        LOG_ERROR("TextureUtils::irradianceConvolution: envMap texture is not cube map");
        return nullptr;
    }

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[]    = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    auto vertexArray = ShapesVAO::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);

    auto shader = ResourceManager::GetInstance().loadShader("irradiance_convolution",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/vert/"
                                                                             "cube_map.vert",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/frag/"
                                                                             "irradiance_"
                                                                             "convolution.frag");

    TextureSpecification irradiance_spec;
    irradiance_spec.isHDR       = envMapSpec.isHDR;
    irradiance_spec.textureType = TextureType::TEXTURE_CUBE_MAP;
    irradiance_spec.wrapS       = TextureWrap::CLAMP_TO_EDGE;
    irradiance_spec.wrapT       = TextureWrap::CLAMP_TO_EDGE;
    irradiance_spec.wrapR       = TextureWrap::CLAMP_TO_EDGE;

    auto irradiance_map = std::make_shared<Texture>(resolution, resolution, irradiance_spec);

    shader->bind();
    shader->setUniformInt("u_environmentMap", 0);
    shader->setUniformMat4("u_projectionMatrix", captureProjection);

    envMap->bind(0);

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, resolution, resolution);

    for (uint32_t i = 0; i < 6; i++)
    {
        shader->setUniformMat4("u_viewMatrix", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               irradiance_map->getHandle(), 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vertexArray->bind();
        glDrawElements(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto app        = renderer->getApplication();
    uint32_t width  = app->getWindowWidth();
    uint32_t height = app->getWindowHeight();

    glViewport(0, 0, width, height);

    return irradiance_map;
}

std::shared_ptr<Texture> TextureUtils::prefilterEnvMap(Renderer *renderer, std::shared_ptr<Texture> envMap,
                                                       uint32_t resolution, uint32_t maxMipLevels = 5)
{
    auto &envMapSpec = envMap->getSpecification();
    if (envMapSpec.textureType != TextureType::TEXTURE_CUBE_MAP)
    {
        LOG_ERROR("TextureUtils::prefilterEnvMap: envMap texture is not cube map");
        return nullptr;
    }

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[]    = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    auto vertexArray = ShapesVAO::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);

    auto shader = ResourceManager::GetInstance().loadShader("prefilter_env_map",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/vert/"
                                                                             "cube_map.vert",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/frag/"
                                                                             "prefilter_envmap.frag");

    TextureSpecification prefilter_spec;
    prefilter_spec.isHDR          = envMapSpec.isHDR;
    prefilter_spec.textureType    = TextureType::TEXTURE_CUBE_MAP;
    prefilter_spec.generateMipmap = true;
    prefilter_spec.wrapS          = TextureWrap::CLAMP_TO_EDGE;
    prefilter_spec.wrapT          = TextureWrap::CLAMP_TO_EDGE;
    prefilter_spec.wrapR          = TextureWrap::CLAMP_TO_EDGE;
    prefilter_spec.minFilter      = TextureFilter::LINEAR_MIPMAP_LINEAR;
    prefilter_spec.magFilter      = TextureFilter::LINEAR;

    auto prefilter_map = std::make_shared<Texture>(resolution, resolution, prefilter_spec);

    shader->bind();
    shader->setUniformInt("u_environmentMap", 0);
    shader->setUniformMat4("u_projectionMatrix", captureProjection);

    envMap->bind(0);

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    for (uint32_t mip = 0; mip < maxMipLevels; mip++)
    {
        uint32_t mipWidth  = resolution * std::pow(0.5, mip);
        uint32_t mipHeight = resolution * std::pow(0.5, mip);

        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        shader->setUniformFloat("u_roughness", roughness);

        for (uint32_t i = 0; i < 6; i++)
        {
            shader->setUniformMat4("u_viewMatrix", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                   prefilter_map->getHandle(), mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            vertexArray->bind();
            glDrawElements(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto app        = renderer->getApplication();
    uint32_t width  = app->getWindowWidth();
    uint32_t height = app->getWindowHeight();

    glViewport(0, 0, width, height);

    return prefilter_map;
}

std::shared_ptr<Texture> TextureUtils::generateBRDFLUT(Renderer *renderer, uint32_t resolution)
{
    // 全屏四边形
    std::vector<float> vertices = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,
    };

    auto vertexArray = std::make_shared<VertexArray>();
    vertexArray->bind();
    {
        auto vertexBuffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
        vertexBuffer->setLayout({
            {ShaderDataType::Float3, "a_position"},
            {ShaderDataType::Float2, "a_texCoord"},
        });
        vertexArray->addVertexBuffer(vertexBuffer);

        auto indexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());
        vertexArray->setIndexBuffer(indexBuffer);
    }

    auto shader = ResourceManager::GetInstance().loadShader("brdf_lut",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/vert/"
                                                                             "quad.vert",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/frag/"
                                                                             "brdf_lut.frag");

    TextureSpecification brdfLUTSpec;
    brdfLUTSpec.textureType     = TextureType::TEXTURE_2D;
    brdfLUTSpec.isHDR           = false;
    brdfLUTSpec.generateMipmap  = false;
    brdfLUTSpec.internalFormat  = TextureInternalFormat::RG16F;
    brdfLUTSpec.format          = TextureFormat::RG;
    brdfLUTSpec.textureDataType = TextureDataType::FLOAT;
    brdfLUTSpec.wrapS           = TextureWrap::CLAMP_TO_EDGE;
    brdfLUTSpec.wrapT           = TextureWrap::CLAMP_TO_EDGE;
    brdfLUTSpec.minFilter       = TextureFilter::LINEAR;
    brdfLUTSpec.magFilter       = TextureFilter::LINEAR;

    auto brdfLUT = std::make_shared<Texture>(resolution, resolution, brdfLUTSpec);

    shader->bind();

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT->getHandle(), 0);

    glViewport(0, 0, resolution, resolution);

    vertexArray->bind();
    glDrawElements(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, nullptr);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto app        = renderer->getApplication();
    uint32_t width  = app->getWindowWidth();
    uint32_t height = app->getWindowHeight();

    glViewport(0, 0, width, height);

    return brdfLUT;
}

} // namespace Airwave