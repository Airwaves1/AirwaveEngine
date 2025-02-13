#pragma once

#include <cstdint>
#include <glad/glad.h>
#include "core/math/aabb.hpp"

namespace Airwave
{
class Primitive
{
  public:
    Primitive()          = default;
    ~Primitive();

    void draw();
    void drawInstanced(uint32_t instanceCount);

  public:
    uint32_t vao = 0;
    uint32_t vbo = 0;
    uint32_t ebo = 0;

    uint32_t material_index = 0;

    uint32_t indexCount = 0;
    GLenum indexType    = GL_UNSIGNED_INT;

    Math::AABB bounding_box;
};

} // namespace Airwave
