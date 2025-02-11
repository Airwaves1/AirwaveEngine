#include "resource/model_resource.hpp"

#include <glad/glad.h>
#include <tiny_gltf/tiny_gltf.h>

#include "core/log.hpp"

#include "ecs/aw_scene.hpp"
#include "ecs/aw_entity.hpp"

#include "ecs/components/mesh_component.hpp"
#include "ecs/components/material_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/hierarchy_component.hpp"

namespace Airwave
{

Model::~Model()
{
    if (!m_loaded) return;

    // 释放GPU资源

    m_model.reset();
    m_primitiveCache.clear();
    m_textureCache.clear();
    m_materialCache.clear();
    m_loaded = false;
}

bool Model::load(const std::string &path)
{
    if (m_loaded) return true;

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);

    if (!warn.empty())
    {
        LOG_WARN("Warn: {0}", warn);
    }

    if (!err.empty() || !ret)
    {
        LOG_ERROR("Error: {0}", err);
        return false;
    }

    m_model = std::make_unique<tinygltf::Model>(std::move(model));

    m_loaded = true;

    return ret;
}

entt::entity Model::instantiate(AwScene *scene, entt::entity rootEntity)
{
    if (!m_loaded || !scene)
    {
        LOG_ERROR("Model not loaded or scene is null");
        return entt::null;
    }

    const auto &gltf_scene = m_model->scenes[m_model->defaultScene];
    if(rootEntity == entt::null)
    {
        rootEntity = scene->createEntity("ModelRoot");
    }

    const glm::mat4 rootTransform{1.0f};
    for (const auto &nodeIdx : gltf_scene.nodes)
    {
        processNode(m_model->nodes[nodeIdx], scene, rootEntity, rootTransform);
    }

    return rootEntity;
}

void Model::processNode(const tinygltf::Node &node, AwScene *scene, entt::entity parentEntity, const glm::mat4 &parentMatrix) {}

void Model::processMesh(const tinygltf::Mesh &mesh, AwScene *scene, entt::entity entity, const glm::mat4 &transform) {}

std::shared_ptr<Primitive> Airwave::Model::createPrimitive(const tinygltf::Primitive &gltfPrimitive) { return std::shared_ptr<Primitive>(); }

// MaterialParams Airwave::Model::processMaterial(const tinygltf::Material &gltfMaterial) { return MaterialParams(); }

} // namespace Airwave
