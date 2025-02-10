#pragma once

#include <memory>
#include <imgui.h>
#include <imgui_internal.h>
#include <functional>

#include "core/window.hpp"

#include "core/application.hpp"

namespace Airwave
{
class Panel;
class Editor
{
  public:
    Editor(Application *context);
    ~Editor();

    void setThemeStyle();
    void setFontStyle();

    Application *getContext() { return m_context; }

    void beginNewFrame();
    void endNewFrame();
    void render();

    void beginDockSpace();
    void endDockSpace();

    void dealMultiViewport();

    template <typename T, typename... Args> T *addPanel(Args &&...args)
    {
        m_panels.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return static_cast<T *>(m_panels.back().get());
    }

    void onUpdate(float deltaTime);
    void renderPanels();

  public:
    void setSelectedEntity(entt::entity entity) { m_selectedEntity = entity; }
    entt::entity getSelectedEntity() { return m_selectedEntity; }

    std::function<void()> onSelectedEntityChanged;
    std::function<void()> onDrawDebugInfo;

  protected:
    Application *m_context;
    std::vector<std::unique_ptr<Panel>> m_panels;

    entt::entity m_selectedEntity = entt::null;
};

} // namespace Airwave
