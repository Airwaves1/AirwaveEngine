#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <string>
#include "core/math/aabb.hpp"

namespace Airwave
{
class Primitive
{
  public:
    Primitive() = default;
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

    std::string position_name = "a_position";
    std::string normal_name   = "a_normal";
    std::string texcoord_name = "a_texcoord";
    std::string tangent_name  = "a_tangent";
    std::string joint_name    = "a_joint";
    std::string weight_name   = "a_weight";
};

} // namespace Airwave
