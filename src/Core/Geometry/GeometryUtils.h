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

    class GeometryUtils{
        public:
            GeometryUtils() {}
            ~GeometryUtils() {}

            //创建立方体，球体，圆柱体，圆锥体，平面，圆环等几何体顶点数据

            static void CreateBoxData(float width, float height, float depth, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

        private:


    };

}


#endif // GEOMETRY_UTILS_H