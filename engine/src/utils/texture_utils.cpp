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
std::shared_ptr<Texture>
TextureUtils::equirectangularToCubemap(Renderer *renderer, std::shared_ptr<Texture> equirectangular,
                                       uint32_t resolution, bool isHDR)
{
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[]    = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                       glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
                       glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                       glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                       glm::vec3(0.0f, -1.0f, 0.0f))};

    // std::vector<AwVertex> cubeVertices;
    // std::vector<uint32_t> cubeIndices;
    // GeometryUtils::CreateCube(cubeVertices, cubeIndices, 1.0f, 1.0f, 1.0f, 1, 1, 1);
    auto vertexArray = ShapesVAO::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);

    auto shader = ResourceManager::GetInstance().loadShader("equirect",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/"
                                                                             "equirect.vert",
                                                            PROJECT_ROOT_DIR "/assets/shaders/"
                                                                             "shader_lib/"
                                                                             "equirect.frag");

    TextureSpecification envMap_spec;
    envMap_spec.isHDR = isHDR;
    auto envMap       = std::make_shared<Texture>(resolution, resolution, envMap_spec);

    shader->bind();
    shader->setUniformInt("u_equirectangularMap", 0);
    shader->setUniformMat4("u_projectionMatrix", captureProjection);

    equirectangular->bind(0);

    FramebufferSpecification fob_spec;
    fob_spec.colorInternalFormat = TextureInternalFormat::RGB16F;
    fob_spec.colorFormat         = TextureFormat::RGB;
    auto fbo                     = std::make_shared<Framebuffer>(resolution, resolution, fob_spec);

    glViewport(0, 0, resolution, resolution);

    uint32_t handle = fbo->getHandle();
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
    for (uint32_t i = 0; i < 6; i++)
    {
        shader->setUniformMat4("u_viewMatrix", captureViews[i]);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envMap->getHandle(), 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vertexArray->bind();
        glDrawElements(GL_TRIANGLES, vertexArray->getIndexCount(), GL_UNSIGNED_INT, nullptr);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    auto app = renderer->getApplication();
    uint32_t width = app->getWindowWidth();
    uint32_t height = app->getWindowHeight();

    glViewport(0, 0, width, height);

    return envMap;
}

} // namespace Airwave
