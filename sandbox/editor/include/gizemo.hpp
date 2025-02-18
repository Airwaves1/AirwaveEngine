#pragma once

#include <ImGuizmo.h>

namespace Airwave
{
struct GizmoSettings
{
    ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE currentMode           = ImGuizmo::WORLD;
    bool useSnap                         = false;
    float snap[3]                        = {1.f, 1.f, 1.f};
};

class Gizemo
{
  public:
    Gizemo() {}
    ~Gizemo() {}

    void render() {
        // 控制面板
    }

    GizmoSettings settings;
};

} // namespace Airwave
