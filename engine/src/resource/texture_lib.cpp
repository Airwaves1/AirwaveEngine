#include "resource/texture_lib.hpp"

namespace Airwave
{
// Implementation
void TextureLibrary::add(const std::string &filePath, const std::shared_ptr<Texture> &texture)
{
    m_textures[filePath] = texture;
}

std::shared_ptr<Texture> TextureLibrary::load(const std::string &filePath,
                                              TextureSpecification &spec, bool flip)
{
    if (m_textures.find(filePath) != m_textures.end())
    {
        return m_textures[filePath];
    }

    spec.flip = flip;

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(filePath, spec);
    if (!texture)
    {
        LOG_ERROR("Failed to load texture '{0}'", filePath);
        return nullptr;
    }

    m_textures[filePath] = texture;
    return texture;
}

std::shared_ptr<Texture> TextureLibrary::get(const std::string &filePath)
{
    if (m_textures.find(filePath) == m_textures.end())
    {
        LOG_WARN("Texture '{0}' not found", filePath);
        return nullptr;
    }

    return m_textures[filePath];
}

void TextureLibrary::remove(const std::string &filePath)
{
    auto it = m_textures.find(filePath);
    if (it != m_textures.end())
    {
        m_textures.erase(it);
        LOG_INFO("Texture '{0}' removed", filePath);
    }
    else
    {
        LOG_WARN("Texture '{0}' not found for removal", filePath);
    }
}

} // namespace Airwave
