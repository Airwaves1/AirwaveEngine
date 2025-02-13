#pragma once

#include "resource/resource.hpp"
#include "resource/texture_resource.hpp"

#include "rendering/primitive.hpp"
#include "rendering/material.hpp"

#include <entt/entt.hpp>
#include <tiny_gltf/tiny_gltf.h>

namespace Airwave
{

class AwScene;
class AwEntity;
struct VertexAttributes
{
    glm::vec3 position;  // 12 bytes
    glm::vec3 normal;    // 12 bytes (累计24)
    glm::vec2 uv;        // 8 bytes  (累计32)
    glm::vec4 tangent;   // 16 bytes (累计48)
    glm::u16vec4 joints; // 8 bytes  (累计56)
    glm::vec4 weights;   // 16 bytes (累计72)
}; // 总大小72字节
class ModelResource : public Resource
{
  public:
    ModelResource() = default;
    ~ModelResource() override;

    entt::entity instantiate(AwScene *scene, entt::entity rootEntity = entt::null);

  protected:
    bool onLoad(const std::string &path, const std::any &params) override;

  private:
    void processNode(const tinygltf::Node &node, AwScene *scene, entt::entity parentEntity, const glm::mat4 &parentMatrix);

    void processMesh(const tinygltf::Mesh &mesh, AwScene *scene, entt::entity entity, const glm::mat4 &transform);

    std::shared_ptr<Primitive> createPrimitive(const tinygltf::Primitive &gltfPrimitive);

    std::shared_ptr<Material> processMaterial(const tinygltf::Material &gltfMaterial);

  private:
    std::string m_dir;

    // GLTF模型数据
    std::unique_ptr<tinygltf::Model> m_model;

    // 资源缓存
    std::unordered_map<int, std::shared_ptr<TextureResource>> m_textureCache;
    std::unordered_map<int, Material> m_materialCache;
    std::unordered_map<int, std::shared_ptr<Primitive>> m_primitiveCache;

    // 根实体引用
    entt::entity m_rootEntity{entt::null};
};

} // namespace Airwave
