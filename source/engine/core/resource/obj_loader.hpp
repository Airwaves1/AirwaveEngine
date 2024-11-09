#pragma once

#include <tiny_obj/tiny_obj_loader.h>
#include <filesystem>
#include "ecs/aw_entity.hpp"
#include "ecs/component/transform/transform_component.hpp"
#include "ecs/component/mesh/mesh_component.hpp"
#include "ecs/component/material/material_component.hpp"
#include "utils/geometry_utils.hpp"
#include "resource/texture_lib.hpp"

namespace Airwave
{
class ObjLoader
{
  public:
    static int Load(const std::shared_ptr<AwEntity> &entity, const std::string &path)
    {
        auto modelEntity = entity->getScene()->createEntity(entity->getName() + "_model");
        modelEntity->setParent(entity);
        auto &transformComp = modelEntity->addComponent<TransformComponent>();
        transformComp.setParent(entity);

        std::string extension = std::filesystem::path(path).extension().string();
        std::filesystem::path filePath(path);
        std::string directory = filePath.parent_path().string() + "/";

        if (extension != ".obj")
        {
            LOG_ERROR("Unsupported file format: {0} , this loader only supports .obj files",
                      extension);
            return -1;
        }

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(),
                                    directory.c_str());
        if (!ret)
        {
            LOG_ERROR("Failed to load model: {0}", err);
            return -1;
        }

        std::vector<AWVertex> sharedVertices;
        std::vector<uint32_t> sharedIndices;

        std::vector<MaterialComponent> materialComps;
        ProcessMaterials(directory, materials, materialComps);

        // 将所有的顶点数据都存储到共享顶点中
        for (size_t v = 0; v < attrib.vertices.size() / 3; v++)
        {
            AWVertex vertex;
            vertex.position = glm::vec3(attrib.vertices[3 * v], attrib.vertices[3 * v + 1],
                                        attrib.vertices[3 * v + 2]);
            if (v < attrib.normals.size() / 3)
            {
                vertex.normal = glm::vec3(attrib.normals[3 * v], attrib.normals[3 * v + 1],
                                          attrib.normals[3 * v + 2]);
            }

            if (v < attrib.texcoords.size() / 2)
            {
                vertex.texCoord = glm::vec2(attrib.texcoords[2 * v], attrib.texcoords[2 * v + 1]);
            }

            sharedVertices.push_back(vertex);
        }

        // 处理每个shape
        for (const auto &shape : shapes)
        {
            std::vector<uint32_t> indices;

            for (const auto &index : shape.mesh.indices)
            {
                AWVertex vertex{};

                // 顶点位置
                vertex.position = {attrib.vertices[3 * index.vertex_index],
                                   attrib.vertices[3 * index.vertex_index + 1],
                                   attrib.vertices[3 * index.vertex_index + 2]};

                // 法线
                if (index.normal_index >= 0)
                {
                    vertex.normal = {attrib.normals[3 * index.normal_index],
                                     attrib.normals[3 * index.normal_index + 1],
                                     attrib.normals[3 * index.normal_index + 2]};
                }

                // UV 坐标（纹理坐标）
                if (index.texcoord_index >= 0)
                {
                    vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index],
                                       attrib.texcoords[2 * index.texcoord_index + 1]};
                }

                // 添加到共享顶点
                sharedVertices.push_back(vertex);
                indices.push_back(sharedVertices.size() - 1); // 将顶点索引添加到索引数组中
            }

            // 创建子实体
            auto meshEntity =
                entity->getScene()->createEntity(entity->getName() + "_mesh_" + shape.name);
            meshEntity->setParent(modelEntity);

            auto &transformComp = meshEntity->addComponent<TransformComponent>();
            transformComp.setParent(modelEntity);

            // 添加网格组件
            auto &meshComp = meshEntity->addComponent<MeshComponent>(sharedVertices, indices);

            // 关联材质
            if (shape.mesh.material_ids.size() > 0)
            {
                int materialId = shape.mesh.material_ids[0]; // 获取第一个材质ID
                if (materialId >= 0 && materialId < materialComps.size())
                {
                    meshEntity->addComponent<MaterialComponent>(
                        materialComps[materialId]); // 关联材质
                }
                else
                {
                    LOG_WARN("Material id {0} out of range", materialId);
                    meshEntity->addComponent<MaterialComponent>(); // 使用默认材质
                }
            }

            // 设置网格实体的父实体
            meshEntity->setParent(modelEntity);
        }

        return 0;
    }

    static int ProcessMaterials(const std::string &path,
                                const std::vector<tinyobj::material_t> &materials,
                                std::vector<MaterialComponent> &materialComps)
    {
        for (const auto &tinyMaterial : materials)
        {
            MaterialComponent materialComp;
            materialComp.name      = tinyMaterial.name;
            materialComp.type      = MaterialType::BlinnPhong;
            materialComp.ambient   = glm::vec3(tinyMaterial.ambient[0], tinyMaterial.ambient[1],
                                               tinyMaterial.ambient[2]);
            materialComp.diffuse   = glm::vec3(tinyMaterial.diffuse[0], tinyMaterial.diffuse[1],
                                               tinyMaterial.diffuse[2]);
            materialComp.specular  = glm::vec3(tinyMaterial.specular[0], tinyMaterial.specular[1],
                                               tinyMaterial.specular[2]);
            materialComp.shininess = tinyMaterial.shininess;

            if (!tinyMaterial.diffuse_texname.empty())
            {
                TextureSpecification spec;
                spec.internalFormat = TextureInternalFormat::SRGB_ALPHA;
                materialComp.diffuseMap =
                    TEXTURE_LIB.load(tinyMaterial.diffuse_texname,
                                     path + tinyMaterial.diffuse_texname,spec);
            }

            if (!tinyMaterial.specular_texname.empty())
            {
                materialComp.specularMap =
                    TEXTURE_LIB.load(tinyMaterial.specular_texname,
                                     path + tinyMaterial.specular_texname, TextureSpecification());
            }

            materialComps.push_back(materialComp);
        }

        return 0;
    }
};

} // namespace Airwave
