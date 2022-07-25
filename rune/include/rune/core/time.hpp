// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"

namespace Rune
{
    namespace Time
    {
        auto getTimeSinceStartup() -> f32;
        auto getDeltaTime() -> f32;

        void beginFrame();

    }
}
