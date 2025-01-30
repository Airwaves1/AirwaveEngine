#include "panels/properties_panel.hpp"
#include "editor.hpp"
#include "ecs/aw_entity.hpp"
#include "ecs/components/camera_component.hpp"
#include "ecs/components/tag_component.hpp"
#include "ecs/components/transform_component.hpp"

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
        float colWidth = 180.0f;

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
        static int currentCameraTypeIndex = 0;

        // Start table with 2 columns (Label, Control)
        ImGui::BeginTable("CameraProperties", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit);

        // Set column widths
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 300.0f);  // Label column fixed width
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);      // Control column takes remaining width

        // Column 1: CameraType label
        ImGui::TableNextColumn();
        ImGui::Text("CameraType");

        // Column 2: CameraType ComboBox
        ImGui::TableNextColumn();
        if (ImGui::Combo("##CameraType", &currentCameraTypeIndex, cameraTypeItems, IM_ARRAYSIZE(cameraTypeItems)))
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



void PropertiesPanel::DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue,
                                      float columnWidth)
{
    // ImGUi push多少 要pop多少，不然会报错
    ImGui::PushID(label.c_str()); // 每一行用label做ID，3行ID不同互不干扰

    // 设置一行两列
    ImGui::Columns(2);
    // 第一列
    ImGui::SetColumnWidth(0, columnWidth); // 设置第1列宽100
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    // 第二列
    // 放入3个item的宽
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 6.5f; // 设置行高
    ImVec2 buttonSize = {lineHeight + 5.0f, lineHeight};                             // 按钮大小

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
