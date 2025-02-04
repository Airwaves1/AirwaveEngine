#pragma once

#include <filesystem>
#include <string>
#include "panels/panel.hpp"
#include <imgui.h>

namespace Airwave
{

class ContentBrowserPanel : public Panel
{
  public:
    ContentBrowserPanel(Editor *editor, const std::string &title, const std::string &baseDir,
                        bool startOpened = true)
        : Panel(editor, title, startOpened), m_baseDirectory(baseDir), m_currentDirectory(baseDir)
    {
        // 图标尺寸配置
        m_thumbnailSize = 128.0f;
        m_padding       = 32.0f;

        // 初始化图标映射
        m_iconMap = {{"", "📁"}, // 目录
                     {".png", "🖼️"}, {".jpg", "🖼️"},   {".jpeg", "🖼️"},  {".wav", "🎵"},
                     {".mp3", "🎵"}, {".ogg", "🎵"},   {".txt", "📄"},   {".md", "📄"},
                     {".cpp", "📝"}, {".h", "📝"},     {".hpp", "📝"},   {".fbx", "🎲"},
                     {".obj", "🎲"}, {".scene", "🌐"}, {"default", "❓"}};

        m_iconSize = 128.0f;
        // 加载内置图标（需要提前加载纹理到m_FolderIcon/m_FileIcon）
        // 这里需要你实现实际的纹理加载逻辑
        // m_folderIcon = LoadTexture("resources/folder.png");
        // m_fileIcon = LoadTexture("resources/file.png");
    }

    void onImGuiRender() override
    {
        ImGui::Begin(m_title.c_str());

        // 顶部控制栏
        DrawTopBar();

        // 主体内容区域
        DrawContentArea();

        ImGui::End();
    }

  private:
    void DrawTopBar()
    {
        // 返回按钮和路径导航
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));

        // 返回按钮
        if (ImGui::Button("<-") && m_currentDirectory != m_baseDirectory)
        {
            m_currentDirectory = m_currentDirectory.parent_path();
        }

        // 面包屑导航
        ImGui::SameLine();
        DrawBreadcrumbs();

        ImGui::PopStyleVar();
    }

    void DrawBreadcrumbs()
    {
        std::filesystem::path current  = m_currentDirectory;
        std::filesystem::path relative = std::filesystem::relative(current, m_baseDirectory);

        // 根目录
        if (ImGui::Button(m_baseDirectory.filename().string().c_str()))
        {
            m_currentDirectory = m_baseDirectory;
        }

        // 子路径
        for (const auto &part : relative)
        {
            ImGui::SameLine();
            ImGui::Text("/");
            ImGui::SameLine();

            if (ImGui::Button(part.string().c_str()))
            {
                m_currentDirectory = m_baseDirectory / part;
            }
        }
    }

    void DrawContentArea()
    {
        const float cellSize   = m_thumbnailSize + m_padding;
        const float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount        = static_cast<int>(panelWidth / cellSize);
        columnCount            = columnCount < 1 ? 1 : columnCount;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(m_padding, m_padding));

        if (ImGui::BeginTable("##ContentTable", columnCount))
        {
            for (auto &entry : std::filesystem::directory_iterator(m_currentDirectory))
            {
                ImGui::TableNextColumn();
                DrawItem(entry);
            }
            ImGui::EndTable();
        }

        ImGui::PopStyleVar();
    }

    void DrawItem(const std::filesystem::directory_entry &entry)
    {
        const auto &path           = entry.path();
        const bool isDirectory     = entry.is_directory();
        const std::string filename = path.filename().string();

        // 选择图标
        // const char *icon = isDirectory ? m_iconMap[""].c_str()
        //                                : (m_iconMap.count(path.extension().string()) > 0
        //                                       ? m_iconMap[path.extension().string()].c_str()
        //                                       : m_iconMap["default"].c_str());
        const char *icon = "📁";

        // ImTextureID icon     = isDirectory ? m_folderIcon : m_fileIcon;
        const float iconSize = m_iconSize;

        // 图标按钮
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        if (ImGui::Button(icon, ImVec2(iconSize, iconSize)))
        {
            if (isDirectory)
            {
                m_currentDirectory = path;
            }
            else
            {
                // 处理文件打开
                // handleFileOpen(path);
            }
        }

        // 双击打开
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (isDirectory)
            {
                m_currentDirectory = path;
            }
            else
            {
                // 处理文件打开
            }
        }

        // 右键菜单
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("打开"))
            { /* 打开操作 */
            }
            if (ImGui::MenuItem("删除"))
            { /* 删除操作 */
            }
            if (ImGui::MenuItem("重命名"))
            { /* 重命名操作 */
            }
            ImGui::EndPopup();
        }

        // 文件名
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + iconSize);
        ImGui::TextUnformatted(filename.c_str());
        ImGui::PopTextWrapPos();

        ImGui::EndGroup();
        ImGui::PopStyleColor();
    }

  private:
    std::filesystem::path m_baseDirectory;
    std::filesystem::path m_currentDirectory;

    // 图标配置
    std::unordered_map<std::string, std::string> m_iconMap;
    float m_itemWidth;
    float m_itemHeight;
    float m_iconSize;

    // 图标资源（需要提前加载）
    ImTextureID m_folderIcon = nullptr;
    ImTextureID m_fileIcon   = nullptr;

    // 样式参数
    float m_thumbnailSize;
    float m_padding;
};

} // namespace Airwave