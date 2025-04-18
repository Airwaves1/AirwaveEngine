#pragma once

#include <memory>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "function/window/aw_window.hpp"
#include "function/ecs/world.hpp"
#include "core/resource/resource_manager.hpp"
namespace Airwave
{
struct SandboxConfig
{
    std::string sandbox_path = "sandbox/";
    std::string sandbox_name = "sandbox";
};

struct EngineConfig
{
    std::string assets_path = "assets/";
    SandboxConfig sandbox_config;
    AwWindowConfig window_config;
};

#define GlobalContext RuntimeGlobalContext::getInstance()
#define AssetsPath GlobalContext.engine_config.assets_path

class RuntimeGlobalContext
{
  public:
    static RuntimeGlobalContext &getInstance()
    {
        static RuntimeGlobalContext instance;
        return instance;
    }

    void startRuntimeSystem(const std::string &config_file_path);
    void shutdownRuntimeSystem();

    void update(float delta_time);

    EngineConfig serializeConfig(const std::string &config_file_path);
    SandboxConfig serializeSandboxConfig(const nlohmann::json &json_config);
    AwWindowConfig serializeWindowConfig(const nlohmann::json &json_config);

  public:
    EngineConfig engine_config;
    std::shared_ptr<AwWindow> window = nullptr;
    std::shared_ptr<World> world     = nullptr;
    std::shared_ptr<ResourceManager> resource_manager = nullptr;
};
} // namespace Airwave
