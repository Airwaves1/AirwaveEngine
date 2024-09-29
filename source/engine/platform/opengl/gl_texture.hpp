#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
namespace Airwave
{

enum class TextureType : uint32_t
{
    TEXTURE_1D                   = GL_TEXTURE_1D,
    TEXTURE_2D                   = GL_TEXTURE_2D,
    TEXTURE_3D                   = GL_TEXTURE_3D,
    TEXTURE_CUBE_MAP             = GL_TEXTURE_CUBE_MAP,
    TEXTURE_1D_ARRAY             = GL_TEXTURE_1D_ARRAY,
    TEXTURE_2D_ARRAY             = GL_TEXTURE_2D_ARRAY,
    TEXTURE_CUBE_MAP_ARRAY       = GL_TEXTURE_CUBE_MAP_ARRAY,
    TEXTURE_RECTANGLE            = GL_TEXTURE_RECTANGLE,
    TEXTURE_BUFFER               = GL_TEXTURE_BUFFER,
    TEXTURE_2D_MULTISAMPLE       = GL_TEXTURE_2D_MULTISAMPLE,
    TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
};

enum class TextureWrap : uint32_t
{
    REPEAT          = GL_REPEAT,
    MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
    CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
};

enum class TextureFormat : uint32_t
{
    RED           = GL_RED,
    RG            = GL_RG,
    RGB           = GL_RGB,
    RGBA          = GL_RGBA,
    DEPTH         = GL_DEPTH_COMPONENT,
    STENCIL       = GL_STENCIL_INDEX,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
};

enum class TextureInternalFormat : uint32_t
{
    RED               = GL_RED,
    RG                = GL_RG,
    RGB               = GL_RGB,
    RGBA              = GL_RGBA,
    SRGB              = GL_SRGB,
    SRGB_ALPHA        = GL_SRGB_ALPHA,
    R8                = GL_R8,
    RG8               = GL_RG8,
    RGB8              = GL_RGB8,
    RGBA8             = GL_RGBA8,
    R16F              = GL_R16F,
    RG16F             = GL_RG16F,
    RGB16F            = GL_RGB16F,
    RGBA16F           = GL_RGBA16F,
    R32F              = GL_R32F,
    RG32F             = GL_RG32F,
    RGB32F            = GL_RGB32F,
    RGBA32F           = GL_RGBA32F,
    DEPTH             = GL_DEPTH_COMPONENT,
    DEPTH16           = GL_DEPTH_COMPONENT16,
    DEPTH24           = GL_DEPTH_COMPONENT24,
    DEPTH32           = GL_DEPTH_COMPONENT32,
    DEPTH32F          = GL_DEPTH_COMPONENT32F,
    DEPTH24_STENCIL8  = GL_DEPTH24_STENCIL8,
    DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
};

enum class TextureDataType : uint32_t
{
    UINT8   = GL_UNSIGNED_BYTE,
    UINT16  = GL_UNSIGNED_SHORT,
    UINT32  = GL_UNSIGNED_INT,
    FLOAT   = GL_FLOAT,
    FLOAT16 = GL_HALF_FLOAT,
    FLOAT32 = GL_FLOAT,
    SHORT   = GL_SHORT,
    INT     = GL_INT,
};

enum class TextureFilter : uint32_t
{
    NEAREST                = GL_NEAREST,
    LINEAR                 = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,
};

struct TextureSpecification
{
    TextureWrap wrapS = TextureWrap::REPEAT;
    TextureWrap wrapT = TextureWrap::REPEAT;
    TextureWrap wrapR = TextureWrap::REPEAT;

    TextureFormat format = TextureFormat::RGBA;

    TextureInternalFormat internalFormat = TextureInternalFormat::RGBA8;

    TextureDataType dataType = TextureDataType::UINT8;

    TextureFilter minFilter = TextureFilter::LINEAR;
    TextureFilter magFilter = TextureFilter::LINEAR;

    bool generateMipmaps = false;

    bool enableMSAA = false;

    uint32_t samples = 1;
};

class Texture
{

  public:
    Texture(uint32_t width, uint32_t height, const TextureSpecification &spec);

    Texture(const std::string &path, const TextureSpecification &spec, bool flip = true);

    virtual ~Texture();

    void bind(uint32_t slot = 0) const;

    void unbind() const;

    std::vector<uint8_t> getData();

    void setData(uint32_t width, uint32_t height, const void *data);

    void setWrap(TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR);

    void setFilter(TextureFilter minFilter, TextureFilter magFilter) {}

    void setFormat(TextureFormat format) {}

    void setInternalFormat(TextureInternalFormat internalFormat) {}

    void setDataType(TextureDataType dataType) {}

    void setGenerateMipmaps(bool generateMipmaps) {}

    void setEnableMSAA(bool enableMSAA) {}

    void setSamples(uint32_t samples) {}

    uint32_t getHandle() const { return m_handle; }

  private:
    uint32_t m_handle = 0;

    uint32_t m_width = 0;

    uint32_t m_height = 0;

    TextureSpecification m_spec;

    bool b_isRenderTexture = false;
};

} // namespace Airwave
