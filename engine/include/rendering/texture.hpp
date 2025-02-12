#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include "core/common.hpp"

namespace Airwave
{
// 纹理规格

class Texture
{
  public:
    // 创建空 2D 纹理或立方体贴图
    Texture(TextureSpecification spec = TextureSpecification(), std::vector<uint8_t> data = std::vector<uint8_t>());

    ~Texture();

    void bind(uint32_t slot = 0) const;
    void unbind() const;
    uint32_t getHandle() const { return m_handle; }
    void setHandle(uint32_t handle) { m_handle = handle; }
    TextureType getType() const { return m_spec.textureType; }

    TextureSpecification &getSpecification() { return m_spec; }

    uint32_t getWidth() const { return m_spec.width; }
    uint32_t getHeight() const { return m_spec.height; }

  private:
    uint32_t m_handle;
    TextureSpecification m_spec;
};

} // namespace Airwave
