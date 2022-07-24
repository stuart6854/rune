#pragma once

#include "pch.hpp"

#if RUNE_PLATFORM_WINDOWS
    #define UNNAMED_DEBUG_BREAK() __debugbreak()
#elif RUNE_PLATFORM_APPLE
    #error Apple platforms are not currently supported!
#elif RUNE_PLATFORM_LINUX
    #include <signal.h>
    #define UNNAMED_DEBUG_BREAK() raise(SIGTRAP)
#elif RUNE_PLATFORM_ANDROID
    #error Android platform is not currently supported!
#else
    #error Unknown platform is not currently supported!
#endif

#include "core/log.hpp"

#define CORE_LOG_TRACE(...) ::Rune::LogSystem::getCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_INFO(...) ::Rune::LogSystem::getCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_DEBUG(...) ::Rune::LogSystem::getCoreLogger()->debug(__VA_ARGS__)
#define CORE_LOG_WARN(...) ::Rune::LogSystem::getCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...) ::Rune::LogSystem::getCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_CRITICAL(...) ::Rune::LogSystem::getCoreLogger()->critical(__VA_ARGS__)

#define LOG_TRACE(...) ::Rune::LogSystem::getGameLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::Rune::LogSystem::getGameLogger()->info(__VA_ARGS__)
#define LOG_DEBUG(...) ::Rune::LogSystem::getGameLogger()->debug(__VA_ARGS__)
#define LOG_WARN(...) ::Rune::LogSystem::getGameLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Rune::LogSystem::getGameLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Rune::LogSystem::getGameLogger()->critical(__VA_ARGS__)

namespace Rune
{
#ifdef _DEBUG
    #define RUNE_ENG_ASSERT(expr, msg) \
        if (!(expr))                   \
        {                              \
            CORE_LOG_ERROR("[Engine][Assert] Failed: {} at {}:{}", #expr, __FILE__, __LINE__);\
            CORE_LOG_ERROR("{}", msg);       \
            UNNAMED_DEBUG_BREAK();   \
        }
#else
    #define RUNE_ENG_ASSERT(expr)
#endif
}