#pragma once
#include "function/render/gl_resource.hpp"
#include <vector>
namespace Airwave
{
enum class TextureFormat : GLenum
{
    RGB              = GL_RGB,
    RGBA             = GL_RGBA,
    DEPTH_COMPONENT  = GL_DEPTH_COMPONENT,
    STENCIL_INDEX    = GL_STENCIL_INDEX,
    DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
    RED              = GL_RED,
    RGBA16F          = GL_RGBA16F,
    RGBA32F          = GL_RGBA32F
};

enum class TextureInternalFormat
{
    RED8              = GL_R8,
    RED16             = GL_R16,
    RGB8              = GL_RGB8,
    RGBA8             = GL_RGBA8,
    DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
    STENCIL_INDEX8    = GL_STENCIL_INDEX8,
    DEPTH24_STENCIL8  = GL_DEPTH24_STENCIL8,
    RED16F            = GL_R16F,
    RGBA16F           = GL_RGBA16F,
    RGBA32F           = GL_RGBA32F
};

enum class TextureType
{
    TEXTURE_2D        = GL_TEXTURE_2D,
    TEXTURE_CUBE_MAP  = GL_TEXTURE_CUBE_MAP,
    TEXTURE_3D        = GL_TEXTURE_3D,
    TEXTURE_ARRAY     = GL_TEXTURE_2D_ARRAY,
    TEXTURE_BUFFER    = GL_TEXTURE_BUFFER,
    TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE
};

enum class TextureWrapMode
{
    REPEAT               = GL_REPEAT,
    MIRRORED_REPEAT      = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE        = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER      = GL_CLAMP_TO_BORDER,
    MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
};

enum class TextureFilterMode
{
    NEAREST                = GL_NEAREST,
    LINEAR                 = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
};

enum class TextureMinFilterMode
{
    NEAREST                = GL_NEAREST,
    LINEAR                 = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
};

enum class TextureMagFilterMode
{
    NEAREST = GL_NEAREST,
    LINEAR  = GL_LINEAR
};

enum class TextureMipMapMode
{
    NEAREST = GL_NEAREST,
    LINEAR  = GL_LINEAR
};

struct TextureConfig
{
    int32_t width{0};
    int32_t height{0};
    int32_t channels{0};
    std::vector<uint8_t> data;

    TextureFormat format{GL_RGBA};
    TextureInternalFormat internal_format{GL_RGBA8};
    TextureType type{GL_TEXTURE_2D};
    TextureWrapMode wrap_s{GL_REPEAT};
    TextureWrapMode wrap_t{GL_REPEAT};
    TextureWrapMode wrap_r{GL_REPEAT};
    TextureMinFilterMode min_filter{GL_LINEAR};
    TextureMagFilterMode mag_filter{GL_LINEAR};
    TextureMipMapMode mipmap_filter{GL_LINEAR};
    TextureMipMapMode mipmap_mode{GL_LINEAR};
    bool generate_mipmaps{false};
    bool flip_y{true};
};

class Texture : public IGLResource
{
  public:
    Texture(const std::string &resource_path, TextureConfig &textureConifg);
    ~Texture() override;

    virtual void load() override;
    virtual void reload() override;
    virtual void release() override;

    void bind(GLuint unit = 0) const;
    void unbind() const;

  private:
    TextureConfig m_textureConfig;
    mutable uint32_t m_slot{0};
};
} // namespace Airwave
