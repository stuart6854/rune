// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"

namespace Rune
{
    struct EventWindowClose
    {
    };

    struct EventWindowSize
    {
        i32 width;
        i32 height;
    };

    struct EventFramebufferSize
    {
        i32 width;
        i32 height;
    };

    struct EventKeyDown
    {
        i32 key;
    };

    struct EventKeyUp
    {
        i32 key;
    };

    struct EventKeyHeld
    {
        i32 key;
    };

    struct EventMouseBtnDown
    {
        i32 btn;
    };

    struct EventMouseBtnUp
    {
        i32 btn;
    };

    struct EventCursorPos
    {
        double x;
        double y;
    };
}