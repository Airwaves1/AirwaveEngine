#include "texture_lib.hpp"

namespace Airwave
{
// Implementation
void TextureLibrary::add(const std::string &name, const std::shared_ptr<Texture> &texture)
{
    m_textures[name] = texture;
}

std::shared_ptr<Texture> TextureLibrary::load(const std::string &name, const std::string &filePath,
                                              const TextureSpecification &spec, bool flip)
{
    if (m_textures.find(name) != m_textures.end())
    {
        LOG_WARN("Texture '{0}' already exists", name);
        return m_textures[name];
    }

    std::shared_ptr<Texture> texture =
        std::make_shared<Texture>(filePath, TextureSpecification(), true);
    if (!texture)
    {
        LOG_ERROR("Failed to load texture '{0}'", name);
        return nullptr;
    }

    m_textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture> TextureLibrary::get(const std::string &name)
{
    if (m_textures.find(name) == m_textures.end())
    {
        LOG_WARN("Texture '{0}' not found", name);
        return nullptr;
    }

    return m_textures[name];
}

void TextureLibrary::remove(const std::string &name)
{
    auto it = m_textures.find(name);
    if (it != m_textures.end())
    {
        m_textures.erase(it);
        LOG_INFO("Texture '{0}' removed", name);
    }
    else
    {
        LOG_WARN("Texture '{0}' not found for removal", name);
    }
}

} // namespace Airwave
