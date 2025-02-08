#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

#include "core/common.hpp"

namespace Airwave
{
// 纹理规格
struct TextureSpecification
{
    TextureType textureType = TextureType::TEXTURE_2D;

    TextureWrap wrapS = TextureWrap::REPEAT;
    TextureWrap wrapT = TextureWrap::REPEAT;
    TextureWrap wrapR = TextureWrap::REPEAT;

    TextureFilter minFilter = TextureFilter::LINEAR;
    TextureFilter magFilter = TextureFilter::LINEAR;

    TextureInternalFormat internalFormat = TextureInternalFormat::RGBA8;
    TextureFormat format                 = TextureFormat::RGBA;
    TextureDataType textureDataType      = TextureDataType::UINT8;

    bool generateMipmap = true;
    bool flip           = true;
    bool sRGB           = false;
    bool isHDR          = false;
    uint32_t samples    = 1;
    bool enableMSAA     = false;
};

class Texture
{
  public:
    // 从文件加载 2D 纹理或 HDR 纹理
    Texture(const std::string &path, const TextureSpecification &spec = TextureSpecification());

    // 从 6 张面图加载立方体贴图
    Texture(const std::vector<std::string> &faces,
            const TextureSpecification &spec = TextureSpecification());

    // 创建空 2D 纹理或立方体贴图
    Texture(uint32_t width, uint32_t height,
            const TextureSpecification &spec = TextureSpecification(), void *data = nullptr);

    ~Texture();

    void bind(uint32_t slot = 0) const;
    void unbind() const;
    uint32_t getHandle() const { return m_handle; }
    void setHandle(uint32_t handle) { m_handle = handle; }
    TextureType getType() const { return m_spec.textureType; }

    TextureSpecification& getSpecification() { return m_spec; }

    uint32_t getWidth() const { return m_width; }
    uint32_t getHeight() const { return m_height; }

  private:
    uint32_t m_width, m_height;
    uint32_t m_handle;
    TextureSpecification m_spec;
    std::string m_path;
};

} // namespace Airwave
