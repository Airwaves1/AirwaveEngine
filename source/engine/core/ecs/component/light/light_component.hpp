#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "opengl/gl_texture.hpp"

namespace Airwave
{
enum class LightType
{
    Point,
    Directional,
    Spot
};

struct LightComponent
{
    LightType type;  // 光源类型
    float intensity; // 光源强度（在PBR中可表示辐射率）

    // 光照分量
    glm::vec3 ambient;  // 环境光颜色
    glm::vec3 diffuse;  // 漫反射颜色
    glm::vec3 specular; // 镜面反射颜色

    glm::vec3 position;  // 光源位置（对点光源和聚光灯有效）
    glm::vec3 direction; // 光源方向（对平行光和聚光灯有效）

    float constant;    // 衰减系数（点光源和聚光灯使用）
    float linear;      // 衰减系数（点光源和聚光灯使用）
    float quadratic;   // 衰减系数（点光源和聚光灯使用）
    float cutOff;      // 聚光灯内切角
    float outerCutOff; // 聚光灯外切角

    float shadowBias; // 阴影偏移
    bool castsShadow; // 是否产生阴影

    // 纹理贴图
    std::shared_ptr<Texture> shadowMap;      // 阴影贴图
    std::shared_ptr<Texture> irradianceMap;  // 漫反射环境光贴图（用于PBR）
    std::shared_ptr<Texture> environmentMap; // 环境贴图（用于PBR）

    LightComponent(LightType type = {}, float intensity = {}, bool castsShadow = false)
        : type(type), intensity(intensity), shadowBias(0.005f), castsShadow(castsShadow)
    {
        // 默认光源位置和方向
        position  = glm::vec3(0.0f, 1.0f, 0.0f);
        direction = glm::vec3(0.0f, -1.0f, 0.0f);
        ambient   = glm::vec3(0.2f);
        diffuse   = glm::vec3(0.5f);
        specular  = glm::vec3(1.0f);

        // 默认光源衰减系数
        constant  = 1.0f;
        linear    = 0.09f;
        quadratic = 0.032f;

        // 默认聚光灯内外切角
        cutOff      = glm::cos(glm::radians(12.5f));
        outerCutOff = glm::cos(glm::radians(17.5f));
    }
};

} // namespace Airwave
