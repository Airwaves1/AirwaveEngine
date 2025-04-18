#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "core/log/log.hpp"

namespace Airwave
{
Texture::Texture(const std::string &resource_path, TextureConfig &textureConifg)
    : IGLResource(GL_TEXTURE_2D, resource_path), m_textureConfig(textureConifg)
{
}

Texture::~Texture() { release(); }

void Texture::load()
{
    if (!m_resource_path.empty())
    {
        stbi_set_flip_vertically_on_load(m_textureConfig.flip_y);

        uint8_t *data = stbi_load(m_resource_path.c_str(), &m_textureConfig.width, &m_textureConfig.height, &m_textureConfig.channels, 0);

        if (m_textureConfig.channels == 1)
        {
            m_textureConfig.format          = TextureFormat::RED;
            m_textureConfig.internal_format = TextureInternalFormat::RED8;
        }
        else if (m_textureConfig.channels == 3)
        {
            m_textureConfig.format          = TextureFormat::RGB;
            m_textureConfig.internal_format = TextureInternalFormat::RGB8;
        }
        else if (m_textureConfig.channels == 4)
        {
            m_textureConfig.format          = TextureFormat::RGBA;
            m_textureConfig.internal_format = TextureInternalFormat::RGBA8;
        }
        else
        {
            LOG_ERROR("Unsupported texture format: {}, channels: {}", m_resource_path, m_textureConfig.channels);
            return;
        }

        if (data)
        {
            glGenTextures(1, &m_handle);
            glBindTexture(GL_TEXTURE_2D, m_handle); // ✅ 绑定到 OpenGL

            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLenum>(m_textureConfig.internal_format), m_textureConfig.width, m_textureConfig.height, 0,
                         static_cast<GLenum>(m_textureConfig.format), GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            if (m_textureConfig.generate_mipmaps)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            stbi_image_free(data);
        }
        else
        {
            LOG_ERROR("Failed to load texture: {}", m_resource_path);
        }
    }
}

void Texture::reload() {}

void Texture::release()
{
    unbind();
    if (m_handle != 0)
    {
        glDeleteTextures(1, &m_handle);
        m_handle = 0;
    }
    m_state = ResourceState::Unloaded;
}

void Texture::bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    m_slot = GL_TEXTURE0 + unit;
    glBindTexture(static_cast<GLenum>(m_textureConfig.type), m_handle);
}

void Texture::unbind() const
{
    glActiveTexture(m_slot);
    glBindTexture(static_cast<GLenum>(m_textureConfig.type), 0);
}

} // namespace Airwave
