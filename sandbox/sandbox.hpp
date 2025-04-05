#pragma once

#include "engine.hpp"

class Sandbox : public Airwave::Application
{
  public:
  protected:
    virtual void onInit() override;
    virtual void onUpdate(float delta_time) override;
    virtual void onShutdown() override;

  private:
    uint32_t m_vao{0};
    uint32_t m_vbo{0};
    uint32_t m_ebo{0};
    std::shared_ptr<Airwave::Texture> m_texture{nullptr};
    std::shared_ptr<Airwave::Shader> m_shader{nullptr};
};
Airwave::Application *CreateApplication(int argc, char **argv) { return new Sandbox(); } // namespace Airwave