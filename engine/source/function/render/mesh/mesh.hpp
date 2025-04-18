#pragma once

#include <glad/glad.h>
#include <vector>
#include "function/render/material/material.hpp"
#include "core/resource/resource.hpp"
#include <variant>

namespace Airwave
{

enum class VertexType
{
    Basic,
    Skinned,
};

struct VertexBasic
{
    glm::vec3 position; // 顶点位置
    glm::vec3 normal;   // 法线
    glm::vec2 texCoord; // 纹理坐标
    glm::vec4 tangent;  // 切线
};

struct VertexSkinned
{
    glm::vec3 position; // 顶点位置
    glm::vec3 normal;   // 法线
    glm::vec2 texCoord; // 纹理坐标
    glm::vec4 tangent;  // 切线

    glm::ivec4 joints; // 骨骼索引
    glm::vec4 weights; // 权重
};

struct MeshBuffer
{
    GLuint vao = 0;
    GLuint vbo = 0; // 顶点缓冲对象
    GLuint ebo = 0; // 索引缓冲对象

    size_t vertex_count = 0; // 顶点数量
    size_t index_count  = 0; // 索引数量

    std::vector<uint32_t> indices; // 索引数据

    VertexType vertex_type = VertexType::Basic; // 顶点类型
    std::vector<VertexBasic> vertices_basic;     // 顶点数据
    std::vector<VertexSkinned> vertices_skinned; // 骨骼顶点数据
};

struct Primitive
{
    std::shared_ptr<MeshBuffer> mesh_buffer;
    std::shared_ptr<Material> material;

    size_t indexOffseet = 0;
    size_t indexCount   = 0;
    size_t vertexOffset = 0;

    int skinIndex = -1;

    ~Primitive();
};

class Mesh : public IResource
{
  public:
    Mesh(const std::string &resource_path) : IResource(resource_path) {}
    ~Mesh() override = default;

    void load() override;
    void release() override;

    std::vector<std::shared_ptr<Primitive>> primitives;
};

} // namespace Airwave
