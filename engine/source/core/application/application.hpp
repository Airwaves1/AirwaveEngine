#pragma once
#include <string>
#include <chrono>
namespace Airwave
{

class Application
{
  public:
    Application()                               = default;
    Application(const Application &)            = delete;
    Application &operator=(const Application &) = delete;
    Application(Application &&)                 = delete;
    Application &operator=(Application &&)      = delete;
    virtual ~Application()                      = default;

    void init(int argc, char **argv);
    void mainLoop();
    void shutdown();

    std::string getConfigPath(int argc, char **argv) const;
    float calculateFPS(float delta_time);

  protected:
    virtual void onInit(){};
    virtual void onUpdate(float delta_time){};
    virtual void onRender(){};
    virtual void onShutdown(){};
    
  protected:
    std::string m_config_path = "";
    std::chrono::steady_clock::time_point m_lastFrameTimePoint{};
    std::chrono::steady_clock::time_point m_startTimePoint{};
    std::atomic<bool> m_alive = true;

    float m_average_duration{0.f};
    int m_frame_count{0};
    int m_fps{0};

  private:
};

} // namespace Airwave
