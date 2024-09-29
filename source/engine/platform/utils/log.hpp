#pragma once

#include <spdlog/common.h>

namespace Airwave
{
class Log
{
  public:
    Log(const Log &)            = delete;
    Log &operator=(const Log &) = delete;

    static void Init();

    static Log *GetLoggerInstance() { return &s_LoggerInstance; }

    template <typename... Args>
    void Logger(spdlog::source_loc loc, spdlog::level::level_enum lvl,
                spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        spdlog::memory_buf_t buf;
        fmt::vformat_to(fmt::appender(buf), fmt, fmt::make_format_args(args...));
        Logger(loc, lvl, buf);
    }

  private:
    Log()  = default;
    ~Log() = default;

    void Logger(spdlog::source_loc loc, spdlog::level::level_enum lvl,
                const spdlog::memory_buf_t &buffer);

    static Log s_LoggerInstance;
};

#define LOG_LOGGER_CALL(Log, level, ...)                                                           \
    (Log)->Logger(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#define LOG_TRACE(...)                                                                             \
    LOG_LOGGER_CALL(Airwave::Log::GetLoggerInstance(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)                                                                             \
    LOG_LOGGER_CALL(Airwave::Log::GetLoggerInstance(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)                                                                              \
    LOG_LOGGER_CALL(Airwave::Log::GetLoggerInstance(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)                                                                              \
    LOG_LOGGER_CALL(Airwave::Log::GetLoggerInstance(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)                                                                             \
    LOG_LOGGER_CALL(Airwave::Log::GetLoggerInstance(), spdlog::level::err, ##__VA_ARGS__)

#define AW_ASSERT(x, ...)                                                                     \
    if (!x)                                                                                        \
    {                                                                                              \
        LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                           \
        __debugbreak();                                                                            \
    }
} // namespace Airwave
