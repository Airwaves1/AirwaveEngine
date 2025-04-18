#include "mesh.hpp"

namespace Airwave
{
Primitive::~Primitive()
{
    if (mesh_buffer)
    {
        if (mesh_buffer->vbo) glDeleteBuffers(1, &mesh_buffer->vbo);
        if (mesh_buffer->ebo) glDeleteBuffers(1, &mesh_buffer->ebo);
        if (mesh_buffer->vao) glDeleteVertexArrays(1, &mesh_buffer->vao);
    }
}

void Mesh::load() {}

void Mesh::release() {}

} // namespace Airwave
