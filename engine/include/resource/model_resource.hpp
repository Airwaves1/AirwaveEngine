// #pragma once

// #include "resource/resource.hpp"
// #include "resource/texture_resource.hpp"

// #include "rendering/primitive.hpp"
// #include "rendering/material.hpp"

// #include <entt/entt.hpp>
// #include <tiny_gltf/tiny_gltf.h>

// namespace Airwave
// {

// class AwScene;
// class AwEntity;

// class ModelResource : public Resource
// {
//   public:
//     ModelResource() = default;
//     ~ModelResource() override;

//     entt::entity instantiate(AwScene *scene, entt::entity rootEntity = entt::null);

//   protected:
//     bool onLoad(const std::string &path, const std::any &params) override;

//   private:
//     void processNode(const tinygltf::Node &node, AwScene *scene, entt::entity parentEntity, const glm::mat4 &parentMatrix);

//     void processMesh(const tinygltf::Mesh &mesh, AwScene *scene, entt::entity entity, const glm::mat4 &transform);

//     std::shared_ptr<Primitive> createPrimitive(const tinygltf::Primitive &gltfPrimitive);

//     Material processMaterial(const tinygltf::Material &gltfMaterial);

//   private:
//     // GLTF模型数据
//     std::unique_ptr<tinygltf::Model> m_model;

//     // 资源缓存
//     std::unordered_map<int, std::shared_ptr<TextureResource>> m_textureCache;
//     std::unordered_map<int, Material> m_materialCache;
//     std::unordered_map<int, std::shared_ptr<Primitive>> m_primitiveCache;

//     // 根实体引用
//     entt::entity m_rootEntity{entt::null};
// };

// } // namespace Airwave
