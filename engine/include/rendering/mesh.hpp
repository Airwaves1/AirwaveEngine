#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

#include "core/common.hpp"

namespace Airwave
{
class Mesh
{
  public:
    Mesh(const std::vector<float> &vertices, const std::vector<uint32_t> &indices);
    Mesh(const std::vector<AwVertex> &vertices, const std::vector<uint32_t> &indices);

    ~Mesh();

    void draw();

    void setupInstanceing(int attributeLocation, int divisor = 1);
    void drawInstanced(uint32_t instanceCount);

    uint32_t vao = 0;
    uint32_t vbo = 0;
    uint32_t ebo = 0;

    uint32_t indexCount = 0;

    // instancing
    uint32_t instanceVBO = 0;
    bool hasInstanceing  = false;
};

} // namespace Airwave
