#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <chrono>

#include "core/window.hpp"
#include "ecs/aw_scene.hpp"
#include "rendering/renderer.hpp"
namespace Airwave
{

struct ApplicationConfig
{
    uint32_t width  = 1400;
    uint32_t height = 800;

    std::string title = "Airwave Engine";
};

class Application
{
  public:
    Application()                               = default;
    Application(const Application &)            = delete;
    Application &operator=(const Application &) = delete;
    virtual ~Application();

    void start(int argc, char **argv);
    void mainLoop();
    void quit();

    uint32_t getWindowWidth() const { return m_window->getWidth(); }
    uint32_t getWindowHeight() const { return m_window->getHeight(); }
    uint32_t getFPS() const { return 1.0f / m_deltaTime; }

    Window *getWindow() const { return m_window.get(); }
    AwScene *getScene() const { return m_scene.get(); }
    Renderer *getRenderer() const { return m_renderer.get(); }
    entt::entity getAdminEntity() const { return m_scene->getAdminEntity(); }

  protected:
    virtual void onConfigurate(ApplicationConfig &config) {}
    virtual void onInit() {}
    virtual void onDestory() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onRender() {}
    virtual void onImGuiRender() {}
    virtual void onPreLoad() {}

    std::chrono::steady_clock::time_point m_lastFrameTimePoint;
    std::chrono::steady_clock::time_point m_startTimePoint;
    uint64_t m_frameIndex = 0;

    std::unique_ptr<Window> m_window;
    std::unique_ptr<AwScene> m_scene;
    std::unique_ptr<Renderer> m_renderer;

    float m_deltaTime = 0.0f;

  private:
    void preLoad();


    ApplicationConfig m_config;

    bool b_pause = false;
};
} // namespace Airwave
