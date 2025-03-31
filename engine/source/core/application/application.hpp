#pragma once

namespace Airwave
{
class Application
{
  public:
    Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;
    virtual ~Application() = default;

    void init(int argc, char** argv);
    void mainLoop();
    void shutdown();


  protected:
  private:
};

} // namespace Airwave
