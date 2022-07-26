#include "pch.hpp"
#include "graphics/renderer.hpp"

#include "vulkan_core.hpp"

namespace Rune
{
    void Renderer::init(const std::string& appName)
    {
        Vulkan::init(appName);
    }

    void Renderer::cleanup()
    {
        Vulkan::cleanup();
    }

}
