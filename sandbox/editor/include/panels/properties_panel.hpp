// Editor/Panels/PropertiesPanel.h
#pragma once
#include "panels/panel.hpp"
#include <glm/glm.hpp>

namespace Airwave
{
class TagComponent;
class TransformComponent;
class CameraComponent;
class TrackballController;
class MaterialComponent;
class LightComponent;

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
    void drawTrackballControllerComponent(TrackballController &trackballController);
    void drawMaterialComponent(MaterialComponent &materialComponent);
    void drawLightComponent(LightComponent &lightComponent);

    static void DrawColorPresets(float *currentColor);
    static void DrawColorControl(const std::string &label, glm::vec3& color);
    static void DrawVec3Control(const std::string &label, glm::vec3 &values,
                                float resetValue = 0.0f, float columnWidth = 120.0f);
};
} // namespace Airwave
