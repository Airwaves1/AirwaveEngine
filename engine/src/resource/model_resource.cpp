// #include "resource/model_resource.hpp"

// #include <tiny_gltf/tiny_gltf.h>

// #include "ecs/aw_scene.hpp"
// #include "ecs/aw_entity.hpp"

// #include "ecs/components/mesh_component.hpp"
// #include "ecs/components/material_component.hpp"
// #include "ecs/components/transform_component.hpp"
// #include "ecs/components/hierarchy_component.hpp"

// namespace Airwave
// {

// Model::~Model() {}

// bool Model::load(const std::string &path)
// {
//     tinygltf::Model model;
//     tinygltf::TinyGLTF loader;
//     std::string err, warn;

//     // 加载gltf文件
//     bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
//     if (!warn.empty())
//     {
//         LOG_WARN("gltf warn: {0}", warn);
//     }
//     if (!err.empty() || !ret)
//     {
//         LOG_ERROR("gltf error: {0}", err);
//         return false;
//     }

//     m_loaded = true;

//     return m_loaded;
// }

// entt::entity Model::loadModelToScene(AwScene *scene, entt::entity rootEntity)
// {
//     return entt::null;
// }

// // CachedNode Model::createNodeHierarchy(AwScene *scene, entt::entity parentEntity, int gltfNodeIndex)
// // {
// //     return CachedNode();
// // }

// } // namespace Airwave
