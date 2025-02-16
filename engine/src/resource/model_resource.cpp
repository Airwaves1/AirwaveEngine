#include "resource/model_resource.hpp"

#include <glad/glad.h>
#include <tiny_gltf/tiny_gltf.h>
#include <filesystem>

#include "core/log.hpp"
#include "utils/file_utils.hpp"
#include "utils/geometry_utils.hpp"
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

    m_dir = std::filesystem::path(path).parent_path().string() + "/";
    m_dir = FileUtils::GetAssetPath(m_dir);

    return ret;
}

entt::entity ModelResource::instantiate(AwScene *scene, entt::entity rootEntity)
{
    if (!m_loaded || !scene)
    {
        LOG_ERROR("ModelResource not loaded or scene is null");
        return entt::null;
    }

    if (rootEntity == entt::null)
    {
        rootEntity = scene->createDefaultEntity("model_root");
    }

    const auto &gltf_scene = m_model->scenes[m_model->defaultScene];

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
    auto entity = scene->createDefaultEntity(node.name);

    scene->setEntityParent(entity, parentEntity);

    scene->addComponent<MeshComponent>(entity);
    scene->addComponent<MaterialComponent>(entity, MaterialType::PBR);

    auto &transform_comp = scene->getComponent<TransformComponent>(entity);

    // 处理节点变换
    if (!node.matrix.empty())
    {
        transform_comp.localMatrix = glm::make_mat4(node.matrix.data());
        transform_comp.worldMatrix = parentMatrix * transform_comp.localMatrix;
    }
    else
    {
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
    }

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

void ModelResource::processMesh(const tinygltf::Mesh &mesh, AwScene *scene, entt::entity entity, const glm::mat4 &transform)
{
    if (mesh.primitives.empty())
    {
        LOG_ERROR("Mesh has no primitives");
        return;
    }

    auto &mesh_comp     = scene->getComponent<MeshComponent>(entity);
    auto &material_comp = scene->getComponent<MaterialComponent>(entity);

    bool hasTangent = false;

    for (const auto &primitive : mesh.primitives)
    {
        auto primitivePtr = createPrimitive(primitive, material_comp.material->shader->getHandle(), &hasTangent);
        if (!primitivePtr)
        {
            LOG_ERROR("Failed to create primitive");
        }
        
        mesh_comp.primitives.push_back(primitivePtr);
    }

    // 处理材质, 暂时只处理第一个primitive的材质
    const auto &primitive = mesh.primitives[0];
    auto material = processMaterial(m_model->materials[primitive.material]);
    material_comp.material = material;
    material_comp.material->hasTanget = hasTangent;

}

std::shared_ptr<Primitive> ModelResource::createPrimitive(const tinygltf::Primitive &gltfPrimitive, uint32_t program, bool *hasTangent)
{
    auto primitive = std::make_shared<Primitive>();

    // 检查必要属性
    if (gltfPrimitive.attributes.find("POSITION") == gltfPrimitive.attributes.end())
    {
        LOG_ERROR("Primitive missing POSITION attribute");
        return nullptr;
    }

    // 创建VAO
    glGenVertexArrays(1, &primitive->vao);
    glBindVertexArray(primitive->vao);

    // 生成并填充VBO
    glGenBuffers(1, &primitive->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, primitive->vbo);

    // 收集顶点数据
    const auto &posAccessor  = m_model->accessors[gltfPrimitive.attributes.at("POSITION")];
    const size_t vertexCount = posAccessor.count;

    std::vector<VertexAttributes> vertices(vertexCount);
    for (const auto &[name, accessorIdx] : gltfPrimitive.attributes)
    {
        const auto &accessor   = m_model->accessors[accessorIdx];
        const auto &bufferView = m_model->bufferViews[accessor.bufferView];
        const auto &buffer     = m_model->buffers[bufferView.buffer];
        const uint8_t *dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

        for (size_t i = 0; i < vertexCount; ++i)
        {
            const size_t stride = bufferView.byteStride ? bufferView.byteStride : accessor.ByteStride(bufferView);
            const uint8_t *src  = dataPtr + i * stride;

            if (name == "POSITION")
            {
                memcpy(&vertices[i].position, src, sizeof(glm::vec3));
            }
            else if (name == "NORMAL")
            {
                memcpy(&vertices[i].normal, src, sizeof(glm::vec3));
            }
            else if (name == "TEXCOORD_0")
            {
                memcpy(&vertices[i].uv, src, sizeof(glm::vec2));
            }
            else if (name == "TANGENT")
            {
                memcpy(&vertices[i].tangent, src, sizeof(glm::vec4));
            }
            else if (name == "JOINTS_0")
            {
                // 处理不同整数类型
                if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
                {
                    const auto *joints = reinterpret_cast<const uint8_t *>(src);
                    vertices[i].joints = {joints[0], joints[1], joints[2], joints[3]};
                }
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                {
                    const auto *joints = reinterpret_cast<const uint16_t *>(src);
                    vertices[i].joints = {joints[0], joints[1], joints[2], joints[3]};
                }
            }
            else if (name == "WEIGHTS_0")
            {
                memcpy(&vertices[i].weights, src, sizeof(glm::vec4));
            }
        }
    }

    // 上传VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexAttributes), vertices.data(), GL_STATIC_DRAW);

    // 设置顶点属性
    const GLsizei stride = sizeof(VertexAttributes);
    size_t offset        = 0;

    // Position
    if (gltfPrimitive.attributes.find("POSITION") != gltfPrimitive.attributes.end())
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
        offset += sizeof(glm::vec3);
    }

    // Normal
    if (gltfPrimitive.attributes.find("NORMAL") != gltfPrimitive.attributes.end())
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)offset);
        offset += sizeof(glm::vec3);
    }

    // texCoord
    if (gltfPrimitive.attributes.find("TEXCOORD_0") != gltfPrimitive.attributes.end())
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)offset);
        offset += sizeof(glm::vec2);
    }

    // Tangent
    if (gltfPrimitive.attributes.find("TANGENT") != gltfPrimitive.attributes.end())
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void *)offset);
        offset += sizeof(glm::vec4);

        if (hasTangent)
        {
            *hasTangent = true;
        }
    }

    // Joints
    if (gltfPrimitive.attributes.find("JOINTS_0") != gltfPrimitive.attributes.end())
    {
        glEnableVertexAttribArray(4);
        if (sizeof(glm::u16vec4::value_type) == 2)
        {
            glVertexAttribIPointer(4, 4, GL_UNSIGNED_SHORT, stride, (void *)offset);
        }
        else
        {
            LOG_ERROR("Unsupported joints data type");
        }
        offset += sizeof(glm::u16vec4);
    }

    if (gltfPrimitive.attributes.find("WEIGHTS_0") != gltfPrimitive.attributes.end())
    {
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (void *)offset);
        offset += sizeof(glm::vec4);
    }

    // 处理索引缓冲
    if (gltfPrimitive.indices >= 0)
    {
        const auto &indexAccessor = m_model->accessors[gltfPrimitive.indices];
        const auto &bufferView    = m_model->bufferViews[indexAccessor.bufferView];
        const auto &buffer        = m_model->buffers[bufferView.buffer];

        glGenBuffers(1, &primitive->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitive->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferView.byteLength, buffer.data.data() + bufferView.byteOffset + indexAccessor.byteOffset,
                     GL_STATIC_DRAW);

        primitive->indexCount = indexAccessor.count;
        primitive->indexType  = indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
    }

    glBindVertexArray(0);
    return primitive;
}

std::shared_ptr<Material> ModelResource::processMaterial(const tinygltf::Material &gltfMaterial)
{
    std::shared_ptr<Material> material = std::make_shared<Material>(MaterialType::PBR);

    // base color
    if (gltfMaterial.pbrMetallicRoughness.baseColorFactor.size() == 4)
    {
        material->color = glm::make_vec3(gltfMaterial.pbrMetallicRoughness.baseColorFactor.data());
    }

    // base color texture
    if (gltfMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.pbrMetallicRoughness.baseColorTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        spec.sRGB = true;
        // spec.flip            = true;
        auto textureResource = RES.load<TextureResource>(m_dir + image.uri, spec);
        material->albedoMap  = textureResource->getTexture();
    }

    // normal texture
    if (gltfMaterial.normalTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.normalTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        auto textureResource = RES.load<TextureResource>(m_dir + image.uri, spec);
        material->normalMap  = textureResource->getTexture();
    }

    // metallic roughness texture
    if (gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        spec.sRGB                      = false;
        spec.flip                      = false;
        spec.format                    = TextureFormat::RGB;
        spec.internalFormat            = TextureInternalFormat::RGB8;
        auto textureResource           = RES.load<TextureResource>(m_dir + image.uri, spec);
        material->metallicRoughnessMap = textureResource->getTexture();
    }

    // metallic factor
    material->metallic = gltfMaterial.pbrMetallicRoughness.metallicFactor;

    // roughness factor
    material->roughness = gltfMaterial.pbrMetallicRoughness.roughnessFactor;

    // occlusion texture
    if (gltfMaterial.occlusionTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.occlusionTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        auto textureResource = RES.load<TextureResource>(m_dir + image.uri, spec);
        material->aoMap      = textureResource->getTexture();
    }

    // emissive texture
    if (gltfMaterial.emissiveTexture.index >= 0)
    {
        const auto &texture = m_model->textures[gltfMaterial.emissiveTexture.index];
        const auto &image   = m_model->images[texture.source];
        const auto &sampler = m_model->samplers[texture.sampler];

        TextureSpecification spec;
        auto textureResource  = RES.load<TextureResource>(m_dir + image.uri, spec);
        material->emissiveMap = textureResource->getTexture();
    }

    // emissive factor
    if (gltfMaterial.emissiveFactor.size() == 3)
    {
        material->emissive = glm::make_vec3(gltfMaterial.emissiveFactor.data());
    }

    return material;
}

} // namespace Airwave
