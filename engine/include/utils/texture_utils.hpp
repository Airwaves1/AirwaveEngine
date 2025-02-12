#pragma once

#include <memory>
#include <vector>

namespace Airwave
{
class Renderer;
class Texture;
class Shader;

class TextureUtils
{
  public:
    // 柱面投影纹理生成立方体贴图
    static std::shared_ptr<Texture> equirectangularToCubemap(Renderer *renderer, const std::shared_ptr<Texture> &equirectangular, uint32_t resolution,
                                                             bool isHDR = false);

    // 立方体贴图生成辐照度贴图
    static std::shared_ptr<Texture> irradianceConvolution(Renderer *renderer, const std::shared_ptr<Texture> &envMap, uint32_t resolution);

    // 立方体贴图生成预过滤贴图
    static std::shared_ptr<Texture> prefilterEnvMap(Renderer *renderer, const std::shared_ptr<Texture> &envMap, uint32_t resolution,
                                                    uint32_t maxMipLevels);

    // 生成BRDF积分贴图
    static std::shared_ptr<Texture> generateBRDFLUT(Renderer *renderer, uint32_t resolution = 512);
};

} // namespace Airwave
