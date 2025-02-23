#pragma once

#include <memory>

#include "core/entry_point.hpp"
#include "airwave_engine.hpp"
#include "editor.hpp"

namespace Airwave
{
  class test_event{
    public:
      int a = 0;
      int b = 1;
  };
  
class Sandbox : public Airwave::Application
{
  public:
    Sandbox() {}
    ~Sandbox() {}

    entt::entity createStairs(std::string name, glm::vec3 position, glm::vec3 size, glm::vec3 rotation, int step);

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