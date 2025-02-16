#pragma once

#include "resource/resource_manager.hpp"
#include "resource/resource.hpp"
#include "rendering/texture.hpp"
namespace Airwave
{
class Texture;
class TextureResource : public Resource
{
  public:
    TextureResource() = default;
    TextureResource(std::shared_ptr<Texture> texture) : m_texture(texture) {}
    ~TextureResource() override;

    std::shared_ptr<Texture> getTexture() const
    {
        if (m_texture == nullptr)
        {
            return RES.get<TextureResource>("empty_map")->getTexture();
        }
        return m_texture;
    }

    void setTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

  protected:
    bool onLoad(const std::string &path, const std::any &params) override;

  private:
    std::shared_ptr<Texture> m_texture = nullptr;
};

} // namespace Airwave
