// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "vulkan_utils.hpp"

#include <vulkan/vulkan.hpp>

namespace Rune
{
    bool vulkanIsLayerSupported(const char* layerName)
    {
        auto layers = vk::enumerateInstanceLayerProperties();
        for (const auto& layer : layers)
        {
            if (std::strcmp(layerName, layer.layerName) == 0)
                return true;
        }
        return false;
    }

    bool vulkanIsExtensionSupported(const char* extensionName)
    {
        auto extensions = vk::enumerateInstanceExtensionProperties();
        for (const auto& extension : extensions)
        {
            if (std::strcmp(extensionName, extension.extensionName) == 0)
                return true;
        }
        return false;
    }
}
