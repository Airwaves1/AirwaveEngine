#pragma once

#include <memory>

#include "core/entry_point.hpp"
#include "airwave_engine.hpp"
#include "editor.hpp"

namespace Airwave
{
class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

  protected:
    void onConfigurate(Airwave::ApplicationConfig &config) override;
    void onInit() override;
    void onDestory() override;
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onImGuiRender() override;
    void onPreLoad() override;

  private:
    std::unique_ptr<Airwave::Editor> m_editor;
};

} // namespace Airwave

Airwave::Application *CreateAirwaveEngineApplication() { return new Airwave::Sandbox(); }