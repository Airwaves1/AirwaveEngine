#include "gl_vertex_buffer.hpp"
#include <glad/glad.h>

namespace Airwave
{

// ==============================================================
// ======================== VertexBuffer ========================
// ==============================================================
VertexBuffer::VertexBuffer(uint32_t size)
{
    glGenBuffers(1, &m_handle);
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}
VertexBuffer::VertexBuffer(float *vertices, uint32_t size)
{
    glGenBuffers(1, &m_handle);
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_handle); }

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_handle); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::setData(uint32_t pos, uint32_t size, const void *data)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferSubData(GL_ARRAY_BUFFER, pos, size, data);
}


// ==============================================================
// ======================== IndexBuffer ========================
// ==============================================================
IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t count)
{
    glGenBuffers(1, &m_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    m_count = count;
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_handle); }

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle); }

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

} // namespace Airwave
