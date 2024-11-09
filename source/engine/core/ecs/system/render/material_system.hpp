#pragma once

#include "ecs/system/aw_system.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/component/material/material_component.hpp"
#include "ecs/component/mesh/mesh_component.hpp"
#include "ecs/component/transform/transform_component.hpp"
#include "ecs/component/camera/camera_component.hpp"
#include "ecs/component/light/light_component.hpp"
#include "resource/shader_lib.hpp"
#include "resource/texture_lib.hpp"
#include "utils/log.hpp"
#include <glad/glad.h>
namespace Airwave
{
class MaterialSystem : public AwSystem
{
  public:
    MaterialSystem() = default;

    virtual ~MaterialSystem() = default;

    virtual void onUpdate(float deltaTime, std::shared_ptr<Scene> scene) override {}

    void applyMaterial(MaterialComponent &mat, const CameraComponent &cameraComp,
                       glm::mat4 &modelMatrix, const std::vector<LightComponent> &lightComps,
                       float deltaTime)
    {
        mat.shader->bind();

        mat.shader->uploadUniformMat4("u_modelMatrix", modelMatrix);
        mat.shader->uploadUniformMat4("u_viewMatrix", cameraComp.camera->getViewMatrix());
        mat.shader->uploadUniformMat4("u_projectionMatrix",
                                      cameraComp.camera->getProjectionMatrix());

        mat.shader->uploadUniformFloat3("u_cameraPosition", cameraComp.camera->getPosition());

        mat.shader->uploadUniformFloat3("u_material.ambient", mat.ambient);
        mat.shader->uploadUniformFloat3("u_material.diffuse", mat.diffuse);
        mat.shader->uploadUniformFloat3("u_material.specular", mat.specular);
        mat.shader->uploadUniformFloat("u_material.shininess", mat.shininess);
        mat.shader->uploadUniformInt("u_material.diffuseMap", 0);
        mat.shader->uploadUniformInt("u_material.specularMap", 1);

        if (mat.diffuseMap == nullptr)
        {
            mat.shader->uploadUniformInt("u_useDiffuseMap", 0);
        }
        else
        {
            mat.diffuseMap->bind(0);
            mat.shader->uploadUniformInt("u_useDiffuseMap", 1);
        }

        if (mat.specularMap == nullptr)
        {
            mat.shader->uploadUniformInt("u_useSpecularMap", 0);
        }
        else
        {
            mat.specularMap->bind(1);
            mat.shader->uploadUniformInt("u_useSpecularMap", 1);
        }

        if (lightComps.empty()) return;
        auto shader = mat.shader;
        shader->bind();
        shader->uploadUniformInt("u_lightCount", lightComps.size());
        for (int i = 0; i < lightComps.size(); i++)
        {

            const auto &light = lightComps[i];
            shader->uploadUniformInt("u_lights[" + std::to_string(i) + "].type",
                                     static_cast<int>(light.type));
            shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].intensity",
                                       light.intensity);
            shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].ambient",
                                        light.ambient);
            shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].diffuse",
                                        light.diffuse);
            shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].specular",
                                        light.specular);
            if (light.type == LightType::Point)
            {
                shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].position",
                                            light.position);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].constant",
                                           light.constant);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].linear",
                                           light.linear);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].quadratic",
                                           light.quadratic);
            }
            else if (light.type == LightType::Directional)
            {
                shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].direction",
                                            light.direction);
            }
            else if (light.type == LightType::Spot)
            {
                shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].position",
                                            light.position);
                shader->uploadUniformFloat3("u_lights[" + std::to_string(i) + "].direction",
                                            light.direction);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].constant",
                                           light.constant);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].linear",
                                           light.linear);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].quadratic",
                                           light.quadratic);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].cutOff",
                                           light.cutOff);
                shader->uploadUniformFloat("u_lights[" + std::to_string(i) + "].outerCutOff",
                                           light.outerCutOff);
            }
        }
    }
};

} // namespace Airwave
