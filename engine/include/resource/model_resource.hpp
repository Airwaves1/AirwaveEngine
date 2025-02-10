// #pragma once

// #include "resource/resource.hpp"
// #include <entt/entt.hpp>
// #include <memory>
// #include <tiny_gltf/tiny_gltf.h>

// namespace Airwave
// {
// class AwScene;
// class AwEntity;

// class Model : public Resource
// {
//   public:
//     entt::entity resourceRootEntity;

//     Model() = default;
//     ~Model();

//     bool load(const std::string &path);

//     entt::entity loadModelToScene(AwScene *scene, entt::entity rootEntity = entt::null);

//   private:
//     struct CachedNode
//     {
//         entt::entity entity;
//         int gltfNodeIndex;
//         std::vector<CachedNode> children;
//     };

//     CachedNode m_rootNode;

//     // CachedNode createNodeHierarchy(AwScene *scene, entt::entity parentEntity, int gltfNodeIndex);

//     std::shared_ptr<tinygltf::Model> m_model;
// };

// } // namespace Airwave
