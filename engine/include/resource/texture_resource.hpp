#pragma once

#include "rendering/"
#include "resource/resource.hpp"

#include <glad/glad.h>

#include "core/common.hpp"

namespace Airwave
{
class TextureResource : public Resource
{
  public:
    TextureResource() = default;
    TextureResource(TextureSpecification spec, std::vector<uint8_t> data = {});
    ~TextureResource() override;

    ResourceType getType() const override { return ResourceType::Texture; }
    TextureSpecification &getSpec() { return m_spec; }

    // 从文件加载立方体贴图
    // bool load(const std::vector<std::string> &paths, TextureSpecification spec);

    bool reload(TextureSpecification spec, void *data = nullptr);

  protected:
    bool onLoad(const std::string &path, const std::any &params) override;

  private:
    std::shared_ptr<Texture> m_texture;
};

} // namespace Airwave
