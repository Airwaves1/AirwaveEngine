#pragma once

#include <memory>
#include <string>
#include <chrono>
#include "window/aw_window.hpp"

namespace Airwave
{

struct ApplicationConfig
{
    uint32_t width = 1280;
    uint32_t height = 720;
    std::string title = "Airwave Engine";

    bool enableImGui = true;
};

class Application
{
  public:
    Application() {}
    Application(const Application &)            = delete;
    Application &operator=(const Application &) = delete;
    virtual ~Application() {}

    void start(int argc, char **argv);
    void quit();
    void mainLoop();

    const ApplicationConfig &getConfig() const { return m_config; }

    const std::unique_ptr<AwWindow> &getWindow() const { return m_window; }

  protected:
    virtual void onConfigurate(ApplicationConfig &config) {}
    virtual void onInit() {}
    virtual void onUpdate(float deltaTime) {}
    virtual void onRender() {}
    virtual void onImGuiRender() {}
    virtual void onDestory() {}
    virtual void onEvent() {}

    std::chrono::steady_clock::time_point m_startTimePoint;
    std::chrono::steady_clock::time_point m_lastFrameTimePoint;

    uint64_t m_frameIndex = 0;

  private:
    void parseArgs(int argc, char **argv);

  private:
    ApplicationConfig m_config;
    bool b_pause = false;

    std::unique_ptr<AwWindow> m_window;


};

} // namespace Airwave
