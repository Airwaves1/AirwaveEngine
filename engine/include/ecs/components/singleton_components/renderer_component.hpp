#pragma once

#include "ecs/aw_component.hpp"

#include "resource/resource_manager.hpp"
#include "resource/resource.hpp"
#include "resource/texture_resource.hpp"
#include "resource/shader_resource.hpp"

#include "rendering/texture.hpp"
#include "rendering/shader.hpp"
#include "rendering/primitive.hpp"

#include "utils/geometry_utils.hpp"

#include <glm/glm.hpp>

namespace Airwave
{
class RendererComponent : public AwComponent
{
  public:
    uint64_t drawCalls = 0;

    std::shared_ptr<Shader> backgroundShader;
    std::shared_ptr<Shader> basicShader;
    std::shared_ptr<Shader> geometryPassShader;
    std::shared_ptr<Shader> lightingPassShader;
    std::shared_ptr<Shader> shadowShader;

    std::shared_ptr<Texture> emptyMap;
    std::shared_ptr<Texture> defaultNormal;

    std::shared_ptr<Texture> irradianceMap;
    std::shared_ptr<Texture> prefilterMap;
    std::shared_ptr<Texture> brdfLUT;

    std::shared_ptr<Texture> envMap;
    std::shared_ptr<Texture> backgroundMap;

    RendererComponent()
    {
        backgroundShader = RES.get<ShaderResource>("shaders/shader_lib/background.glsl")->getShader();
        basicShader      = RES.get<ShaderResource>("shaders/shader_lib/basic.glsl")->getShader();
        geometryPassShader = RES.get<ShaderResource>("shaders/shader_lib/geometry_pass.glsl")->getShader();
        lightingPassShader = RES.get<ShaderResource>("shaders/shader_lib/lighting_pass.glsl")->getShader();
        shadowShader = RES.get<ShaderResource>("shaders/shader_lib/shadow.glsl")->getShader();


        emptyMap      = RES.get<TextureResource>("empty_map")->getTexture();
        defaultNormal = RES.get<TextureResource>("default_normal")->getTexture();

        cube = GeometryUtils::CreateCube(1.0f, 1.0f, 1.0f, 1, 1, 1);
        quad = GeometryUtils::CreateQuad(1.0f, 1.0f, 1, 1);
    }

  private:
    std::shared_ptr<Primitive> cube;
    std::shared_ptr<Primitive> quad;

    friend class RenderSystem;
};

} // namespace Airwave
