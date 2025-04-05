#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <iostream>

namespace Airwave
{

class LogSystem
{
  public:
    // 初始化日志系统
    static void Init(const std::string &logFile = "engine.log")
    {
        try
        {
            // 创建带颜色的控制台日志 sink
            std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S] [%l] [%s:%#] %v%$");
            // 这里 `%^` 和 `%$` 包裹整行，保证整句都带颜色

            // 创建日志文件 sink
            std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true);
            file_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] [%s:%#] %v");

            // 组合 sink
            std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
            logger = std::make_shared<spdlog::logger>("EngineLogger", sinks.begin(), sinks.end());
            logger->set_level(spdlog::level::trace);
            spdlog::set_default_logger(logger);
        }
        catch (const spdlog::spdlog_ex &ex)
        {
            std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        }
    }

    // 关闭日志
    static void Shutdown()
    {
        logger.reset();
        spdlog::shutdown();
    }

    // 获取日志对象
    static std::shared_ptr<spdlog::logger> &GetLogger() { return logger; }

  private:
    static inline std::shared_ptr<spdlog::logger> logger;
};

} // namespace Airwave
#define LOG_TRACE(...) SPDLOG_LOGGER_CALL(Airwave::LogSystem::GetLogger(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_CALL(Airwave::LogSystem::GetLogger(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_CALL(Airwave::LogSystem::GetLogger(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_CALL(Airwave::LogSystem::GetLogger(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_CALL(Airwave::LogSystem::GetLogger(), spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CALL(Airwave::LogSystem::GetLogger(), spdlog::level::critical, __VA_ARGS__)
