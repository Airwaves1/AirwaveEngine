#pragma once

#include "core/resource/resource.hpp"
#include <unordered_map>
#include "function/render/shader/shader.hpp"
#include "function/render/texture/texture.hpp"

namespace Airwave
{
struct MaterialConfig
{
    bool b_transparent{false};
    bool b_doubleSide{false};
};

class Material : public IResource
{
  public:
    Material(const std::string &resource_path) : IResource(resource_path) {}
    ~Material() override = default;

    std::shared_ptr<Shader> shader; // 着色器对象
};

} // namespace Airwave
