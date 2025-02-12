#include "rendering/opengl/gl_texture.hpp"
#include "core/log.hpp"
#include "core/common.hpp"

namespace Airwave
{
Texture::Texture(TextureSpecification spec, std::vector<uint8_t> data) : m_spec(spec)
{
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

    if (spec.textureType == TextureType::TEXTURE_CUBE_MAP)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLint>(m_spec.wrapR));
    }

    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    static_cast<GLint>(m_spec.magFilter));

    void *_data = data.empty() ? nullptr : data.data();

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
        if (spec.enableMSAA)
        {
            m_spec.samples     = spec.samples;
            m_spec.textureType = TextureType::TEXTURE_2D_MULTISAMPLE;
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_spec.samples, static_cast<GLint>(m_spec.internalFormat), spec.width, spec.height,
                                    GL_FALSE);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), spec.width, spec.height, 0, static_cast<GLenum>(m_spec.format),
                         static_cast<GLenum>(m_spec.textureDataType), _data);
        }
    }

    if (m_spec.generateMipmap && !spec.enableMSAA)
    {
        glGenerateMipmap(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);
    }

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

void Texture::unbind() const { glBindTexture(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0); }

} // namespace Airwave
