#include "utils/geometry_utils.hpp"

namespace Airwave
{
void GeometryUtils::CreatePlane(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices,
                                float width, float height, int widthSegments, int heightSegments,
                                float repeatX, float repeatY )
{
    // 计算每个段的宽度和高度
    float segmentWidth  = width / widthSegments;
    float segmentHeight = height / heightSegments;

    // 生成顶点数据
    for (size_t y = 0; y <= heightSegments; y++)
    {
        for (size_t x = 0; x <= widthSegments; x++)
        {
            AwVertex vertex;

            vertex.position =
                glm::vec3(x * segmentWidth - width / 2.0f, 0.0f, y * segmentHeight - height / 2.0f);

            // UV坐标乘以重复因子，实现纹理重复
            vertex.texCoord = glm::vec2(static_cast<float>(x) / widthSegments * repeatX,
                                        static_cast<float>(y) / heightSegments * repeatY);

            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);

            vertices.push_back(vertex);
        }
    }

    // 生成索引数据
    for (size_t y = 0; y < heightSegments; y++)
    {
        for (size_t x = 0; x < widthSegments; x++)
        {
            uint32_t topLeft     = y * (widthSegments + 1) + x;
            uint32_t topRight    = topLeft + 1;
            uint32_t bottomLeft  = (y + 1) * (widthSegments + 1) + x;
            uint32_t bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}


void GeometryUtils::CreateCube(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices,
                               float width, float height, float depth, int widthSegments,
                               int heightSegments, int depthSegments)
{
    float halfWidth  = width / 2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth  = depth / 2.0f;

    // 用于生成一个面的顶点和索引,
    // 接受面法线、四个角落的位置，以及每个方向上的细分数（即每个面上的网格细分
    auto addFace = [&](const glm::vec3 &normal, const glm::vec3 &corner1, const glm::vec3 &corner2,
                       const glm::vec3 &corner3, const glm::vec3 &corner4, int widthSeg,
                       int heightSeg) -> void
    {
        int baseIndex = vertices.size();

        for (int y = 0; y <= heightSeg; ++y)
        {
            for (int x = 0; x <= widthSeg; ++x)
            {
                AwVertex vertex;
                vertex.position =
                    glm::mix(glm::mix(corner1, corner2, x / float(widthSeg)),
                             glm::mix(corner3, corner4, x / float(widthSeg)), y / float(heightSeg));
                vertex.normal   = normal;
                vertex.texCoord = glm::vec2(x / float(widthSeg), y / float(heightSeg));
                vertices.push_back(vertex);
            }
        }

        // Add indices for the face
        for (int y = 0; y < heightSeg; ++y)
        {
            for (int x = 0; x < widthSeg; ++x)
            {
                int i0 = baseIndex + y * (widthSeg + 1) + x;
                int i1 = i0 + 1;
                int i2 = i0 + (widthSeg + 1);
                int i3 = i2 + 1;

                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i1);

                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i3);
            }
        }
    };

    // Define cube faces with their normal vectors
    glm::vec3 normalFront(0.0f, 0.0f, 1.0f);
    glm::vec3 normalBack(0.0f, 0.0f, -1.0f);
    glm::vec3 normalTop(0.0f, 1.0f, 0.0f);
    glm::vec3 normalBottom(0.0f, -1.0f, 0.0f);
    glm::vec3 normalLeft(-1.0f, 0.0f, 0.0f);
    glm::vec3 normalRight(1.0f, 0.0f, 0.0f);

    glm::vec3 cornerFrontTopLeft(-halfWidth, halfHeight, halfDepth);
    glm::vec3 cornerFrontTopRight(halfWidth, halfHeight, halfDepth);
    glm::vec3 cornerFrontBottomLeft(-halfWidth, -halfHeight, halfDepth);
    glm::vec3 cornerFrontBottomRight(halfWidth, -halfHeight, halfDepth);

    glm::vec3 cornerBackTopLeft(-halfWidth, halfHeight, -halfDepth);
    glm::vec3 cornerBackTopRight(halfWidth, halfHeight, -halfDepth);
    glm::vec3 cornerBackBottomLeft(-halfWidth, -halfHeight, -halfDepth);
    glm::vec3 cornerBackBottomRight(halfWidth, -halfHeight, -halfDepth);

    // Front face
    addFace(normalFront, cornerFrontTopLeft, cornerFrontTopRight, cornerFrontBottomLeft,
            cornerFrontBottomRight, widthSegments, heightSegments);
    // Back face
    addFace(normalBack, cornerBackTopRight, cornerBackTopLeft, cornerBackBottomRight,
            cornerBackBottomLeft, widthSegments, heightSegments);
    // Top face
    addFace(normalTop, cornerFrontTopRight, cornerFrontTopLeft, cornerBackTopRight,
            cornerBackTopLeft, widthSegments, depthSegments);
    // Bottom face
    addFace(normalBottom, cornerFrontBottomLeft, cornerFrontBottomRight, cornerBackBottomLeft,
            cornerBackBottomRight, widthSegments, depthSegments);
    // Left face
    addFace(normalLeft, cornerFrontTopLeft, cornerBackTopLeft, cornerFrontBottomLeft,
            cornerBackBottomLeft, depthSegments, heightSegments);
    // Right face
    addFace(normalRight, cornerBackTopRight, cornerFrontTopRight, cornerBackBottomRight,
            cornerFrontBottomRight, depthSegments, heightSegments);
}

void GeometryUtils::CreateSphere(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices,
                                 float radius, int widthSegments, int heightSegments,
                                 float phiStart, float phiLength, float thetaStart,
                                 float thetaLength)
{
    for (int y = 0; y <= heightSegments; ++y)
    {
        float v        = float(y) / heightSegments;
        float theta    = thetaStart + v * thetaLength;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int x = 0; x <= widthSegments; ++x)
        {
            float u      = float(x) / widthSegments;
            float phi    = phiStart + u * phiLength;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            AwVertex vertex;
            vertex.position = glm::vec3(radius * cosPhi * sinTheta, radius * cosTheta, radius * sinPhi * sinTheta);
            vertex.texCoord = glm::vec2(u, v);
            vertex.normal   = glm::normalize(vertex.position);

            vertices.push_back(vertex);
        }
    }

    for (int y = 0; y < heightSegments; ++y)
    {
        for (int x = 0; x < widthSegments; ++x)
        {
            int i0 = y * (widthSegments + 1) + x;
            int i1 = i0 + 1;
            int i2 = i0 + (widthSegments + 1);
            int i3 = i2 + 1;

            // 确保逆时针顺序
            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }
}

void GeometryUtils::CreateCylinder(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices,
                                   float radiusTop, float radiusBottom, float height,
                                   int radialSegments, int heightSegments, bool openEnded,
                                   float thetaStart, float thetaLength)
{
    float halfHeight    = height / 2.0f;
    float segmentHeight = height / heightSegments;
    float radiusDiff    = radiusBottom - radiusTop;
    float slope         = radiusDiff / height;

    for (int y = 0; y <= heightSegments; ++y)
    {
        float v      = y / static_cast<float>(heightSegments);
        float radius = v * radiusDiff + radiusTop;

        for (int x = 0; x <= radialSegments; ++x)
        {
            float u        = x / static_cast<float>(radialSegments);
            float theta    = thetaStart + u * thetaLength;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            AwVertex vertex;
            vertex.position =
                glm::vec3(radius * sinTheta, -v * height + halfHeight, radius * cosTheta);
            vertex.texCoord = glm::vec2(u, 1.0f - v);

            glm::vec3 tangent   = glm::vec3(-cosTheta, 0.0f, sinTheta);
            glm::vec3 bitangent = glm::vec3(radius * cosTheta, 0.0f, radius * sinTheta);

            glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));

            vertex.normal = normal;

            vertices.push_back(vertex);
        }
    }

    int stride = radialSegments + 1;

    for (int y = 0; y < heightSegments; ++y)
    {
        for (int x = 0; x < radialSegments; ++x)
        {
            int i0 = y * stride + x;
            int i1 = i0 + 1;
            int i2 = i0 + stride;
            int i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }
}
void GeometryUtils::CreateCone(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices,
                               float radius, float height, int radialSegments, int heightSegments,
                               bool openEnded, float thetaStart, float thetaLength)
{
    float halfHeight    = height / 2.0f;
    float segmentHeight = height / heightSegments;

    for (int y = 0; y <= heightSegments; ++y)
    {
        float v      = y / static_cast<float>(heightSegments);
        float radius = (1.0f - v) * radius;

        for (int x = 0; x <= radialSegments; ++x)
        {
            float u        = x / static_cast<float>(radialSegments);
            float theta    = thetaStart + u * thetaLength;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            AwVertex vertex;
            vertex.position =
                glm::vec3(radius * sinTheta, -v * height + halfHeight, radius * cosTheta);
            vertex.texCoord = glm::vec2(u, 1.0f - v);

            glm::vec3 tangent   = glm::vec3(-cosTheta, 0.0f, sinTheta);
            glm::vec3 bitangent = glm::vec3(radius * cosTheta, 0.0f, radius * sinTheta);

            glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));

            vertex.normal = normal;

            vertices.push_back(vertex);
        }
    }

    int stride = radialSegments + 1;

    for (int y = 0; y < heightSegments; ++y)
    {
        for (int x = 0; x < radialSegments; ++x)
        {
            int i0 = y * stride + x;
            int i1 = i0 + 1;
            int i2 = i0 + stride;
            int i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }
}
void GeometryUtils::CreateTorus(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices,
                                float radius, float tube, int radialSegments, int tubularSegments,
                                float arc)
{
    for (int y = 0; y <= tubularSegments; ++y)
    {
        for (int x = 0; x <= radialSegments; ++x)
        {
            float u = x / static_cast<float>(radialSegments);
            float v = y / static_cast<float>(tubularSegments);

            float theta = u * arc;
            float phi   = v * 2.0f * PI;

            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            float sinPhi   = sin(phi);
            float cosPhi   = cos(phi);

            AwVertex vertex;
            vertex.position = glm::vec3((radius + tube * cosPhi) * cosTheta, tube * sinPhi,
                                        (radius + tube * cosPhi) * sinTheta);
            vertex.texCoord = glm::vec2(u, v);

            glm::vec3 tangent   = glm::vec3(-sinTheta, 0.0f, cosTheta);
            glm::vec3 bitangent = glm::vec3(cosTheta * (-sinPhi), cosPhi, sinTheta * (-sinPhi));

            glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));

            vertex.normal = normal;

            vertices.push_back(vertex);
        }
    }

    int stride = radialSegments + 1;

    for (int y = 0; y < tubularSegments; ++y)
    {
        for (int x = 0; x < radialSegments; ++x)
        {
            int i0 = y * stride + x;
            int i1 = i0 + 1;
            int i2 = i0 + stride;
            int i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }
}

void GeometryUtils::CreateTorusKnot(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices,
                                    float radius, float tube, int tubularSegments,
                                    int radialSegments, float p, float q)
{
    for (int y = 0; y <= tubularSegments; ++y)
    {
        for (int x = 0; x <= radialSegments; ++x)
        {
            float u = x / static_cast<float>(radialSegments);
            float v = y / static_cast<float>(tubularSegments);

            float phi   = 2.0f * PI * u * p;
            float theta = 2.0f * PI * v * q;

            float sinPhi   = sin(phi);
            float cosPhi   = cos(phi);
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            AwVertex vertex;
            vertex.position = glm::vec3((radius + tube * cosPhi) * cosTheta,
                                        (radius + tube * cosPhi) * sinTheta, tube * sinPhi);
            vertex.texCoord = glm::vec2(u, v);

            glm::vec3 tangent   = glm::vec3(-sinTheta * (radius + tube * cosPhi), -sinPhi * tube,
                                            cosTheta * (radius + tube * cosPhi));
            glm::vec3 bitangent = glm::vec3(cosTheta * cosPhi, -sinPhi, sinTheta * cosPhi);

            glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));

            vertex.normal = normal;

            vertices.push_back(vertex);
        }
    }

    int stride = radialSegments + 1;

    for (int y = 0; y < tubularSegments; ++y)
    {
        for (int x = 0; x < radialSegments; ++x)
        {
            int i0 = y * stride + x;
            int i1 = i0 + 1;
            int i2 = i0 + stride;
            int i3 = i2 + 1;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }
}
} // namespace Airwave