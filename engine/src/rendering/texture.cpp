#include "rendering/texture.hpp"
#include "core/log.hpp"
#include "core/common.hpp"

namespace Airwave
{

// std::shared_ptr<Texture> Texture::white_texture = std::make_shared<Texture>(
//     TextureSpecification{.width = 1, .height = 1, .internalFormat = TextureInternalFormat::RGBA8}, std::vector<uint8_t>{255, 255, 255, 255});

std::shared_ptr<Texture> Texture::create_white_texture()
{
    static std::shared_ptr<Texture> white_texture = std::make_shared<Texture>(
        TextureSpecification{.width = 1, .height = 1, .internalFormat = TextureInternalFormat::RGBA8}, std::vector<uint8_t>{255, 255, 255, 255}.data());

    return white_texture;
}

std::shared_ptr<Texture> Texture::create_default_normal_map()
{
    static std::shared_ptr<Texture> default_normal_map = std::make_shared<Texture>(
        TextureSpecification{.width = 1, .height = 1, .internalFormat = TextureInternalFormat::RGBA8}, std::vector<uint8_t>{128, 128, 255, 255}.data());

    return default_normal_map;
}

Texture::Texture(TextureSpecification spec, void* data) : m_spec(spec)
{
    if (m_spec.isHDR)
    {
        m_spec.internalFormat  = TextureInternalFormat::RGB32F;
        m_spec.format          = TextureFormat::RGB;
        m_spec.textureDataType = TextureDataType::FLOAT;
    }

    if(m_spec.channels == 1)
    {
        m_spec.internalFormat = TextureInternalFormat::R8;
        m_spec.format = TextureFormat::RED;
    }
    else if(m_spec.channels == 2)
    {
        m_spec.internalFormat = TextureInternalFormat::RG8;
        m_spec.format = TextureFormat::RG;
    }
    else if(m_spec.channels == 3)
    {
        m_spec.internalFormat = TextureInternalFormat::RGB8;
        m_spec.format = TextureFormat::RGB;
    }
    else if(m_spec.channels == 4)
    {
        m_spec.internalFormat = TextureInternalFormat::RGBA8;
        m_spec.format = TextureFormat::RGBA;
    }

    glGenTextures(1, &m_handle);
    glBindTexture(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_handle);

    glTexParameteri(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    static_cast<GLint>(m_spec.wrapT));

    if (m_spec.textureType == TextureType::TEXTURE_CUBE_MAP)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLint>(m_spec.wrapR));
    }

    glTexParameteri(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(m_spec.textureType == TextureType::TEXTURE_CUBE_MAP ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    static_cast<GLint>(m_spec.magFilter));


    if (m_spec.textureType == TextureType::TEXTURE_CUBE_MAP)
    {
        for (size_t i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast<GLint>(m_spec.internalFormat), m_spec.width, m_spec.width, 0,
                         static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.textureDataType), data);
        }
    }
    else
    {
        if (m_spec.enableMSAA)
        {
            m_spec.samples     = spec.samples;
            m_spec.textureType = TextureType::TEXTURE_2D_MULTISAMPLE;
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_spec.samples, static_cast<GLint>(m_spec.internalFormat), m_spec.width, m_spec.height,
                                    GL_FALSE);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), m_spec.width, m_spec.height, 0,
                         static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.textureDataType), data);
        }
    }

    if (m_spec.generateMipmap && !m_spec.enableMSAA)
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

void Texture::resize(uint32_t width, uint32_t height)
{
    if (m_spec.isRenderTarget)
    {
        m_spec.width  = width;
        m_spec.height = height;

        m_spec.textureType = TextureType::TEXTURE_2D;

        glBindTexture(GL_TEXTURE_2D, m_handle);

        if (m_spec.enableMSAA)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_spec.samples, static_cast<GLint>(m_spec.internalFormat), width, height, GL_FALSE);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), width, height, 0, static_cast<GLenum>(m_spec.format),
                         static_cast<GLenum>(m_spec.textureDataType), nullptr);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
void Texture::setRenderTarget(bool enable) { m_spec.isRenderTarget = enable; }

} // namespace Airwave
