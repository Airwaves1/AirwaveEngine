#pragma once

#include "rendering/opengl/gl_vertex_array.hpp"
#include "rendering/opengl/gl_vertex_buffer.hpp"
#include "geometry_utils.hpp"

namespace Airwave
{
class ShapesVAO
{
  public:
    static std::shared_ptr<VertexArray> CreatePlane(float width = 1.0, float height = 1.0, int widthSegments = 1.0,
                                                    int heightSegments = 1.0, float repeatX = 1.0f, float repeatY = 1.0f)
    {
        std::vector<AwVertex> vertices;
        std::vector<uint32_t> indices;
        GeometryUtils::CreatePlane(vertices, indices, width, height, widthSegments, heightSegments, repeatX, repeatY);
        auto singleVerices = GeometryUtils::ConvertWaveVertexToFloatArray(vertices);

        auto vertexArray = std::make_shared<VertexArray>();
        vertexArray->bind();
        {
            auto vertexBuffer = std::make_shared<VertexBuffer>(
                singleVerices.data(), singleVerices.size() * sizeof(float));
            vertexBuffer->setLayout({
                {ShaderDataType::Float3, "a_position"},
                {ShaderDataType::Float3, "a_normal"},
                {ShaderDataType::Float2, "a_texCoord"},
            });
            vertexArray->addVertexBuffer(vertexBuffer);

            auto indexBuffer = std::make_shared<IndexBuffer>(static_cast<uint32_t *>(indices.data()), indices.size());

            vertexArray->setIndexBuffer(indexBuffer);
        }
        vertexArray->unbind();

        return vertexArray;
    }

    static std::shared_ptr<VertexArray> CreateCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f,
                                                   int widthSegments = 1, int heightSegments = 1, int depthSegments = 1)
    {
        std::vector<AwVertex> vertices;
        std::vector<uint32_t> indices;
        GeometryUtils::CreateCube(vertices, indices, width, height, depth, widthSegments, heightSegments, depthSegments);
        auto singleVerices = GeometryUtils::ConvertWaveVertexToFloatArray(vertices);

        auto vertexArray = std::make_shared<VertexArray>();
        vertexArray->bind();
        {
            auto vertexBuffer = std::make_shared<VertexBuffer>(
                singleVerices.data(), singleVerices.size() * sizeof(float));
            vertexBuffer->setLayout({
                {ShaderDataType::Float3, "a_position"},
                {ShaderDataType::Float3, "a_normal"},
                {ShaderDataType::Float2, "a_texCoord"},
            });
            vertexArray->addVertexBuffer(vertexBuffer);

            auto indexBuffer = std::make_shared<IndexBuffer>(static_cast<uint32_t *>(indices.data()), indices.size());

            vertexArray->setIndexBuffer(indexBuffer);
        }
        vertexArray->unbind();

        return vertexArray;
    }

    static std::shared_ptr<VertexArray> CreateSphere(float radius = 1.0f, int widthSegments = 32, int heightSegments = 32,
                                                     float phiStart = 0.0f, float phiLength = 2 * PI,
                                                     float thetaStart = 0.0f, float thetaLength = PI)
    {
        std::vector<AwVertex> vertices;
        std::vector<uint32_t> indices;
        GeometryUtils::CreateSphere(vertices, indices, radius, widthSegments, heightSegments, phiStart, phiLength, thetaStart, thetaLength);
        auto singleVerices = GeometryUtils::ConvertWaveVertexToFloatArray(vertices);

        auto vertexArray = std::make_shared<VertexArray>();
        vertexArray->bind();
        {
            auto vertexBuffer = std::make_shared<VertexBuffer>(
                singleVerices.data(), singleVerices.size() * sizeof(float));
            vertexBuffer->setLayout({
                {ShaderDataType::Float3, "a_position"},
                {ShaderDataType::Float3, "a_normal"},
                {ShaderDataType::Float2, "a_texCoord"},
            });
            vertexArray->addVertexBuffer(vertexBuffer);

            auto indexBuffer = std::make_shared<IndexBuffer>(static_cast<uint32_t *>(indices.data()), indices.size());

            vertexArray->setIndexBuffer(indexBuffer);
        }
        vertexArray->unbind();

        return vertexArray;
    }


  private:
};

} // namespace Airwave
