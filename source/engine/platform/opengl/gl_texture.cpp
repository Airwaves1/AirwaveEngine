#include "gl_texture.hpp"
#include "utils/log.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace Airwave
{
Texture::Texture(uint32_t width, uint32_t height, const TextureSpecification &spec)
{
    m_spec   = spec;
    m_width  = width;
    m_height = height;

    if (m_spec.enableMSAA)
    {
        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_handle);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_spec.samples, static_cast<GLint>(m_spec.internalFormat),
                                width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else
    {
        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.wrapS));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.wrapT));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_spec.minFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_spec.magFilter));

        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), width, height, 0,
                     static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.dataType), nullptr);

        if (m_spec.generateMipmaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if (m_handle == 0)
    {
        LOG_ERROR("Failed to create texture");
    }
}

Texture::Texture(const std::string &path, const TextureSpecification &spec, bool flip)
{
    m_spec = spec;
    int width, height, channels;

    stbi_set_flip_vertically_on_load(flip);

    uint8_t *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data == nullptr)
    {
        LOG_ERROR("Failed to load texture: {0}", path);
        return;
    }

    m_width = width;

    m_height = height;

    if(channels == 3)
    {
        m_spec.format = TextureFormat::RGB;
    }
    else if(channels == 4)
    {
        m_spec.format = TextureFormat::RGBA;
    }

    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.wrapT));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_spec.magFilter));

    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), width, height, 0,
                 static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.dataType), data);

    if (m_spec.generateMipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_handle == 0)
    {
        LOG_ERROR("Failed to load texture");
    }
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
    if (m_spec.enableMSAA)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_handle);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_handle);
    }
}

void Texture::unbind() const
{
    if (m_spec.enableMSAA)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

std::vector<uint8_t> Texture::getData()
{
    std::vector<uint8_t> data(m_width * m_height * 4);
    bind();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    unbind();
    return data;
}

void Texture::setData(uint32_t width, uint32_t height, const void *data)
{
    m_width  = width;
    m_height = height;
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), width, height, 0,
                 static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.dataType), data);
    if (m_spec.generateMipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    unbind();
}

void Texture::setWrap(TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR)
{
    m_spec.wrapS = wrapS;
    m_spec.wrapT = wrapT;
    m_spec.wrapR = wrapR;
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.wrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, static_cast<GLint>(m_spec.wrapR));
    unbind();
}

} // namespace Airwave
