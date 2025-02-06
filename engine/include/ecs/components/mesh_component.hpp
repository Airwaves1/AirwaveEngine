#pragma once

#include <glad/glad.h>
#include <memory>

#include "rendering/opengl/gl_vertex_array.hpp"
#include "rendering/opengl/gl_vertex_buffer.hpp"
#include "core/log.hpp"
#include "ecs/aw_component.hpp"
#include "core/common.hpp"
#include "utils/geometry_utils.hpp"

namespace Airwave
{
class MeshComponent : public AwComponent
{
  public:
    MeshComponent() {}

    MeshComponent(const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
    {
        m_vertices = vertices;
        m_indices  = indices;
        updateMesh();
    }

    MeshComponent(const std::vector<AwVertex> &vertices, const std::vector<uint32_t> &indices)
    {
        m_vertices = GeometryUtils::ConvertWaveVertexToFloatArray(vertices);
        m_indices  = indices;
        updateMesh();
    }

    ~MeshComponent() {}

    void setData(const std::vector<float> &vertices, const std::vector<uint32_t> &indices)
    {
        m_vertices = vertices;
        m_indices  = indices;
        updateMesh();
    }

    void setData(const std::vector<AwVertex> &vertices, const std::vector<uint32_t> &indices)
    {
        m_vertices = GeometryUtils::ConvertWaveVertexToFloatArray(vertices);
        m_indices  = indices;
        updateMesh();
    }

    void setVertices(const std::vector<float> &vertices)
    {
        m_vertices = vertices;
        updateMesh();
    }

    void setVertices(const std::vector<AwVertex> &vertices)
    {
        m_vertices = GeometryUtils::ConvertWaveVertexToFloatArray(vertices);
        updateMesh();
    }

    void setIndices(const std::vector<uint32_t> &indices)
    {
        m_indices = indices;
        updateMesh();
    }

    std::shared_ptr<VertexArray> getVertexArray() const { return m_vertexArray; }

    void setVertexArray(std::shared_ptr<VertexArray> vertexArray) { m_vertexArray = vertexArray; }

    void draw()
    {
        if (!m_vertexArray)
        {
            static bool flag = false;
            if (!flag)
            {
                LOG_ERROR("MeshComponent::draw: vertexArray is nullptr!");
                flag = true;
            }
            return;
        }
        m_vertexArray->bind();
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    uint32_t getVertexCount() const { return m_vertices.size(); }
    uint32_t getIndexCount() const { return m_indices.size(); }

  private:
    void updateMesh()
    {
        if (m_vertices.empty())
        {
            LOG_ERROR("MeshComponent::updateMesh: vertices is empty!");
            return;
        }

        if (!m_vertexArray)
        {
            m_vertexArray = std::make_shared<VertexArray>();
        }
        m_vertexArray->bind();
        {
            auto vertexBuffer = std::make_shared<VertexBuffer>(m_vertices.data(),
                                                               m_vertices.size() * sizeof(float));
            vertexBuffer->setLayout({
                {ShaderDataType::Float3, "a_position"},
                {ShaderDataType::Float3, "a_normal"},
                {ShaderDataType::Float2, "a_texCoord"},
            });
            m_vertexArray->addVertexBuffer(vertexBuffer);

            auto indexBuffer = std::make_shared<IndexBuffer>(m_indices.data(), m_indices.size());
            m_vertexArray->setIndexBuffer(indexBuffer);
        }
        m_vertexArray->unbind();
    }

  private:
    std::shared_ptr<VertexArray> m_vertexArray;
    std::vector<float> m_vertices;
    std::vector<uint32_t> m_indices;
};

} // namespace Airwave
