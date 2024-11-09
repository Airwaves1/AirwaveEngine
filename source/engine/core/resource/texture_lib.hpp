#pragma once

#include <unordered_map>
#include <memory>
#include "opengl/gl_texture.hpp"
#include "utils/log.hpp"

#define TEXTURE_LIB TextureLibrary::getInstance()

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

    void add(const std::string &name, const std::shared_ptr<Texture> &texture);

    std::shared_ptr<Texture> load(const std::string &name, const std::string &filePath,
                                  const TextureSpecification &spec, bool flip = true);

    std::shared_ptr<Texture> get(const std::string &name);

    void remove(const std::string &name);

  private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
};

} // namespace Airwave
