#include "utils/texture_utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/application.hpp"

#include "resource/resource_manager.hpp"
#include "resource/shader_resource.hpp"


#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/framebuffer.hpp"
#include "rendering/primitive.hpp"

#include "core/common.hpp"
#include "utils/geometry_utils.hpp"

namespace Airwave
{
std::shared_ptr<Texture> TextureUtils::equirectangularToCubemap(Renderer *renderer, const std::shared_ptr<Texture> &equirectangular,
                                                                uint32_t resolution, bool isHDR)
{
    if (equirectangular == nullptr || equirectangular->getHandle() == 0)
    {
        LOG_ERROR("TextureUtils::equirectangular: equirectangular is nullptr");
        return nullptr;
    }

    auto &equirectSpec = equirectangular->getSpec();
    if (equirectSpec.textureType != TextureType::TEXTURE_2D)
    {
        LOG_ERROR("TextureUtils::equirectangularToCubemap: equirectangular texture is not 2D");
        return nullptr;
    }

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[]    = {glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    // auto vertexArray = ShapesVAO::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);
    auto primitive = GeometryUtils::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);

    auto shader_res = RES.load<ShaderResource>("shaders/shader_lib/equirectangular_to_cubemap.glsl");
    if (!shader_res)
    {
        LOG_ERROR("TextureUtils::prefilterEnvMap: shader resource is nullptr");
        return nullptr;
    }
    uint32_t shader = shader_res->getShader()->getHandle();

    TextureSpecification cube_map_spec;
    cube_map_spec.width       = resolution;
    cube_map_spec.height      = resolution;
    cube_map_spec.isHDR       = isHDR;
    cube_map_spec.textureType = TextureType::TEXTURE_CUBE_MAP;
    cube_map_spec.wrapS       = TextureWrap::CLAMP_TO_EDGE;
    cube_map_spec.wrapT       = TextureWrap::CLAMP_TO_EDGE;
    cube_map_spec.wrapR       = TextureWrap::CLAMP_TO_EDGE;
    cube_map_spec.minFilter   = TextureFilter::LINEAR_MIPMAP_LINEAR; // enable pre-filter mipmap sampling (combatting
                                                                     // visible dots artifact)
    cube_map_spec.magFilter = TextureFilter::LINEAR;

    auto cube_map = std::make_shared<Texture>(cube_map_spec);

    renderer->bindShader(shader);
    renderer->set("u_equirectangularMap", 0);
    renderer->set("u_projectionMatrix", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, equirectangular->getHandle());

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, resolution, resolution);

    for (uint32_t i = 0; i < 6; i++)
    {
        renderer->set("u_viewMatrix", captureViews[i]);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cube_map->getHandle(), 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer->uploadUniforms(shader);
        primitive->draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto app        = renderer->getApplication();
    uint32_t width  = app->getWindowWidth();
    uint32_t height = app->getWindowHeight();

    glViewport(0, 0, width, height);

    // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map->getHandle());
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    return cube_map;
}

std::shared_ptr<Texture> TextureUtils::irradianceConvolution(Renderer *renderer, const std::shared_ptr<Texture> &envMap,
                                                                     uint32_t resolution)
{
    auto &envMapSpec = envMap->getSpec();
    if (envMapSpec.textureType != TextureType::TEXTURE_CUBE_MAP)
    {
        LOG_ERROR("TextureUtils::irradianceConvolution: envMap texture is not cube map");
        return nullptr;
    }

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[]    = {glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    auto primitive = GeometryUtils::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);

    auto shader_res = RES.load<ShaderResource>("shaders/shader_lib/irradiance_convolution.glsl");
    if (!shader_res)
    {
        LOG_ERROR("TextureUtils::prefilterEnvMap: shader resource is nullptr");
        return nullptr;
    }
    uint32_t shader = shader_res->getShader()->getHandle();

    TextureSpecification irradiance_spec;
    irradiance_spec.width       = resolution;
    irradiance_spec.height      = resolution;
    irradiance_spec.isHDR       = envMapSpec.isHDR;
    irradiance_spec.textureType = TextureType::TEXTURE_CUBE_MAP;
    irradiance_spec.wrapS       = TextureWrap::CLAMP_TO_EDGE;
    irradiance_spec.wrapT       = TextureWrap::CLAMP_TO_EDGE;
    irradiance_spec.wrapR       = TextureWrap::CLAMP_TO_EDGE;

    auto irradiance_map = std::make_shared<Texture>(irradiance_spec);

    renderer->bindShader(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envMap->getHandle());
    renderer->set("u_environmentMap", 0);
    renderer->set("u_projectionMatrix", captureProjection);

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glViewport(0, 0, resolution, resolution);

    for (uint32_t i = 0; i < 6; i++)
    {
        renderer->set("u_viewMatrix", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_map->getHandle(), 0);

        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer->uploadUniforms(shader);
        primitive->draw();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto app        = renderer->getApplication();
    uint32_t width  = app->getWindowWidth();
    uint32_t height = app->getWindowHeight();

    glViewport(0, 0, width, height);

    return irradiance_map;
}

std::shared_ptr<Texture> TextureUtils::prefilterEnvMap(Renderer *renderer, const std::shared_ptr<Texture> &envMap,
                                                               uint32_t resolution, uint32_t maxMipLevels = 5)
{
    if (envMap == nullptr || envMap->getHandle() == 0)
    {
        LOG_ERROR("TextureUtils::prefilterEnvMap: envMap is nullptr");
        return nullptr;
    }
    auto &envMapSpec = envMap->getSpec();
    if (envMapSpec.textureType != TextureType::TEXTURE_CUBE_MAP)
    {
        LOG_ERROR("TextureUtils::prefilterEnvMap: envMap texture is not cube map");
        return nullptr;
    }

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[]    = {glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                                   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

    auto primitive = GeometryUtils::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);

    TextureSpecification prefilter_spec;
    prefilter_spec.isHDR          = envMapSpec.isHDR;
    prefilter_spec.width          = resolution;
    prefilter_spec.height         = resolution;
    prefilter_spec.textureType    = TextureType::TEXTURE_CUBE_MAP;
    prefilter_spec.generateMipmap = true;
    prefilter_spec.wrapS          = TextureWrap::CLAMP_TO_EDGE;
    prefilter_spec.wrapT          = TextureWrap::CLAMP_TO_EDGE;
    prefilter_spec.wrapR          = TextureWrap::CLAMP_TO_EDGE;
    prefilter_spec.minFilter      = TextureFilter::LINEAR_MIPMAP_LINEAR;
    prefilter_spec.magFilter      = TextureFilter::LINEAR;

    auto prefilter_map = std::make_shared<Texture>(prefilter_spec);

    auto shader_res = RES.load<ShaderResource>("shaders/shader_lib/prefilter_envmap.glsl");
    if (!shader_res)
    {
        LOG_ERROR("TextureUtils::prefilterEnvMap: shader resource is nullptr");
        return nullptr;
    }
    uint32_t shader = shader_res->getShader()->getHandle();

    renderer->bindShader(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envMap->getHandle());

    renderer->set("u_environmentMap", 0);
    renderer->set("u_projectionMatrix", captureProjection);
    renderer->set("u_envMap_resolution", static_cast<float>(envMapSpec.width));

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    for (uint32_t mip = 0; mip < maxMipLevels; mip++)
    {
        uint32_t mipWidth  = resolution * std::pow(0.5, mip);
        uint32_t mipHeight = resolution * std::pow(0.5, mip);

        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        renderer->set("u_roughness", roughness);

        for (uint32_t i = 0; i < 6; i++)
        {
            renderer->set("u_viewMatrix", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_map->getHandle(), mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderer->uploadUniforms(shader);
            primitive->draw();
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

    auto primitive = GeometryUtils::CreateQuad(1.0f, 1.0f, 1, 1);
    // auto primitive = GeometryUtils::CreatePlane(2.0f, 2.0f, 1, 1);

    auto shader_res = RES.load<ShaderResource>("shaders/shader_lib/brdf_lut.glsl");
    if (!shader_res)
    {
        LOG_ERROR("TextureUtils::generateBRDFLUT: shader resource is nullptr");
        return nullptr;
    }
    uint32_t shader = shader_res->getShader()->getHandle();

    TextureSpecification brdfLUTSpec;
    brdfLUTSpec.width           = resolution;
    brdfLUTSpec.height          = resolution;
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

    auto brdfLUT = std::make_shared<Texture>(brdfLUTSpec);

    renderer->bindShader(shader);

    uint32_t fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT->getHandle(), 0);

    glViewport(0, 0, resolution, resolution);

    renderer->uploadUniforms(shader);
    primitive->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto app        = renderer->getApplication();
    uint32_t width  = app->getWindowWidth();
    uint32_t height = app->getWindowHeight();

    glViewport(0, 0, width, height);

    return brdfLUT;
}

} // namespace Airwave