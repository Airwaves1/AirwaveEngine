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

#include "resource/resource_manager.hpp"
#include "resource/texture_resource.hpp"

namespace Airwave
{

ModelResource::~ModelResource()
{
    if (!m_loaded) return;

    // 释放GPU资源

    m_model.reset();
    m_primitiveCache.clear();
    m_textureCache.clear();
    m_materialCache.clear();
    m_loaded = false;
}

bool Airwave::ModelResource::onLoad(const std::string &path, const std::any &params)
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

entt::entity ModelResource::instantiate(AwScene *scene, entt::entity rootEntity)
{
    if (!m_loaded || !scene)
    {
        LOG_ERROR("ModelResource not loaded or scene is null");
        return entt::null;
    }

    const auto &gltf_scene = m_model->scenes[m_model->defaultScene];
    if (rootEntity == entt::null)
    {
        rootEntity = scene->createDefaultEntity("model_root");
    }

    auto &transform_comp    = scene->getComponent<TransformComponent>(rootEntity);
    glm::mat4 rootTransform = transform_comp.getWorldMatrix();

    for (const auto &nodeIdx : gltf_scene.nodes)
    {
        processNode(m_model->nodes[nodeIdx], scene, rootEntity, rootTransform);
    }

    return rootEntity;
}

void ModelResource::processNode(const tinygltf::Node &node, AwScene *scene, entt::entity parentEntity, const glm::mat4 &parentMatrix)
{
    auto entity          = scene->createDefaultEntity(node.name);
    auto &transform_comp = scene->getComponent<TransformComponent>(entity);

    // 处理节点变换
    if (!node.translation.empty())
    {
        transform_comp.setPosition(glm::make_vec3(node.translation.data()));
    }
    if (!node.rotation.empty())
    {
        transform_comp.setRotation(glm::make_quat(node.rotation.data()));
    }
    if (!node.scale.empty())
    {
        transform_comp.setScale(glm::make_vec3(node.scale.data()));
    }

    transform_comp.localMatrix = transform_comp.calculateTransformMatrix();
    transform_comp.worldMatrix = parentMatrix * transform_comp.localMatrix;

    // 处理子节点
    for (int meshIdx : node.children)
    {
        processNode(m_model->nodes[meshIdx], scene, entity, transform_comp.worldMatrix);
    }

    // 处理网格
    if (node.mesh >= 0)
    {
        const auto &gltfMesh = m_model->meshes[node.mesh];
        processMesh(gltfMesh, scene, entity, transform_comp.worldMatrix);
    }
}

void ModelResource::processMesh(const tinygltf::Mesh &mesh, AwScene *scene, entt::entity entity, const glm::mat4 &transform) {}

std::shared_ptr<Primitive> ModelResource::createPrimitive(const tinygltf::Primitive &gltfPrimitive)
{
    auto primitive = std::make_shared<Primitive>();

    // vao
    glGenVertexArrays(1, &primitive->vao);
    glBindVertexArray(primitive->vao);

    // vbos
    const auto &attributes = gltfPrimitive.attributes;
    for (const auto &[name, accessorIdx] : attributes)
    {
        const auto &accessor   = m_model->accessors[accessorIdx];
        const auto &buffer     = m_model->buffers[accessor.bufferView];
        const auto &bufferView = m_model->bufferViews[accessor.bufferView];

        glGenBuffers(1, &primitive->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, primitive->vbo);
        glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data[bufferView.byteOffset], GL_STATIC_DRAW);

        // vertex attributes
        if (name == "POSITION")
        {
            glVertexAttribPointer(0, accessor.type, accessor.componentType, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);
        }
        else if (name == "NORMAL")
        {
            glVertexAttribPointer(1, accessor.type, accessor.componentType, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);
        }
        else if (name == "TEXCOORD_0")
        {
            glVertexAttribPointer(2, accessor.type, accessor.componentType, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(2);
        }
        else if (name == "TANGENT")
        {
            glVertexAttribPointer(3, accessor.type, accessor.componentType, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(3);
        }
        else if (name == "COLOR_0")
        {
            glVertexAttribPointer(4, accessor.type, accessor.componentType, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(4);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // index buffer
        if (gltfPrimitive.indices >= 0)
        {
            const auto &indexAccessor   = m_model->accessors[gltfPrimitive.indices];
            const auto &indexBuffer     = m_model->buffers[indexAccessor.bufferView];
            const auto &indexBufferView = m_model->bufferViews[indexAccessor.bufferView];

            glGenBuffers(1, &primitive->ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitive->ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferView.byteLength, &indexBuffer.data[indexBufferView.byteOffset], GL_STATIC_DRAW);

            primitive->indexCount = indexAccessor.count;
        }

        glBindVertexArray(0);

        return primitive;
    }

    return nullptr;
}

Material ModelResource::processMaterial(const tinygltf::Material &gltfMaterial)
{
    Material material;

    // base color
    if (gltfMaterial.pbrMetallicRoughness.baseColorFactor.size() == 4)
    {
        material.color = glm::make_vec3(gltfMaterial.pbrMetallicRoughness.baseColorFactor.data());
    }

    // base color texture
    if (gltfMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.pbrMetallicRoughness.baseColorTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        spec.sRGB            = true;
        auto textureResource = RES.load<TextureResource>(image.uri, spec);
        material.albedoMap   = textureResource;
    }

    // normal texture
    if (gltfMaterial.normalTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.normalTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        auto textureResource = RES.load<TextureResource>(image.uri, spec);
        material.normalMap   = textureResource;
    }

    // metallic roughness texture
    if (gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        auto textureResource          = RES.load<TextureResource>(image.uri, spec);
        material.metallicRoughnessMap = textureResource;
    }

    // ao texture
    if (gltfMaterial.occlusionTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.occlusionTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        auto textureResource = RES.load<TextureResource>(image.uri, spec);
        material.aoMap       = textureResource;
    }

    // emissive texture
    if (gltfMaterial.emissiveTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.emissiveTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        auto textureResource = RES.load<TextureResource>(image.uri, spec);
        material.emissiveMap = textureResource;
    }

    return material;
}

} // namespace Airwave
