#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H

#include "Utils/Common.h"
#include <glm/glm.hpp>

namespace Airwave{

    struct Vertex{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

    /*
    *生成基本几何体的顶点数据和索引数据
    * 1.立方体
    * 2.球体
    * 3.圆柱体
    * 4.圆锥体
    * 5.平面
    * 6.圆环
    * 
    * 
    */
    class GeometryUtils{
        public:
            GeometryUtils() {}
            ~GeometryUtils() {}

            /*
             * 立方体
             *
             * @param width 立方体的宽度。
             * @param height 立方体的高度。
             * @param depth 立方体的深度。
             * @param vertices 用于存储顶点数据的向量。
             * @param indices 用于存储索引数据的向量。
             */
            static void CreateBoxData(float width, float height, float depth, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

            /*
             * 球体
             *
             * @param radius 球体的半径。
             * @param segments 球体的分段数，控制球体的精细度。
             * @param startLatitude 球体顶部的起始纬度（角度）。
             * @param endLatitude 球体底部的结束纬度（角度）。
             * @param startLongitude 球体经度的起始角度（从正前方开始顺时针旋转）。
             * @param endLongitude 球体经度的结束角度（从正前方开始顺时针旋转）。
             * @param vertices 用于存储顶点数据的向量。
             * @param indices 用于存储索引数据的向量。
             */
            static void CreateSphereData(float radius, int segments, float startLatitude, float endLatitude, float startLongitude, float endLongitude, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);


            /*
            * 圆柱体
            *
            * @param radius 圆柱体的半径。
            * @param height 圆柱体的高度。
            * @param segments 圆柱体的分段数，控制圆柱体的精细度。
            * @param vertices 用于存储顶点数据的向量。
            * @param indices 用于存储索引数据的向量。
            */
            static void CreateCylinderData(float radius, float height, int segments, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);


            /*
             * 圆锥体
             *
             * @param radius 圆锥体底部的半径。
             * @param height 圆锥体的高度。
             * @param segments 圆锥体的分段数，控制圆锥体的精细度。
             * @param vertices 用于存储顶点数据的向量。
             * @param indices 用于存储索引数据的向量。
             */
            static void CreateConeData(float radius, float height, int segments, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

            /*
             * 平面
             *
             * @param width 平面的宽度。
             * @param height 平面的高度。
             * @param vertices 用于存储顶点数据的向量。
             * @param indices 用于存储索引数据的向量。
             */
            static void CreatePlaneData(float width, float height, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

            /*
             * 圆环
             *
             * @param innerRadius 圆环内圆的半径。
             * @param outerRadius 圆环外圆的半径。
             * @param segments 圆环的分段数，控制圆环的精细度。
             * @param vertices 用于存储顶点数据的向量。
             * @param indices 用于存储索引数据的向量。
             */
            static void CreateRingData(float innerRadius, float outerRadius, int segments, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);



        private:


    };

}


#endif // GEOMETRY_UTILS_H