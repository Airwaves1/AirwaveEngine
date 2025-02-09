#include "rendering/opengl/gl_texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "core/log.hpp"
#include "core/common.hpp"

namespace Airwave
{

Texture::Texture(const std::string &path, const TextureSpecification &spec) : m_spec(spec)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(m_spec.flip);

    void *data = m_spec.isHDR ? (void *)stbi_loadf(path.c_str(), &width, &height, &channels, 0)
                              : (void *)stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data == nullptr)
    {
        LOG_ERROR("Failed to load texture: {0}", path);
        return;
    }

    m_width  = width;
    m_height = height;

    if (m_spec.isHDR)
    {
        m_spec.internalFormat  = TextureInternalFormat::RGB32F;
        m_spec.format          = TextureFormat::RGB;
        m_spec.textureDataType = TextureDataType::FLOAT;
    }
    else
    {
        if (channels == 3)
        {
            m_spec.internalFormat = m_spec.sRGB ? TextureInternalFormat::SRGB : TextureInternalFormat::RGB;
            m_spec.format         = TextureFormat::RGB;
        }
        else if (channels == 1)
        {
            m_spec.internalFormat = TextureInternalFormat::RED;
            m_spec.format         = TextureFormat::RED;
        }
        else if (channels == 4)
        {
            m_spec.internalFormat = m_spec.sRGB ? TextureInternalFormat::SRGB_ALPHA : TextureInternalFormat::RGBA;
            m_spec.format         = TextureFormat::RGBA;
        }
    }

    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), m_width, m_height, 0,
                 static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.textureDataType), data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_spec.magFilter));

    if (m_spec.generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);

    if (m_spec.isHDR)
        stbi_image_free((float *)data);
    else
        stbi_image_free((uint8_t *)data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::vector<std::string> &faces, const TextureSpecification &spec) : m_spec(spec)
{
    m_spec.textureType = TextureType::TEXTURE_CUBE_MAP;

    if (m_spec.isHDR)
    {
        m_spec.internalFormat  = TextureInternalFormat::RGB16F;
        m_spec.format          = TextureFormat::RGB;
        m_spec.textureDataType = TextureDataType::FLOAT;
    }

    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);

    stbi_set_flip_vertically_on_load(m_spec.flip);

    for (size_t i = 0; i < faces.size(); i++)
    {
        int width, height, channels;
        uint8_t *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast<GLint>(m_spec.internalFormat), width,
                         height, 0, static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.textureDataType),
                         data);
            stbi_image_free(data);
        }
        else
        {
            LOG_ERROR("Failed to load cubemap face: {0}", faces[i]);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture::Texture(uint32_t width, uint32_t height, const TextureSpecification &spec, void *data)
{
    m_width  = width;
    m_height = height;
    m_spec   = spec;

    if (m_spec.isHDR)
    {
        m_spec.internalFormat  = TextureInternalFormat::RGB16F;
        m_spec.format          = TextureFormat::RGB;
        m_spec.textureDataType = TextureDataType::FLOAT;
    }

    glGenTextures(1, &m_handle);
    glBindTexture(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_handle);

    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.wrapT));
    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_spec.magFilter));

    if (spec.textureType == TextureType::TEXTURE_CUBE_MAP)
    {
        for (size_t i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast<GLint>(m_spec.internalFormat), m_width,
                         m_height, 0, static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.textureDataType),
                         data);
        }
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), m_width, m_height, 0,
                     static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.textureDataType), data);
    }

    if (m_spec.generateMipmap)
        glGenerateMipmap(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);

    glBindTexture(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    if (m_handle != 0)
    {
        glDeleteTextures(1, &m_handle);
    }
}

void Texture::bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_handle);
}

void Texture::unbind() const
{
    glBindTexture(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
}

} // namespace Airwave
