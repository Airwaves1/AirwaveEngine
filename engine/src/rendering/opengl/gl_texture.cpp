#include "rendering/opengl/gl_texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "core/log.hpp"
#include "core/common.hpp"

namespace Airwave
{
Texture::Texture(const std::string &path, const TextureSpecification &spec)
{

    m_spec = spec;
    int width, height, channels;
    stbi_set_flip_vertically_on_load(m_spec.flip);

    uint8_t *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data == nullptr)
    {
        LOG_ERROR("Failed to load texture: {0}", path);
        return;
    }

    m_width  = width;
    m_height = height;

    if (channels == 3)
    {
        m_spec.internalFormat =
            m_spec.sRGB ? TextureInternalFormat::SRGB : TextureInternalFormat::RGB;
        m_spec.format = TextureFormat::RGB;
    }
    else if (channels == 4)
    {
        m_spec.internalFormat = m_spec.sRGB ? static_cast<TextureInternalFormat>(GL_SRGB_ALPHA)
                                            : static_cast<TextureInternalFormat>(GL_RGBA);
        m_spec.format         = TextureFormat::RGBA;
    }

    // 开辟纹理单元
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    // 设置纹理映射
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.wrapT));

    // 设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_spec.magFilter));

    // 生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), m_width, m_height, 0,
                 static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.type), data);

    // 创建mipmap
    if (m_spec.generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);

    // 释放资源
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_handle == 0)
    {
        LOG_ERROR("Failed to create texture: {0}", path);
    }
}

Texture::Texture(uint32_t width, uint32_t height, const TextureSpecification &spec)
{
    m_width  = width;
    m_height = height;
    m_spec   = spec;

    // 开辟纹理单元
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);

    // 设置纹理映射
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(m_spec.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(m_spec.wrapT));

    // 设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(m_spec.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(m_spec.magFilter));

    // 生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(m_spec.internalFormat), m_width, m_height, 0,
                 static_cast<GLenum>(m_spec.format), static_cast<GLenum>(m_spec.type), nullptr);

    // 创建mipmap
    if (m_spec.generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_handle == 0)
    {
        LOG_ERROR("Failed to create texture");
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
    glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

} // namespace Airwave
