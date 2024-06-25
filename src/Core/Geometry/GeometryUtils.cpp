#include "GeometryUtils.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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

    void GeometryUtils::CreateSphereData(float radius, int segments, float startLatitude, float endLatitude, float startLongitude, float endLongitude, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        vertices.clear();
        indices.clear();

        constexpr  float pi = glm::pi<float>();
        const float twoPi = 2.0f * pi;

        // Convert degrees to radians
        startLatitude = glm::radians(startLatitude);
        endLatitude = glm::radians(endLatitude);
        startLongitude = glm::radians(startLongitude);
        endLongitude = glm::radians(endLongitude);

        // Calculate the number of steps in latitude and longitude
        int latSteps = static_cast<int>((endLatitude - startLatitude) / (pi / segments));
        int lonSteps = static_cast<int>((endLongitude - startLongitude) / (twoPi / segments));

        // Generate vertices
        for (int lat = 0; lat <= latSteps; ++lat) {
            float theta = startLatitude + lat * (endLatitude - startLatitude) / latSteps;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= lonSteps; ++lon) {
                float phi = startLongitude + lon * (endLongitude - startLongitude) / lonSteps;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                glm::vec3 position = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
                glm::vec3 normal = glm::normalize(position);
                glm::vec2 texCoords = glm::vec2(1.0f - (lon / static_cast<float>(lonSteps)), 1.0f - (lat / static_cast<float>(latSteps)));

                vertices.push_back({ position * radius, normal, texCoords });
            }
        }

        // Generate indices
        for (int lat = 0; lat < latSteps; ++lat) {
            for (int lon = 0; lon < lonSteps; ++lon) {
                uint32_t first = lat * (lonSteps + 1) + lon;
                uint32_t second = first + lonSteps + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }

    void GeometryUtils::CreateCylinderData(float radius, float height, int segments, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        vertices.clear();
        indices.clear();

        // Create vertices for the sides of the cylinder
        float halfHeight = height / 2.0f;
        float segmentAngle = glm::two_pi<float>() / static_cast<float>(segments);
        float texCoordXStep = 1.0f / static_cast<float>(segments);

        // Create vertices for the sides (including top and bottom rings)
        for (int i = 0; i <= segments; ++i) {
            float angle = static_cast<float>(i) * segmentAngle;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            // Bottom vertex
            vertices.push_back({ glm::vec3(x, -halfHeight, z), glm::vec3(x, 0.0f, z), glm::vec2(texCoordXStep * static_cast<float>(i), 0.0f) });

            // Top vertex
            vertices.push_back({ glm::vec3(x, halfHeight, z), glm::vec3(x, 0.0f, z), glm::vec2(texCoordXStep * static_cast<float>(i), 1.0f) });
        }

        // Create indices for the sides
        for (int i = 0; i < segments * 2; i += 2) {
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back((i + 2) % (segments * 2));

            indices.push_back((i + 2) % (segments * 2));
            indices.push_back(i + 1);
            indices.push_back((i + 3) % (segments * 2));
        }

        // Create vertices for the top and bottom faces
        uint32_t baseIndex = vertices.size();
        // Bottom center vertex
        vertices.push_back({ glm::vec3(0.0f, -halfHeight, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 0.5f) });
        // Top center vertex
        vertices.push_back({ glm::vec3(0.0f, halfHeight, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f) });

        // Create indices for the bottom face
        for (int i = 0; i < segments; ++i) {
            indices.push_back(baseIndex);
            indices.push_back(i * 2);
            indices.push_back(((i + 1) % segments) * 2);
        }

        // Create indices for the top face
        baseIndex++;
        for (int i = 0; i < segments; ++i) {
            indices.push_back(baseIndex);
            indices.push_back(i * 2 + 1);
            indices.push_back(((i + 1) % segments) * 2 + 1);
        }
    }

    void GeometryUtils::CreateConeData(float radius, float height, int segments, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        vertices.clear();
        indices.clear();

        // Create vertices for the base circle
        float halfHeight = height / 2.0f;
        float segmentAngle = glm::two_pi<float>() / static_cast<float>(segments);
        float texCoordXStep = 1.0f / static_cast<float>(segments);

        // Center vertex for the base
        vertices.push_back({ glm::vec3(0.0f, -halfHeight, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 0.5f) });

        // Vertices for the base circle
        for (int i = 0; i < segments; ++i) {
            float angle = static_cast<float>(i) * segmentAngle;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            vertices.push_back({ glm::vec3(x, -halfHeight, z), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(texCoordXStep * static_cast<float>(i), 0.0f) });
        }

        // Indices for the base circle
        for (int i = 2; i <= segments; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i == segments ? 1 : i + 1);
        }

        // Vertex for the tip of the cone
        vertices.push_back({ glm::vec3(0.0f, halfHeight, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f) });

        // Vertices for the sides of the cone
        for (int i = 0; i < segments; ++i) {
            float angle = static_cast<float>(i) * segmentAngle;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            vertices.push_back({ glm::vec3(x, -halfHeight, z), glm::normalize(glm::vec3(x, radius, z)), glm::vec2(texCoordXStep * static_cast<float>(i), 1.0f) });
        }

        // Indices for the sides of the cone
        for (int i = 1; i <= segments; ++i) {
            indices.push_back(vertices.size() - 1);
            indices.push_back(i);
            indices.push_back(i == segments ? 1 : i + 1);
        }
    }

    void GeometryUtils::CreatePlaneData(float width, float height, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        vertices.clear();
        indices.clear();

        // Vertices
        vertices.push_back({ glm::vec3(-width / 2.0f, 0.0f, -height / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
        vertices.push_back({ glm::vec3(width / 2.0f, 0.0f, -height / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
        vertices.push_back({ glm::vec3(width / 2.0f, 0.0f, height / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) });
        vertices.push_back({ glm::vec3(-width / 2.0f, 0.0f, height / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) });

        // Indices
        indices.push_back(0); indices.push_back(1); indices.push_back(2);
        indices.push_back(2); indices.push_back(3); indices.push_back(0);
    }

    void GeometryUtils::CreateRingData(float innerRadius, float outerRadius, int segments, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        vertices.clear();
        indices.clear();

        float segmentAngle = glm::two_pi<float>() / static_cast<float>(segments);
        float texCoordXStep = 1.0f / static_cast<float>(segments);

        // Create vertices for the outer circle
        for (int i = 0; i < segments; ++i) {
            float angle = static_cast<float>(i) * segmentAngle;
            float x = outerRadius * cos(angle);
            float z = outerRadius * sin(angle);
            vertices.push_back({ glm::vec3(x, 0.0f, z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(texCoordXStep * static_cast<float>(i), 1.0f) });
        }

        // Create vertices for the inner circle
        for (int i = 0; i < segments; ++i) {
            float angle = static_cast<float>(i) * segmentAngle;
            float x = innerRadius * cos(angle);
            float z = innerRadius * sin(angle);
            vertices.push_back({ glm::vec3(x, 0.0f, z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(texCoordXStep * static_cast<float>(i), 0.0f) });
        }

        // Create indices for the ring
        for (int i = 0; i < segments; ++i) {
            int next = (i + 1) % segments;
            // Outer circle vertices
            int outerStart = i;
            int outerEnd = (i + 1) % segments;
            // Inner circle vertices
            int innerStart = i + segments;
            int innerEnd = (i + 1) % segments + segments;

            // Triangle 1
            indices.push_back(outerStart);
            indices.push_back(innerEnd);
            indices.push_back(innerStart);

            // Triangle 2
            indices.push_back(outerStart);
            indices.push_back(outerEnd);
            indices.push_back(innerEnd);
        }
    }

}