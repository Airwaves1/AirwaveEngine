#pragma once

#include <memory>
#include <vector>
#include "rendering/opengl/gl_texture.hpp"
#include "rendering/renderer.hpp"

namespace Airwave
{
class TextureResource;
class ShaderResource;

class TextureUtils
{
  public:
    // 柱面投影纹理生成立方体贴图
    static std::shared_ptr<TextureResource> equirectangularToCubemap(Renderer *renderer, const std::shared_ptr<TextureResource> &equirectangular,
                                                                     uint32_t resolution, bool isHDR = false);

    // 立方体贴图生成辐照度贴图
    static std::shared_ptr<TextureResource> irradianceConvolution(Renderer *renderer, const std::shared_ptr<TextureResource> &envMap,
                                                                  uint32_t resolution);

    // 立方体贴图生成预过滤贴图
    static std::shared_ptr<TextureResource> prefilterEnvMap(Renderer *renderer, const std::shared_ptr<TextureResource> &envMap, uint32_t resolution,
                                                            uint32_t maxMipLevels);

    // 生成BRDF积分贴图
    static std::shared_ptr<TextureResource> generateBRDFLUT(Renderer *renderer, uint32_t resolution = 512);
};

} // namespace Airwave
