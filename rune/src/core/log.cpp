#include "pch.hpp"
#include "rune/core/log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <vector>

namespace Rune
{
    Shared<spdlog::logger> s_coreLogger;
    Shared<spdlog::logger> s_gameLogger;

    void LogSystem::init()
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(CreateShared<spdlog::sinks::stdout_color_sink_mt>());

        logSinks[0]->set_pattern("%^[%T][%n] %v%$");

        s_coreLogger = CreateShared<spdlog::logger>("Core", logSinks.begin(), logSinks.end());
        spdlog::register_logger(s_coreLogger);
        s_coreLogger->set_level(spdlog::level::trace);
        s_coreLogger->flush_on(spdlog::level::trace);

        s_gameLogger = CreateShared<spdlog::logger>("Game", logSinks.begin(), logSinks.end());
        spdlog::register_logger(s_gameLogger);
        s_gameLogger->set_level(spdlog::level::trace);
        s_gameLogger->flush_on(spdlog::level::trace);
    }

    void LogSystem::cleanup()
    {
        s_gameLogger->flush();
        s_coreLogger->flush();
        spdlog::shutdown();
    }

    auto LogSystem::getCoreLogger() -> Shared<spdlog::logger>& { return s_coreLogger; }

    auto LogSystem::getGameLogger() -> Shared<spdlog::logger>& { return s_gameLogger; }

}