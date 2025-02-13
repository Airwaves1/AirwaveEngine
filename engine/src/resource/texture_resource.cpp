#include "resource/texture_resource.hpp"
#include <stb_image/stb_image.h>
#include "core/log.hpp"
#include "core/common.hpp"
#include "rendering/texture.hpp"

namespace Airwave
{

TextureResource::~TextureResource() {}

bool Airwave::TextureResource::onLoad(const std::string &path, const std::any &params)
{
    TextureSpecification spec = std::any_cast<TextureSpecification>(params);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(spec.flip);

    void *data = nullptr;
    if (spec.isHDR)
    {
        data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
    }
    else
    {
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }

    if (data == nullptr)
    {
        LOG_ERROR("Failed to load texture: {0}", path);
        return false;
    }

    spec.width  = width;
    spec.height = height;
    spec.channels = channels;

    m_texture = std::make_shared<Texture>(spec, data);

    return true;
}
} // namespace Airwave