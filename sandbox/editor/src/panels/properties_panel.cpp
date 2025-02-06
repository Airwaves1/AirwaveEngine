#include "panels/properties_panel.hpp"
#include "editor.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/components/camera_component.hpp"
#include "ecs/components/tag_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/material_component.hpp"
#include "ecs/components/light_component.hpp"

#include "controller/trackball_controller_component.hpp"

namespace Airwave
{
void PropertiesPanel::onImGuiRender()
{
    ImGui::Begin(m_title.c_str());
    auto currentEntity = m_editor->getSelectedEntity();
    if (!currentEntity)
    {
        TagComponent tagComponent;
        drawTagProperty(tagComponent);
        TransformComponent transformComponent;
        drawTransformComponent("transform", transformComponent);
    }
    else
    {
        if (currentEntity->hasComponent<TagComponent>())
        {
            auto &tagComponent = currentEntity->getComponent<TagComponent>();
            drawTagProperty(tagComponent);
        }

        if (currentEntity->hasComponent<TransformComponent>())
        {
            auto &transformComponent = currentEntity->getComponent<TransformComponent>();
            drawTransformComponent("transform", transformComponent);
        }

        if (currentEntity->hasComponent<CameraComponent>())
        {
            auto &cameraComponent = currentEntity->getComponent<CameraComponent>();
            drawCameraComponent(cameraComponent);
        }

        if (currentEntity->hasComponent<TrackballController>())
        {
            auto &trackballController = currentEntity->getComponent<TrackballController>();
            drawTrackballControllerComponent(trackballController);
        }

        if (currentEntity->hasComponent<MaterialComponent>())
        {
            auto &materialComponent = currentEntity->getComponent<MaterialComponent>();
            drawMaterialComponent(materialComponent);
        }

        if (currentEntity->hasComponent<LightComponent>())
        {
            auto &lightComponent = currentEntity->getComponent<LightComponent>();
            drawLightComponent(lightComponent);
        }
    }
    ImGui::End();
}

void PropertiesPanel::drawTagProperty(TagComponent &tagComponent)
{
    ImGui::Text("Entity");

    // ImGui::SameLine();
    // std::string uuid = tagComponent.uuid.toString();
    // ImGui::Text(uuid.c_str());

    char nameBuffer[256];
    memset(nameBuffer, 0, sizeof(nameBuffer));
    strncpy_s(nameBuffer, sizeof(nameBuffer), tagComponent.name.c_str(), _TRUNCATE);

    if (ImGui::InputText("##name", nameBuffer, sizeof(nameBuffer)))
    {
        tagComponent.name = std::string(nameBuffer);
    }

    // ImGui::Separator(); // 分隔线
    ImGui::Spacing();

    // Tag 输入框 + 下拉框 + 添加按钮
    ImGui::Text("Tag");
    ImGui::SameLine();

    static std::vector<std::string> tagOptions = {"Untagged", "Player", "Enemy", "Interactable",
                                                  "Environment"};

    // 确保 currentTag 不是空的，否则默认选中第一个
    static std::string currentTag = "";
    currentTag                    = tagComponent.tag.empty() ? tagOptions[0] : tagComponent.tag;

    char tagBuffer[256];
    memset(tagBuffer, 0, sizeof(tagBuffer));
    strncpy_s(tagBuffer, sizeof(tagBuffer), currentTag.c_str(), _TRUNCATE);

    // Tag 输入框
    ImGui::PushItemWidth(150);
    if (ImGui::InputText("##tag", tagBuffer, sizeof(tagBuffer)))
    {
        currentTag       = std::string(tagBuffer);
        tagComponent.tag = currentTag;
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    // 下拉框
    ImGui::PushItemWidth(35); // 增大下拉框宽度
    if (ImGui::BeginCombo("##TagSelect", ""))
    {
        for (const auto &tag : tagOptions)
        {
            bool isSelected = (currentTag == tag);
            if (ImGui::Selectable(tag.c_str(), isSelected))
            {
                currentTag       = tag;
                tagComponent.tag = currentTag;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();

    // 添加按钮
    if (ImGui::Button(" + "))
    {
        ImGui::OpenPopup("AddTagPopup"); // 只负责打开弹窗
    }

    // 弹窗逻辑
    if (ImGui::BeginPopupModal("AddTagPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char newTagBuffer[256] = "";

        // **关键优化**: 当弹窗打开时重置 `newTagBuffer`
        if (ImGui::IsWindowAppearing())
        {
            memset(newTagBuffer, 0, sizeof(newTagBuffer));
        }

        ImGui::Text("Enter new tag:");
        ImGui::InputText("##newTag", newTagBuffer, sizeof(newTagBuffer));

        if (ImGui::Button("Add"))
        {
            std::string newTag = std::string(newTagBuffer);
            if (!newTag.empty() &&
                std::find(tagOptions.begin(), tagOptions.end(), newTag) == tagOptions.end())
            {
                tagOptions.push_back(newTag);
                currentTag       = newTag; // 直接选中新 Tag
                tagComponent.tag = newTag;
            }
            ImGui::CloseCurrentPopup(); // 关闭弹窗
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup(); // 关闭弹窗
        }

        ImGui::EndPopup();
    }
}

void PropertiesPanel::drawTransformComponent(const char *label,
                                             TransformComponent &transformComponent)
{
    // 分界线
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::TreeNodeEx((void *)typeid(TransformComponent).hash_code(),
                          ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
    {
        float colWidth = 150.0f;

        auto position = transformComponent.getPosition();
        DrawVec3Control("Position", position, 0.0, colWidth);
        transformComponent.setPosition(position);

        auto rotation      = transformComponent.getRotation();
        auto rotationEuler = glm::degrees(glm::eulerAngles(rotation));
        DrawVec3Control("Rotation", rotationEuler, 0.0, colWidth);
        transformComponent.setRotation(glm::quat(glm::radians(rotationEuler)));

        auto scale = transformComponent.getScale();
        DrawVec3Control("Scale", scale, 1.0f, colWidth);
        transformComponent.setScale(scale);

        // 展开树节点
        ImGui::TreePop();
    }
}

void PropertiesPanel::drawCameraComponent(CameraComponent &cameraComponent)
{
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::TreeNodeEx((void *)typeid(CameraComponent).hash_code(),
                          ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
    {
        static const char *cameraTypeItems[] = {"Perspective", "Orthographic"};
        static int currentCameraTypeIndex    = 0;

        // Start table with 2 columns (Label, Control)
        ImGui::BeginTable("CameraProperties", 2,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);

        // Set column widths
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 300.0f); // Label column
                                                                                    // fixed width
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch); // Control column
                                                                                // takes remaining
                                                                                // width

        // Column 1: CameraType label
        ImGui::TableNextColumn();
        ImGui::Text("CameraType");

        // Column 2: CameraType ComboBox
        ImGui::TableNextColumn();
        if (ImGui::Combo("##CameraType", &currentCameraTypeIndex, cameraTypeItems,
                         IM_ARRAYSIZE(cameraTypeItems)))
        {
            if (currentCameraTypeIndex == 0)
            {
                cameraComponent.setCameraType(CameraComponent::CameraType::Perspective);
            }
            else
            {
                cameraComponent.setCameraType(CameraComponent::CameraType::Orthographic);
            }
        }

        // clip
        // Near clip label
        ImGui::TableNextColumn();
        ImGui::Text("Near Clip");

        // Near clip control
        ImGui::TableNextColumn();
        float nearClip = cameraComponent.getNearPlane();

        if (ImGui::DragFloat("##NearClip", &nearClip, 0.1f, 0.0f, 0.0f, "%.2f"))
        {
            cameraComponent.setNearPlane(nearClip);
        }

        // Far clip label
        ImGui::TableNextColumn();
        ImGui::Text("Far Clip");

        // Far clip control
        ImGui::TableNextColumn();
        float farClip = cameraComponent.getFarPlane();
        if (ImGui::DragFloat("##FarClip", &farClip, 0.1f, 0.0f, 0.0f, "%.2f"))
        {
            cameraComponent.setFarPlane(farClip);
        }

        // fov for Perspective Camera
        if (cameraComponent.getCameraType() == CameraComponent::CameraType::Perspective)
        {
            // FOV label
            ImGui::TableNextColumn();
            ImGui::Text("Fov");

            // FOV control
            ImGui::TableNextColumn();
            float fov = cameraComponent.getFOV();
            if (ImGui::DragFloat("##Fov", &fov, 0.1f, 0.0f, 0.0f, "%.2f"))
            {
                cameraComponent.setFOV(fov);
            }
        }

        // End the table
        ImGui::EndTable();

        ImGui::TreePop();
    }
}

void PropertiesPanel::drawTrackballControllerComponent(TrackballController &trackballController)
{
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::TreeNodeEx((void *)typeid(TrackballController).hash_code(),
                          ImGuiTreeNodeFlags_DefaultOpen, "Trackball Controller"))
    {
        // Start table with 2 columns (Label, Control)
        ImGui::BeginTable("TrackballProperties", 2,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);

        // Set column widths
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 300.0f); // Label column
                                                                                    // fixed width
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch); // Control column
                                                                                // takes remaining
                                                                                // width

        // Column 1: UseTrackball label
        ImGui::TableNextColumn();
        ImGui::Text("Use Trackball");

        // Column 2: UseTrackball CheckBox
        ImGui::TableNextColumn();
        bool useTrackball = trackballController.b_useTrackball;
        if (ImGui::Checkbox("##UseTrackball", &useTrackball))
        {
            trackballController.b_useTrackball = useTrackball;
        }

        // Column 1: RotationSpeed label
        ImGui::TableNextColumn();
        ImGui::Text("Rotation Speed");

        // Column 2: RotationSpeed DragFloat
        ImGui::TableNextColumn();
        float rotationSpeed = trackballController.rotationSpeed;
        if (ImGui::DragFloat("##RotationSpeed", &rotationSpeed, 0.1f, 0.0f, 0.0f, "%.2f"))
        {
            trackballController.rotationSpeed = rotationSpeed;
        }

        // Column 1: ZoomSpeed label
        ImGui::TableNextColumn();
        ImGui::Text("Zoom Speed");

        // Column 2: ZoomSpeed DragFloat
        ImGui::TableNextColumn();
        float zoomSpeed = trackballController.zoomSpeed;
        if (ImGui::DragFloat("##ZoomSpeed", &zoomSpeed, 0.1f, 0.0f, 0.0f, "%.2f"))
        {
            trackballController.zoomSpeed = zoomSpeed;
        }

        // Column 1: PanSpeed label
        ImGui::TableNextColumn();
        ImGui::Text("Pan Speed");

        // Column 2: PanSpeed DragFloat
        ImGui::TableNextColumn();
        float panSpeed = trackballController.panSpeed;
        if (ImGui::DragFloat("##PanSpeed", &panSpeed, 0.1f, 0.0f, 0.0f, "%.2f"))
        {
            trackballController.panSpeed = panSpeed;
        }

        // End the table
        ImGui::EndTable();

        ImGui::TreePop();
    }
}

void PropertiesPanel::drawMaterialComponent(MaterialComponent &materialComponent)
{
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::TreeNodeEx((void *)typeid(MaterialComponent).hash_code(),
                          ImGuiTreeNodeFlags_DefaultOpen, "Material"))
    {
        // Start table with 2 columns (Label, Control)
        ImGui::BeginTable("MaterialProperties", 2,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);

        // Set column widths
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 300.0f); // Label column
                                                                                    // fixed width
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch); // Control column
                                                                                // takes remaining
                                                                                // width

        // Column 1: MaterialType label
        ImGui::TableNextColumn();
        ImGui::Text("Material Type");

        // Column 2: MaterialType ComboBox
        ImGui::TableNextColumn();
        static const char *materialTypeItems[] = {"Basic", "Phong", "PBR"};
        static int currentMaterialTypeIndex    = 0;

        MaterialType currentMaterialType = materialComponent.getMaterialType();
        if (currentMaterialType == MaterialType::Basic)
        {
            currentMaterialTypeIndex = 0;
        }
        else if (currentMaterialType == MaterialType::Phong)
        {
            currentMaterialTypeIndex = 1;
        }
        else if (currentMaterialType == MaterialType::PBR)
        {
            currentMaterialTypeIndex = 2;
        }

        if (ImGui::Combo("##MaterialType", &currentMaterialTypeIndex, materialTypeItems,
                         IM_ARRAYSIZE(materialTypeItems)))
        {
            if (currentMaterialTypeIndex == 0)
            {
                materialComponent.setMaterialType(MaterialType::Basic);
            }
            else if (currentMaterialTypeIndex == 1)
            {
                materialComponent.setMaterialType(MaterialType::Phong);
            }
            else if (currentMaterialTypeIndex == 2)
            {
                materialComponent.setMaterialType(MaterialType::PBR);
            }
        }

        // Column 1: visible label
        ImGui::TableNextColumn();
        ImGui::Text("Visible");

        // Column 2: visible CheckBox
        ImGui::TableNextColumn();
        bool visible = materialComponent.materialRenderParams.visible;
        if (ImGui::Checkbox("##Visible", &visible))
        {
            materialComponent.materialRenderParams.visible = visible;
        }

        DrawColorControl("Color", materialComponent.color);
        switch (materialComponent.getMaterialType())
        {
            case MaterialType::Basic:
            {

                break;
            }
            case MaterialType::PBR:
            {
                // Column 1: Roughness label
                ImGui::TableNextColumn();
                ImGui::Text("Roughness");

                // Column 2: Roughness DragFloat (range: 0.0 - 1.0)
                ImGui::TableNextColumn();
                float roughness = materialComponent.roughness;
                if (ImGui::DragFloat("##Roughness", &roughness, 0.01f, 0.0f, 1.0f, "%.2f"))
                {
                    materialComponent.roughness = glm::clamp(roughness, 0.0f, 1.0f);
                }

                // Column 1: Metallic label
                ImGui::TableNextColumn();
                ImGui::Text("Metallic");

                // Column 2: Metallic DragFloat (range: 0.0 - 1.0)
                ImGui::TableNextColumn();
                float metallic = materialComponent.metallic;
                if (ImGui::DragFloat("##Metallic", &metallic, 0.01f, 0.0f, 1.0f, "%.2f"))
                {
                    materialComponent.metallic = glm::clamp(metallic, 0.0f, 1.0f);
                }

                // Column 1: AO label
                ImGui::TableNextColumn();
                ImGui::Text("AO");

                // Column 2: AO DragFloat (range: 0.0 - 1.0)
                ImGui::TableNextColumn();
                float ao = materialComponent.ao;
                if (ImGui::DragFloat("##AO", &ao, 0.01f, 0.0f, 1.0f, "%.2f"))
                {
                    materialComponent.ao = glm::clamp(ao, 0.0f, 1.0f);
                }

                break;
            }

            case MaterialType::None:
            {
                break;
            }

            default:
            {
                break;
            }
        }

        // End the table
        ImGui::EndTable();

        ImGui::TreePop();
    }
}
void PropertiesPanel::drawLightComponent(LightComponent &lightComponent)
{
    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::TreeNodeEx((void *)typeid(LightComponent).hash_code(),
                          ImGuiTreeNodeFlags_DefaultOpen, "Light"))
    {
        // Start table with 2 columns (Label, Control)
        ImGui::BeginTable("LightProperties", 2,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);

        // Set column widths
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 300.0f); // Label column
                                                                                    // fixed width
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch); // Control column
                                                                                // takes remaining
                                                                                // width

        // Column 1: LightType label
        ImGui::TableNextColumn();
        ImGui::Text("Light Type");

        // Column 2: LightType ComboBox
        ImGui::TableNextColumn();
        static const char *lightTypeItems[] = {"Directional", "Point", "Spot"};
        static int currentLightTypeIndex    = 0;

        LightType currentLightType = lightComponent.type;
        if (currentLightType == LightType::Directional)
        {
            currentLightTypeIndex = 0;
        }
        else if (currentLightType == LightType::Point)
        {
            currentLightTypeIndex = 1;
        }
        else if (currentLightType == LightType::Spot)
        {
            currentLightTypeIndex = 2;
        }

        if (ImGui::Combo("##LightType", &currentLightTypeIndex, lightTypeItems,
                         IM_ARRAYSIZE(lightTypeItems)))
        {
            if (currentLightTypeIndex == 0)
            {
                lightComponent.type = LightType::Directional;
            }
            else if (currentLightTypeIndex == 1)
            {
                lightComponent.type = LightType::Point;
            }
            else if (currentLightTypeIndex == 2)
            {
                lightComponent.type = LightType::Spot;
            }
        }

        // Column 1: Intensity label
        ImGui::TableNextColumn();
        ImGui::Text("Intensity");

        // Column 2: Intensity DragFloat
        ImGui::TableNextColumn();
        float intensity = lightComponent.intensity;

        if (ImGui::DragFloat("##Intensity", &intensity, 0.1f, 0.0f, 0.0f, "%.2f"))
        {
            lightComponent.intensity = intensity;
        }

        DrawColorControl("Color", lightComponent.color);

        // 结束表格
        ImGui::EndTable();

        ImGui::TreePop();
    }
}

void PropertiesPanel::DrawColorPresets(float *currentColor)
{
    const ImVec2 buttonSize(20, 20);
    const ImVec4 presets[] = {
        ImVec4(1, 0, 0, 1),         // Red
        ImVec4(0, 1, 0, 1),         // Green
        ImVec4(0, 0, 1, 1),         // Blue
        ImVec4(1, 1, 0, 1),         // Yellow
        ImVec4(1, 0, 1, 1),         // Magenta
        ImVec4(0, 1, 1, 1),         // Cyan
        ImVec4(1, 1, 1, 1),         // White
        ImVec4(0.5f, 0.5f, 0.5f, 1) // Gray
    };

    for (int i = 0; i < IM_ARRAYSIZE(presets); i++)
    {
        ImGui::PushID(i);
        if (ImGui::ColorButton("##Preset", presets[i], ImGuiColorEditFlags_NoTooltip, buttonSize))
        {
            currentColor[0] = presets[i].x;
            currentColor[1] = presets[i].y;
            currentColor[2] = presets[i].z;
        }
        if ((i % 8) != 7) ImGui::SameLine();
        ImGui::PopID();
    }
}

void PropertiesPanel::DrawColorControl(const std::string &label, glm::vec3 &color)
{
    // Column 1: Color label
    ImGui::TableNextColumn();
    ImGui::Text("Color");

    // Column 2: Color control
    ImGui::TableNextColumn();
    {
        // glm::vec3 &color    = materialComponent.color;
        // float colorArray[3] = {color.r, color.g, color.b};
        // float colorArray[3] = {color[0], color[1], color[2]};
        float colorArray[3] = {color.x, color.y, color.z};

        // 颜色预览按钮
        ImGui::PushID("##ColorPreview");
        ImGui::ColorButton("##ColorPreview",
                           ImVec4(colorArray[0], colorArray[1], colorArray[2], 1.0f),
                           ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop,
                           ImVec2(ImGui::GetFrameHeight() * 1.5f, ImGui::GetFrameHeight()));

        // 弹出颜色选择器
        if (ImGui::BeginPopupContextItem("ColorPickerContext", ImGuiPopupFlags_MouseButtonLeft))
        {
            ImGui::Text("Color Picker");
            ImGui::Separator();

            // 使用更专业的颜色选择器布局
            ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayRGB |
                                        ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB |
                                        ImGuiColorEditFlags_PickerHueWheel;

            if (ImGui::ColorPicker3("##ColorPicker", colorArray, flags))
            {
                color.x = colorArray[0];
                color.y = colorArray[1];
                color.z = colorArray[2];
            }

            // 快速预设颜色
            ImGui::Separator();
            ImGui::Text("Presets");
            DrawColorPresets(colorArray);

            ImGui::EndPopup();
        }
        ImGui::PopID();

        // 右侧显示数值编辑框
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::ColorEdit3("##ColorEdit", colorArray,
                              ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs |
                                  ImGuiColorEditFlags_NoTooltip))
        {
            color.r = colorArray[0];
            color.g = colorArray[1];
            color.b = colorArray[2];
        }
    }
}
void PropertiesPanel::DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue,
                                      float columnWidth)
{
    // ImGUi push多少 要pop多少，不然会报错
    ImGui::PushID(label.c_str()); // 每一行用label做ID，3行ID不同互不干扰

    // 设置一行两列
    ImGui::Columns(2);
    // 第一列
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    // 第二列
    // 放入3个item的宽
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 4.5f; // 设置行高
    ImVec2 buttonSize = {lineHeight, lineHeight};                             // 按钮大小

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                        ImVec2{(buttonSize.x - ImGui::CalcTextSize("X").x) * 0.5f, 4.0f});
    if (ImGui::Button("X", buttonSize))
    {
        values.x = resetValue;
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(3);

    // 因为DragFloat button会换行，所以设置SameLine将不换行
    ImGui::SameLine();
    // ##X将分配一个id，且##x不会在UI界面显示出来。
    // #X 将显示在文本框的右边
    // X  将与上面的BUtton同名 同id，操作的话会报错
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    if (ImGui::Button("Y", buttonSize))
    {
        values.y = resetValue;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"); // 0.1速度，0 - 0
                                                                  // 最小最大无限制
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    if (ImGui::Button("Z", buttonSize))
    {
        values.z = resetValue;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    // 恢复成一行一列
    ImGui::Columns(1);

    ImGui::PopID();
}

} // namespace Airwave
