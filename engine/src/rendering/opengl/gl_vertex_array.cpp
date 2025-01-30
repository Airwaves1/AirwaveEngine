#include "rendering/opengl/gl_vertex_array.hpp"
#include <glad/glad.h>

namespace Airwave
{
static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        case ShaderDataType::None:
            return 0;
    }
    return 0;
}

VertexArray::VertexArray() { glGenVertexArrays(1, &m_handle); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_handle); }

void VertexArray::bind() const { glBindVertexArray(m_handle); }

void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
{
    glBindVertexArray(m_handle);
    vertexBuffer->bind();

    BufferLayout layout = vertexBuffer->getLayout();
    int index           = 0;
    for (const BufferElement &element : layout)
    {
        glEnableVertexAttribArray(index);
        if (element.isInteger())
        {
            glVertexAttribIPointer(index, element.getComponentCount(),
                                   shaderDataTypeToOpenGLBaseType(element.getType()),
                                   layout.getStride(), (const void *)(uint64_t)element.getOffset());
        }
        else
        {
            glVertexAttribPointer(index, element.getComponentCount(),
                                  shaderDataTypeToOpenGLBaseType(element.getType()),
                                  element.isNormalized(), layout.getStride(),
                                  (const void *)(uint64_t)element.getOffset());
        }
        index++;
    }
    m_vertexBuffers.push_back(vertexBuffer);
}

void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{
    glBindVertexArray(m_handle);
    indexBuffer->bind();
    m_indexBuffer = indexBuffer;
}

} // namespace Airwave
