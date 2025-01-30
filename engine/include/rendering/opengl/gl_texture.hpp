#pragma once

#include <glad/glad.h>
#include <string>

#include "core/common.hpp"

namespace Airwave
{
// 纹理规格
struct TextureSpecification
{
    // 纹理映射
    TextureWrap wrapS = TextureWrap::REPEAT;
    TextureWrap wrapT = TextureWrap::REPEAT;

    // 纹理过滤
    TextureFilter minFilter = TextureFilter::LINEAR; // 缩小的过滤器
    TextureFilter magFilter = TextureFilter::LINEAR; // 放大的过滤器

    // 纹理格式
    TextureInternalFormat internalFormat = TextureInternalFormat::RGBA8; // 纹理内部格式
    TextureFormat format                 = TextureFormat::RGBA;          // 纹理格式

    // 纹理类型
    TextureDataType type= TextureDataType::UINT8;

    // 是否生成mipmap
    bool generateMipmap = true;

    // 多重采样
    uint32_t samples = 1;

    // 是否翻转
    bool flip = true;

    // 是否使用sRGB
    bool sRGB = false;

    bool enableMSAA = false;
};

class Texture
{
  public:
    Texture(const std::string &path, const TextureSpecification &spec = TextureSpecification());
    Texture(uint32_t width, uint32_t height, const TextureSpecification &spec = TextureSpecification());
    ~Texture();

    void bind(uint32_t slot = 0) const;
    void unbind() const;

    uint32_t getHandle() const { return m_handle; }

  private:
    uint32_t m_width, m_height;
    uint32_t m_handle;
    TextureSpecification m_spec;
    std::string m_path;
};
} // namespace Airwave
