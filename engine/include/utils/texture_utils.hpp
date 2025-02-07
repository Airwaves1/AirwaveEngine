#pragma once

#include <memory>
#include <vector>
#include "rendering/opengl/gl_texture.hpp"
#include "rendering/renderer.hpp"

namespace Airwave
{
    class TextureUtils
    {
        public:
            static std::shared_ptr<Texture> equirectangularToCubemap(Renderer* renderer, std::shared_ptr<Texture> equirectangular, uint32_t resolution, bool isHDR = false);
    };
    
} // namespace Airwave
