#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <variant>
#include <string>

namespace Airwave
{

const std::string ASSETS_PATH = "/assets/";
const std::string SHADER_PATH = "/assets/shaders/";
const std::string TEXTURE_PATH = "/assets/textures/";
const std::string MODEL_PATH = "/assets/models/";
const std::string FONT_PATH = "/assets/fonts/";

using UniformValue = std::variant<size_t, float, glm::vec2, glm::vec3, glm::vec4, int, glm::ivec2, glm::ivec3, glm::ivec4, glm::mat3, glm::mat4>;

enum class MaterialType
{
    None,
    Basic,
    Phong,
    PBR
};

enum class LightType
{
    Directional,
    Point,
    Spot
};

const int MAX_BONE_COUNT = 100;

struct AwVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec4 tangent;
    glm::ivec4 jointIndices; // 骨骼索引
    glm::vec4 jointWeights;  // 骨骼权重
};


struct FramebufferSpecification
{
    uint32_t width  = 0;
    uint32_t height = 0;

    bool enableMSAA = false;
    uint32_t samples = 1;

    uint32_t colorAttachmentCount = 1;
    bool enableDepth = false;
};

enum class RenderSide
{
    Front      = GL_FRONT,
    Back       = GL_BACK,
    DoubelSide = GL_FRONT_AND_BACK
};

enum class RenderMode
{
    Fill  = GL_FILL,
    Line  = GL_LINE,
    Point = GL_POINT
};

enum class CullFace
{
    Front        = GL_FRONT,
    Back         = GL_BACK,
    FrontAndBack = GL_FRONT_AND_BACK
};

enum class BlendMode
{
    None = 0,
    NormalBlend, // 正常混合
    Additive,    // 加法混合
    Multiply,    // 乘法混合
    AlphaBlend,  // Alpha混合

    // 自定义混合模式：可以直接使用源与目标的混合因子
    CustomBlend = -1 // 特殊，表示需要自定义混合函数
};

enum class DepthFunc
{
    Never        = GL_NEVER,
    Less         = GL_LESS,
    Equal        = GL_EQUAL,
    LessEqual    = GL_LEQUAL,
    Greater      = GL_GREATER,
    NotEqual     = GL_NOTEQUAL,
    GreaterEqual = GL_GEQUAL,
    Always       = GL_ALWAYS
};

enum class StencilFunc
{
    Never        = GL_NEVER,
    Less         = GL_LESS,
    Equal        = GL_EQUAL,
    LessEqual    = GL_LEQUAL,
    Greater      = GL_GREATER,
    NotEqual     = GL_NOTEQUAL,
    GreaterEqual = GL_GEQUAL,
    Always       = GL_ALWAYS
};

enum class StencilOp
{
    Keep          = GL_KEEP,
    Zero          = GL_ZERO,
    Replace       = GL_REPLACE,
    Increment     = GL_INCR,
    IncrementWrap = GL_INCR_WRAP,
    Decrement     = GL_DECR,
    DecrementWrap = GL_DECR_WRAP,
    Invert        = GL_INVERT
};

enum class StencilFace
{
    Front        = GL_FRONT,
    Back         = GL_BACK,
    FrontAndBack = GL_FRONT_AND_BACK
};

enum class BufferBit
{
    Color   = GL_COLOR_BUFFER_BIT,
    Depth   = GL_DEPTH_BUFFER_BIT,
    Stencil = GL_STENCIL_BUFFER_BIT
};
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

enum class TextureAttachment
{
    Color0       = GL_COLOR_ATTACHMENT0,
    Color1       = GL_COLOR_ATTACHMENT1,
    Color2       = GL_COLOR_ATTACHMENT2,
    Color3       = GL_COLOR_ATTACHMENT3,
    Color4       = GL_COLOR_ATTACHMENT4,
    Color5       = GL_COLOR_ATTACHMENT5,
    Color6       = GL_COLOR_ATTACHMENT6,
    Color7       = GL_COLOR_ATTACHMENT7,
    Depth        = GL_DEPTH_ATTACHMENT,
    Stencil      = GL_STENCIL_ATTACHMENT,
    DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
};

enum class FramebufferStatus
{
    Complete                    = GL_FRAMEBUFFER_COMPLETE,
    Undefined                   = GL_FRAMEBUFFER_UNDEFINED,
    IncompleteAttachment        = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
    IncompleteMissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
    IncompleteDrawBuffer        = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
    IncompleteReadBuffer        = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
    Unsupported                 = GL_FRAMEBUFFER_UNSUPPORTED,
    IncompleteMultisample       = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
    IncompleteLayerTargets      = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
};

struct MaterialRenderParams
{
    bool wireframe = false; // 是否线框模式

    bool visible = true; // 是否可见

    RenderSide side = RenderSide::Front; // 渲染面

    RenderMode renderMode = RenderMode::Fill; // 渲染模式

    BlendMode blendMode = BlendMode::NormalBlend; // 混合模式

    DepthFunc depthFunc = DepthFunc::Less; // 深度测试函数
    bool depthTest      = true;            // 是否开启深度测试
    bool depthWrite     = true;            // 是否开启深度写入

    CullFace cullFace = CullFace::Back; // 背面剔除

    bool colorWrite = true; // 是否开启颜色写入

    int stencilRef                = 0;     // 模板测试参考值
    unsigned int stencilMask      = 0xFF;  // 模板测试掩码
    unsigned int stencilWriteMask = 0xFF;  // 模板写入掩码
    bool stencilTest              = false; // 是否开启模板测试
    bool stencilWrite             = false; // 是否开启模板写入

    bool polygonOffset        = false; // 是否开启多边形偏移
    float polygonOffsetFactor = 0.0f;  // 多边形偏移因子
    float polygonOffsetUnits  = 0.0f;  // 多边形偏移单位
};

struct TextureSpecification
{
    uint32_t width  = 0;
    uint32_t height = 0;

    TextureType textureType = TextureType::TEXTURE_2D;

    TextureWrap wrapS = TextureWrap::REPEAT;
    TextureWrap wrapT = TextureWrap::REPEAT;
    TextureWrap wrapR = TextureWrap::REPEAT;

    TextureFilter minFilter = TextureFilter::LINEAR;
    TextureFilter magFilter = TextureFilter::LINEAR;

    TextureInternalFormat internalFormat = TextureInternalFormat::RGBA8;
    TextureFormat format                 = TextureFormat::RGBA;
    TextureDataType textureDataType      = TextureDataType::UINT8;

    bool generateMipmap = true;
    bool flip           = true;
    bool sRGB           = false;
    bool isHDR          = false;
    uint32_t samples    = 1;
    bool enableMSAA     = false;
};

} // namespace Airwave
