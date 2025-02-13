#pragma once

#include <glad/glad.h>
#include <memory>
#include <string>
#include <vector>
#include "core/common.hpp"

namespace Airwave
{
class Texture
{
  public:
    // 空白纹理占位符
    static std::shared_ptr<Texture> create_white_texture();
    // 默认法线贴图占位符
    static std::shared_ptr<Texture> create_default_normal_map();

    // 创建空 2D 纹理或立方体贴图
    Texture(TextureSpecification spec = TextureSpecification(), void* data = nullptr);

    ~Texture();

    uint32_t getHandle() const { return m_handle; }

    const TextureSpecification &getSpec() { return m_spec; }

    uint32_t getWidth() const { return m_spec.width; }
    uint32_t getHeight() const { return m_spec.height; }

    void bind(uint32_t slot = 0) const;
    void unbind() const;

    void resize(uint32_t width, uint32_t height);

    void setRenderTarget(bool enable = true);

  private:
    uint32_t m_handle;
    TextureSpecification m_spec;
};

} // namespace Airwave
