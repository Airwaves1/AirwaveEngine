#include "GeometryUtils.h"


namespace Airwave {

	void GeometryUtils::CreateBoxData(float width, float height, float depth, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
	{
		vertices.clear();
		indices.clear();

        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        float halfDepth = depth / 2.0f;

        // Define vertices for each face of the box
        // Front face
        vertices.push_back({ glm::vec3(-halfWidth, -halfHeight,  halfDepth), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) });
        vertices.push_back({ glm::vec3( halfWidth, -halfHeight,  halfDepth), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) });
        vertices.push_back({ glm::vec3( halfWidth,  halfHeight,  halfDepth), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) });
        vertices.push_back({ glm::vec3(-halfWidth,  halfHeight,  halfDepth), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) });

        // Back face
        vertices.push_back({ glm::vec3(-halfWidth, -halfHeight, -halfDepth), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f) });
        vertices.push_back({ glm::vec3( halfWidth, -halfHeight, -halfDepth), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f) });
        vertices.push_back({ glm::vec3( halfWidth,  halfHeight, -halfDepth), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f) });
        vertices.push_back({ glm::vec3(-halfWidth,  halfHeight, -halfDepth), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f) });

        // Left face
        vertices.push_back({ glm::vec3(-halfWidth, -halfHeight, -halfDepth), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
        vertices.push_back({ glm::vec3(-halfWidth, -halfHeight,  halfDepth), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) });
        vertices.push_back({ glm::vec3(-halfWidth,  halfHeight,  halfDepth), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) });
        vertices.push_back({ glm::vec3(-halfWidth,  halfHeight, -halfDepth), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) });

        // Right face
        vertices.push_back({ glm::vec3(halfWidth, -halfHeight, -halfDepth), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
        vertices.push_back({ glm::vec3(halfWidth,  halfHeight, -halfDepth), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) });
        vertices.push_back({ glm::vec3(halfWidth,  halfHeight,  halfDepth), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) });
        vertices.push_back({ glm::vec3(halfWidth, -halfHeight,  halfDepth), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) });

        // Top face
        vertices.push_back({ glm::vec3(-halfWidth,  halfHeight, -halfDepth), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) });
        vertices.push_back({ glm::vec3( halfWidth,  halfHeight, -halfDepth), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) });
        vertices.push_back({ glm::vec3( halfWidth,  halfHeight,  halfDepth), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
        vertices.push_back({ glm::vec3(-halfWidth,  halfHeight,  halfDepth), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });

        // Bottom face
        vertices.push_back({ glm::vec3(-halfWidth, -halfHeight, -halfDepth), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
        vertices.push_back({ glm::vec3( halfWidth, -halfHeight, -halfDepth), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
        vertices.push_back({ glm::vec3( halfWidth, -halfHeight,  halfDepth), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) });
        vertices.push_back({ glm::vec3(-halfWidth, -halfHeight,  halfDepth), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) });

        // Define indices for the box
        // Front face
        indices.push_back(0); indices.push_back(1); indices.push_back(2);
        indices.push_back(2); indices.push_back(3); indices.push_back(0);

        // Back face
        indices.push_back(4); indices.push_back(5); indices.push_back(6);
        indices.push_back(6); indices.push_back(7); indices.push_back(4);

        // Left face
        indices.push_back(8); indices.push_back(9); indices.push_back(10);
        indices.push_back(10); indices.push_back(11); indices.push_back(8);

        // Right face
        indices.push_back(12); indices.push_back(13); indices.push_back(14);
        indices.push_back(14); indices.push_back(15); indices.push_back(12);

        // Top face
        indices.push_back(16); indices.push_back(17); indices.push_back(18);
        indices.push_back(18); indices.push_back(19); indices.push_back(16);

        // Bottom face
        indices.push_back(20); indices.push_back(21); indices.push_back(22);
        indices.push_back(22); indices.push_back(23); indices.push_back(20);

	}

}