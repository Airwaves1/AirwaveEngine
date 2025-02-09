#pragma once

#include "resource/resource.hpp"

#include <glad/glad.h>

#include "core/common.hpp"

namespace Airwave
{
class TextureResource : public Resource
{
  public:
    TextureResource() = default;
    ~TextureResource() override;

    ResourceType getType() const override { return ResourceType::Texture; }
    TextureSpecification &getSpec() { return m_spec; }

    // 从内存加载纹理
    bool load(TextureSpecification spec, void *data = nullptr);
    // 从文件加载纹理
    bool load(const std::string &path, TextureSpecification spec = {});
    // 从文件加载立方体贴图
    bool load(const std::vector<std::string> &paths, TextureSpecification spec = {});

    bool reload(TextureSpecification spec, void *data = nullptr);


  private:
    TextureSpecification m_spec;
};

} // namespace Airwave
