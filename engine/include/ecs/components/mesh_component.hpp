#pragma once

#include <glad/glad.h>
#include <memory>
#include <vector>

#include "ecs/aw_component.hpp"

#include "rendering/primitive.hpp"

namespace Airwave
{
class MeshComponent : public AwComponent
{
  public:
    MeshComponent() = default;
    std::vector<std::shared_ptr<Primitive>> primitives;
    Math::AABB bounding_box;
    bool deep_frustum_culling = false; // 设为true时，会对每个primitive进行视锥体裁剪，否则直接对整个mesh进行裁剪
};

} // namespace Airwave
