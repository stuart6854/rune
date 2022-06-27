#pragma once

#include "rune/defines.hpp"

#include <spdlog/spdlog.h>

namespace Rune
{
    class LogSystem
    {
    public:
        static void init();
        static void cleanup();

        static auto getCoreLogger() -> Shared<spdlog::logger>&;
        static auto getGameLogger() -> Shared<spdlog::logger>&;
    };
}