#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory> 
#include "core/common.hpp"
#include "rendering/primitive.hpp"

//
const double PI = 3.14159265358979323846;

namespace Airwave
{

class GeometryUtils
{
  public:
    static std::shared_ptr<Primitive> CreateQuad(float width = 1.0f, float height = 1.0f, float repeatX = 1.0f, float repeatY = 1.0f);

    static std::shared_ptr<Primitive> CreatePlane(float width = 1.0, float height = 1.0, int widthSegments = 1.0, int heightSegments = 1.0,
                                                  float repeatX = 1.0f, float repeatY = 1.0f);

    static std::shared_ptr<Primitive> CreateCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f, int widthSegments = 1,
                                                 int heightSegments = 1, int depthSegments = 1);

    static std::shared_ptr<Primitive> CreateSphere(float radius = 1.0f, int widthSegments = 32, int heightSegments = 32, float phiStart = 0.0f,
                                                   float phiLength = 2 * PI, float thetaStart = 0.0f, float thetaLength = PI);

    // 函数将顶点数据转换为扁平化的 float 数组
    static std::vector<float> ConvertWaveVertexToFloatArray(const std::vector<Airwave::AwVertex> &vertices)
    {
        std::vector<float> result;
        result.reserve(vertices.size() * 8); // 每个顶点包含5个浮点数

        for (const auto &vertex : vertices)
        {
            // 将位置、纹理坐标和法线数据转换为 float 数组
            result.push_back(vertex.position.x);
            result.push_back(vertex.position.y);
            result.push_back(vertex.position.z);

            result.push_back(vertex.normal.x);
            result.push_back(vertex.normal.y);
            result.push_back(vertex.normal.z);

            result.push_back(vertex.texCoord.x);
            result.push_back(vertex.texCoord.y);
        }

        return result;
    }

  public:
    /**
     * Plane 平面
     *
     * width: 1.0f
     * height: 1.0f
     * widthSegments: 1
     * heightSegments: 1
     */
    static void CreatePlane(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices, float width = 1.0, float height = 1.0,
                            int widthSegments = 1.0, int heightSegments = 1.0, float repeatX = 1.0f, float repeatY = 1.0f);

    /**
     * Cube 立方体
     *
     * width: 1.0f
     * height: 1.0f
     * depth: 1.0f
     * widthSegments: 1
     * heightSegments: 1
     * depthSegments: 1
     */
    static void CreateCube(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices, float width = 1.0f, float height = 1.0f,
                           float depth = 1.0f, int widthSegments = 1, int heightSegments = 1, int depthSegments = 1);

    /**
     * Sphere 球体
     *
     * radius: 1.0f
     * widthSegments: 32
     * heightSegments: 32
     * phiStart: 0.0f
     * phiLength: 2 * PI
     * thetaStart: 0.0f
     * thetaLength: PI
     */
    static void CreateSphere(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices, float radius = 1.0f, int widthSegments = 32,
                             int heightSegments = 32, float phiStart = 0.0f, float phiLength = 2 * PI, float thetaStart = 0.0f,
                             float thetaLength = PI);

    /**
     * Cylinder 圆柱体
     *
     * radiusTop: 1.0f
     * radiusBottom: 1.0f
     * height: 1.0f
     * radialSegments: 32
     * heightSegments: 1
     * openEnded: false
     * thetaStart: 0.0f
     * thetaLength: 2 * PI
     */
    static void CreateCylinder(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices, float radiusTop = 1.0f, float radiusBottom = 1.0f,
                               float height = 1.0f, int radialSegments = 32, int heightSegments = 1, bool openEnded = false, float thetaStart = 0.0f,
                               float thetaLength = 2 * PI);

    /**
     * Cone 圆锥
     *
     * radius: 1.0f
     * height: 1.0f
     * radialSegments: 32
     * heightSegments: 1
     * openEnded: false
     * thetaStart: 0.0f
     * thetaLength: 2 * PI
     */
    static void CreateCone(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices, float radius = 1.0f, float height = 1.0f,
                           int radialSegments = 32, int heightSegments = 1, bool openEnded = false, float thetaStart = 0.0f,
                           float thetaLength = 2 * PI);

    /**
     * Torus 圆环
     *
     * radius: 1.0f
     * tube: 0.4f
     * radialSegments: 32
     * tubularSegments: 12
     * arc: 2 * PI
     */
    static void CreateTorus(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices, float radius = 1.0f, float tube = 0.4f,
                            int radialSegments = 32, int tubularSegments = 12, float arc = 2 * PI);

    /**
     * TorusKnot 圆环结
     *
     * radius: 1.0f
     * tube: 0.4f
     * tubularSegments: 64
     * radialSegments: 8
     * p: 2
     * q: 3
     */
    static void CreateTorusKnot(std::vector<AwVertex> &vertices, std::vector<uint32_t> &indices, float radius = 1.0f, float tube = 0.4f,
                                int tubularSegments = 64, int radialSegments = 8, float p = 2, float q = 3);
};

} // namespace Airwave
