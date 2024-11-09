#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>

#include "opengl/gl_shader.hpp"
#include "opengl/gl_texture.hpp"

namespace Airwave
{
enum class MaterialType
{
    BlinnPhong,
    PBR
};

enum class BlendMode
{
    NONE,
    ALPHA,
    ADDITIVE,
    MULTIPLY
};

struct MaterialComponent
{
    std::string name; // 材质名称

    MaterialType type = MaterialType::BlinnPhong; // 材质类型

    glm::vec3 diffuse  = glm::vec3(0.8f); // 漫反射
    glm::vec3 ambient  = glm::vec3(0.1f); // 环境光
    glm::vec3 specular = glm::vec3(1.0f); // 镜面反射
    float shininess    = 32.0f;           // 光泽度

    std::shared_ptr<Texture> diffuseMap  = nullptr; // 漫反射贴图
    std::shared_ptr<Texture> specularMap = nullptr; // 镜面反射贴图
    std::shared_ptr<Texture> normalMap   = nullptr; // 法线贴图

    std::shared_ptr<Shader> shader; // 着色器
    bool needUpdate = true;         // 是否需要更新材质

    bool depthTest  = true;  // 深度测试
    bool depthWrite = true;  // 深度写入
    bool cullFace   = true;  // 背面剔除
    bool wireframe  = false; // 线框模式

    BlendMode blendMode = BlendMode::NONE; // 混合模式

    MaterialComponent(MaterialType type = MaterialType::BlinnPhong)
        : type(type)
    {
        if(type == MaterialType::BlinnPhong)
        {
            shader = SHADER_LIB.load("blinn_phong", SHADER_DIR "blinn_phong.vert",
                                     SHADER_DIR "blinn_phong.frag");
        }
        else if(type == MaterialType::PBR)
        {
            shader = SHADER_LIB.load("pbr", SHADER_DIR "pbr.vert", SHADER_DIR "pbr.frag");
        }
    }
};

struct CustomMaterialComponent
{
    std::string name; // 材质名称

    std::shared_ptr<Shader> shader; // 着色器
    bool needUpdate = true;         // 是否需要更新材质

    bool depthTest  = true;  // 深度测试
    bool depthWrite = true;  // 深度写入
    bool cullFace   = true;  // 背面剔除
    bool wireframe  = false; // 线框模式

    BlendMode blendMode = BlendMode::NONE; // 混合模式
};

struct MeshPlaneMaterialComponent : public CustomMaterialComponent
{
    glm::vec3 color = glm::vec3(1.0f); // 颜色

    MeshPlaneMaterialComponent()
    {
        shader = SHADER_LIB.load("mesh_plane", SHADER_DIR "mesh_plane.vert", SHADER_DIR "mesh_plane.frag");
    }
};


} // namespace Airwave
