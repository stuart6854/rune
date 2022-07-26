// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#include "rune/defines.hpp"

#include <vulkan/vulkan.hpp>

namespace Rune
{
    bool vulkanIsLayerSupported(const char* layerName);

    bool vulkanIsExtensionSupported(const char* extensionName);

    struct QueueFamilyIndices
    {
        i32 graphics = -1;
        i32 compute = -1;
        i32 transfer = -1;
    };

    QueueFamilyIndices vulkanGetQueueFamilyIndices(vk::PhysicalDevice physicalDevice, vk::QueueFlags flags);
}
