#include "rune/core/time.hpp"

#include <GLFW/glfw3.h>

namespace Rune
{
    f64 s_lastFrameTime = 0.0;
    f64 s_deltaTime = 0.0;

    auto Time::getTimeSinceStartup() -> f32
    {
        return static_cast<f32>(glfwGetTime());
    }

    auto Time::getDeltaTime() -> f32
    {
        return static_cast<f32>(s_deltaTime);
    }

    void Time::beginFrame()
    {
        auto currentTime = glfwGetTime();
        s_deltaTime = currentTime - s_lastFrameTime;
        s_lastFrameTime = currentTime;
    }
}
