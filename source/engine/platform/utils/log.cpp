#include "log.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"

namespace Airwave
{
Log Log::s_LoggerInstance{};

static std::shared_ptr<spdlog::logger> s_SpdLogger{};

void Log::Init()
{
    s_SpdLogger = spdlog::stdout_color_mt<spdlog::async_factory>("Airwave");
    s_SpdLogger->set_level(spdlog::level::trace);
    s_SpdLogger->set_pattern("%^%H:%M:%S:%e [%P-%t] [%1!L] [%20s:%-4#] - %v%$");
}

void Log::Logger(spdlog::source_loc loc, spdlog::level::level_enum lvl,
                 const spdlog::memory_buf_t &buffer)
{
    s_SpdLogger->log(loc, lvl, spdlog::string_view_t(buffer.data(), buffer.size()));
}

} // namespace Airwave
