#include "resource/texture_resource.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "core/log.hpp"
#include "core/common.hpp"
#include "rendering/texture.hpp"

namespace Airwave
{

TextureResource::~TextureResource() {}

bool Airwave::TextureResource::onLoad(const std::string &path, const std::any &params)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data == nullptr)
    {
        LOG_ERROR("Failed to load texture: {0}", path);
        return false;
    }

    TextureSpecification spec = std::any_cast<TextureSpecification>(params);

    spec.width  = width;
    spec.height = height;

    m_texture = std::make_shared<Texture>(spec, std::vector<uint8_t>(data, data + width * height * channels));

    

    return true;
}
} // namespace Airwave