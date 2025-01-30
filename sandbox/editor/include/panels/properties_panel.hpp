// Editor/Panels/PropertiesPanel.h
#pragma once
#include "panels/panel.hpp"
#include <glm/glm.hpp>

namespace Airwave
{
class TagComponent;
class TransformComponent;
class CameraComponent;

class PropertiesPanel : public Panel
{
  public:
    PropertiesPanel(Editor *editor, const std::string &title, bool startOpened = true)
        : Panel(editor, title)
    {
    }

    virtual ~PropertiesPanel() = default;

    void onImGuiRender() override;

  private:
    void drawTagProperty(TagComponent &tagComponent);
    void drawTransformComponent(const char *label, TransformComponent &transformComponent);
    void drawCameraComponent(CameraComponent &cameraComponent);
    static void DrawVec3Control(const std::string &label, glm::vec3 &values,
                                float resetValue = 0.0f, float columnWidth = 120.0f);
};
} // namespace Airwave
