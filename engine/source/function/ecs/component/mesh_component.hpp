#pragma once

#include <glad/glad.h>
#include "function/render/shader/shader.hpp"
#include "function/render/texture/texture.hpp"
#include "function/global/global_context.hpp"
#include "function/render/mesh/mesh.hpp"

namespace Airwave
{
struct MeshComponent
{
    constexpr static const char *getComponentName() { return "MeshComponent"; }

    MeshComponent() = default;

    bool b_visible{true};
    bool b_fromModel{false};
    std::shared_ptr<Mesh> mesh;
};

} // namespace Airwave
