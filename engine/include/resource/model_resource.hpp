#pragma once

#include "resource/resource.hpp"
#include "resource/texture_resource.hpp"

#include "rendering/primitive.hpp"
#include <entt/entt.hpp>
#include <tiny_gltf/tiny_gltf.h>

namespace Airwave
{

class AwScene;
class AwEntity;

class Model : public Resource
{
  public:
    struct MaterialParams
    {
        glm::vec4 baseColorFactor{1.0f};
        float metallicFactor  = 1.0f;
        float roughnessFactor = 1.0f;
        glm::vec3 emissiveFactor{0.0f};

        std::shared_ptr<TextureResource> baseColorTexture;
        std::shared_ptr<TextureResource> normalTexture;
        std::shared_ptr<TextureResource> metallicRoughnessTexture;
    };

    Model() = default;
    ~Model() override;

    bool load(const std::string &path);
    void onDispose() override {}

    entt::entity instantiate(AwScene *scene, entt::entity rootEntity = entt::null);

  private:
    void processNode(const tinygltf::Node &node, AwScene *scene, entt::entity parentEntity, const glm::mat4 &parentMatrix);

    void processMesh(const tinygltf::Mesh &mesh, AwScene *scene, entt::entity entity, const glm::mat4 &transform);

    std::shared_ptr<Primitive> createPrimitive(const tinygltf::Primitive &gltfPrimitive);

    MaterialParams processMaterial(const tinygltf::Material &gltfMaterial);

  private:
    // GLTF模型数据
    std::unique_ptr<tinygltf::Model> m_model;

    // 资源缓存
    std::unordered_map<int, std::shared_ptr<TextureResource>> m_textureCache;
    std::unordered_map<int, MaterialParams> m_materialCache;
    std::unordered_map<int, std::shared_ptr<Primitive>> m_primitiveCache;

    // 根实体引用
    entt::entity m_rootEntity{entt::null};
};

} // namespace Airwave
