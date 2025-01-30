#pragma once

#include <unordered_map>
#include <memory>
#include "rendering/opengl/gl_texture.hpp"
#include "core/log.hpp"

#define TEXTURE_LIB Airwave::TextureLibrary::getInstance()

namespace Airwave
{
class TextureLibrary
{
  public:
    static TextureLibrary &getInstance()
    {
        static TextureLibrary instance;
        return instance;
    }

    TextureLibrary() = default;

    void add(const std::string &filePath, const std::shared_ptr<Texture> &texture);

    std::shared_ptr<Texture> load(const std::string &filePath,
                                TextureSpecification &spec, bool flip = true);

    std::shared_ptr<Texture> get(const std::string &filePath);

    void remove(const std::string &filePath);

  private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};

} // namespace Airwave
