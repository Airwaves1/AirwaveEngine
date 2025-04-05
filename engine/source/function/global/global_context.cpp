#include "global_context.hpp"
#include "core/log/log.hpp"
#include <fstream>
#include <iostream>

namespace Airwave
{
RuntimeGlobalContext g_runtime_global_context;
std::string g_assets_path = "assets/";
std::string g_shader_path = "shaders/";

void RuntimeGlobalContext::startRuntimeSystem(const std::string &config_file_path)
{
    // 日志系统初始化
    LogSystem::Init();

    try
    {
        engine_config = serializeConfig(config_file_path);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading config file: " << e.what() << std::endl;
        return;
    }

    // 创建窗口
    window = std::make_shared<AwWindow>();
    window->initialize(engine_config.window_config);

    // 创建世界
    world = std::make_shared<World>();

    LOG_INFO("Airwave Engine Start!");
}

void RuntimeGlobalContext::shutdownRuntimeSystem() {}

void RuntimeGlobalContext::update(float delta_time) {}

EngineConfig RuntimeGlobalContext::serializeConfig(const std::string &config_file_path)
{
    EngineConfig engine_config;

    // 打开 JSON 文件
    std::ifstream config_file(config_file_path);
    if (!config_file.is_open())
    {
        throw std::runtime_error("Failed to open config file: " + config_file_path);
    }

    // 解析 JSON
    nlohmann::json json_config;
    try
    {
        config_file >> json_config;
    }
    catch (const nlohmann::json::parse_error &e)
    {
        throw std::runtime_error("JSON parse error: " + std::string(e.what()));
    }
    config_file.close();

    // 解析 engine 节点
    if (!json_config.contains("engine") || !json_config["engine"].is_object())
    {
        throw std::runtime_error("Missing or invalid 'engine' section in config file");
    }

    nlohmann::json engine_json = json_config["engine"];

    engine_config.assets_path = engine_json.value("assets_path", "assets/");

    // 解析 sandbox 配置
    if (engine_json.contains("sandbox") && engine_json["sandbox"].is_object())
    {
        engine_config.sandbox_config = serializeSandboxConfig(engine_json["sandbox"]);
    }
    else
    {
        throw std::runtime_error("Missing or invalid 'sandbox' section in config file");
    }

    // 解析 window 配置
    if (engine_json.contains("window") && engine_json["window"].is_object())
    {
        engine_config.window_config = serializeWindowConfig(engine_json["window"]);
    }
    else
    {
        throw std::runtime_error("Missing or invalid 'window' section in config file");
    }

    return engine_config;
}

SandboxConfig RuntimeGlobalContext::serializeSandboxConfig(const nlohmann::json &json_config)
{
    SandboxConfig sandbox_config;

    try
    {
        if (!json_config.contains("config") || !json_config["config"].is_object())
        {
            throw std::runtime_error("Missing or invalid 'config' section in sandbox config");
        }
        const auto &config          = json_config["config"];
        sandbox_config.sandbox_path = config.value("sandbox_path", "sandbox/");
        sandbox_config.sandbox_name = config.value("sandbox_name", "sandbox");
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Error parsing sandbox config: " + std::string(e.what()));
    }

    return sandbox_config;
}

AwWindowConfig RuntimeGlobalContext::serializeWindowConfig(const nlohmann::json &json_config)
{
    AwWindowConfig window_config;

    try
    {
        if (!json_config.contains("config") || !json_config["config"].is_object())
        {
            throw std::runtime_error("Missing or invalid 'config' section in window config");
        }

        const auto &config = json_config["config"];

        window_config.width         = config.value("width", 1280);
        window_config.height        = config.value("height", 720);
        window_config.title         = config.value("title", "Airwave Engine");
        window_config.is_fullscreen = config.value("fullscreen", false);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Error parsing window config: " + std::string(e.what()));
    }

    return window_config;
}

}; // namespace Airwave
