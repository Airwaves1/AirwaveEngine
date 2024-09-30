#pragma once

#include <string>
#include <vector>
#include <initializer_list>

namespace Airwave
{
enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t GetShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        case ShaderDataType::None:
            return 0;
    }
    return 0;
}

static uint32_t GetShaderDataTypeComponentCount(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4;
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        case ShaderDataType::None:
            return 0;
    }
    return 0;
}

class BufferElement
{
  public:
    BufferElement(ShaderDataType type, const std::string &name, bool normalized = false)
        : m_type(type), m_name(name), m_normalized(normalized)
    {
        m_size           = GetShaderDataTypeSize(type);
        m_componentCount = GetShaderDataTypeComponentCount(type);
    }

    void setOffset(uint32_t offset) { m_offset = offset; }
    uint32_t getOffset() const { return m_offset; }
    uint32_t getSize() const { return m_size; }
    uint32_t getComponentCount() const { return m_componentCount; }
    ShaderDataType getType() const { return m_type; }
    const std::string &getName() const { return m_name; }
    bool isNormalized() const { return m_normalized; }
    bool isInteger() const
    {
        return m_type == ShaderDataType::Int || m_type == ShaderDataType::Int2 ||
               m_type == ShaderDataType::Int3 || m_type == ShaderDataType::Int4;
    }

  private:
    ShaderDataType m_type;
    std::string m_name;
    uint32_t m_size;
    uint32_t m_offset;
    uint32_t m_componentCount;
    bool m_normalized;
};

class BufferLayout
{
  public:
    BufferLayout() {}
    BufferLayout(const std::initializer_list<BufferElement> &elements) : m_elements(elements)
    {
        calculateOffsetsAndStride();
    }

    uint32_t getStride() const { return m_stride; }
    size_t getElementCount() const { return m_elements.size(); }

    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }

  private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride = 0;

    void calculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        for (auto &element : m_elements)
        {
            element.setOffset(offset);
            offset += element.getSize();
        }
        m_stride = offset;
    }
};

class VertexBuffer
{
  public:
    VertexBuffer(uint32_t size);                  // 动态顶点缓冲区
    VertexBuffer(float *vertices, uint32_t size); // 静态顶点缓冲区
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    void setData(uint32_t pos, uint32_t size, const void *data);

    const BufferLayout &getLayout() const { return m_layout; }
    void setLayout(const BufferLayout &layout) { m_layout = layout; }

  private:
    uint32_t m_handle;
    BufferLayout m_layout;
};

class IndexBuffer
{
  public:
    IndexBuffer(uint32_t *indices, uint32_t count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    uint32_t getCount() const { return m_count; }

  private:
    uint32_t m_handle;
    uint32_t m_count;
};

} // namespace Airwave
