#pragma once

#include <string>
#include "editor.hpp"

namespace Airwave
{
class Panel
{
  public:
    Panel(Editor *editor, const std::string &title, bool startOpened = true)
        : m_editor(editor), m_title(title), m_isOpen(startOpened)
    {
    }

    virtual ~Panel() = default;

    virtual void onAttach() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onImGuiRender() = 0;

    // 返回面板的标题
    const std::string &getTitle() const { return m_title; }

    // 检查面板是否打开
    bool isOpen() const { return m_isOpen; }

    // 切换面板的打开状态
    void toggle() { m_isOpen = !m_isOpen; }

    // 强制打开或关闭面板
    void open() { m_isOpen = true; }
    void close() { m_isOpen = false; }

  protected:
    std::string m_title; // 面板标题
    bool m_isOpen;       // 面板是否打开

    Editor *m_editor{nullptr};
};

} // namespace Airwave
