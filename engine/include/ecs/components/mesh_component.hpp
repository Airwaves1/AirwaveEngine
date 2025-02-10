#pragma once

#include <glad/glad.h>
#include <memory>

#include "ecs/aw_component.hpp"
#include "rendering/mesh.hpp"

namespace Airwave
{
class MeshComponent : public AwComponent
{
  public:
    MeshComponent() = default;
    std::shared_ptr<Mesh> mesh;

    int currentLOD = 0;
};

} // namespace Airwave
