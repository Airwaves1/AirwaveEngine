#include "gl_cube_texture.hpp"
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include "utils/file_utils.hpp"
#include "utils/log.hpp"

namespace Airwave
{
CubeTexture::CubeTexture(const std::array<std::string, 6> &faces) { loadTextures(faces); }

CubeTexture::~CubeTexture() { glDeleteTextures(1, &m_handle); }

void CubeTexture::bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
}

void CubeTexture::unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

void CubeTexture::loadTextures(const std::array<std::string, 6> &faces)
{
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);

    stbi_set_flip_vertically_on_load(false);

    int width, height, channels;
    for (int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (data)
        {
            GLenum format = GL_RGB;
            if (channels == 1)
                format = GL_RED;
            else if (channels == 3)
                format = GL_RGB;
            else if (channels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, width, height, 0, format,
                         GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            stbi_image_free(data);
            LOG_ERROR("Failed to load texture '{0}'", faces[i]);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

} // namespace Airwave
