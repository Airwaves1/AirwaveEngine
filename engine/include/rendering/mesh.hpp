#pragma once

#include <glad/glad.h>
#include <vector>

namespace Airwave
{
class Mesh
{
  public:
    Mesh();

  private:
    uint32_t m_vao;
    uint32_t m_vbo;
    uint32_t m_ebo;

    uint32_t m_indexCount;
};

} // namespace Airwave
