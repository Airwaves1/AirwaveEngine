#include "resource/texture_resource.hpp"

#include <stb_image/stb_image.h>

#include "core/log.hpp"

namespace Airwave
{

TextureResource::TextureResource(TextureSpecification spec, std::vector<uint8_t> data)
{
    m_spec = spec;

    if (m_spec.isHDR)
    {
        m_spec.internalFormat  = TextureInternalFormat::RGB32F;
        m_spec.format          = TextureFormat::RGB;
        m_spec.textureDataType = TextureDataType::FLOAT;
    }

    glGenTextures(1, &m_handle);
    glBindTexture(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_handle);

    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    static_cast<GLint>(m_spec.wrapT));
    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    static_cast<GLint>(m_spec.magFilter));

    void *_data = nullptr;
    if (data.size() > 0)
    {
        _data = data.data();
    }

    if (spec.textureType == TextureType::TEXTURE_CUBE_MAP)
    {
        for (size_t i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast<GLint>(m_spec.internalFormat), spec.width, spec.width, 0,
                         static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.textureDataType), _data);
        }
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), spec.width, spec.height, 0, static_cast<GLenum>(m_spec.format),
                     static_cast<GLenum>(m_spec.textureDataType), _data);
    }

    if (m_spec.generateMipmap) glGenerateMipmap(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);

    glBindTexture(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);
}

TextureResource::~TextureResource()
{
    if (m_handle != 0)
    {
        glDeleteTextures(1, &m_handle);
    }
}

bool TextureResource::load(const std::string &path, TextureSpecification spec)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(spec.flip);

    void *data =
        spec.isHDR ? (void *)stbi_loadf(path.c_str(), &width, &height, &channels, 0) : (void *)stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data == nullptr)
    {
        LOG_ERROR("Failed to load texture: {0}", path);
        return false;
    }

    m_spec        = spec;
    m_spec.width  = width;
    m_spec.height = height;

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

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), m_spec.width, m_spec.height, 0, static_cast<GLenum>(m_spec.format),
                 static_cast<GLenum>(m_spec.textureDataType), data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_spec.magFilter));

    if (m_spec.generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return true;
}

bool TextureResource::load(const std::vector<std::string> &paths, TextureSpecification spec)
{
    m_spec      = spec;
    m_spec.flip = false;

    if (m_spec.isHDR)
    {
        m_spec.internalFormat  = TextureInternalFormat::RGB16F;
        m_spec.format          = TextureFormat::RGB;
        m_spec.textureDataType = TextureDataType::FLOAT;
    }

    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);

    stbi_set_flip_vertically_on_load(m_spec.flip);

    for (size_t i = 0; i < 6; i++)
    {
        int width, height, channels;
        void *data = m_spec.isHDR ? (void *)stbi_loadf(paths[i].c_str(), &width, &height, &channels, 0)
                                  : (void *)stbi_load(paths[i].c_str(), &width, &height, &channels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast<GLint>(m_spec.internalFormat), width, height, 0,
                         static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.textureDataType), data);
            stbi_image_free(data);
        }
        else
        {
            LOG_ERROR("Failed to load cubemap face: {0}", paths[i]);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return true;
}

bool TextureResource::reload(TextureSpecification spec, void *data) { return false; }

} // namespace Airwave