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

    QueueFamilyIndices vulkanGetQueueFamilyIndices(vk::PhysicalDevice physicalDevice, vk::QueueFlags flags)
    {
        auto queueProperties = physicalDevice.getQueueFamilyProperties();

        QueueFamilyIndices indices{};

        // Look for dedicated compute queue
        if (flags & vk::QueueFlagBits::eCompute)
        {
            for (int i = 0; i < queueProperties.size(); ++i)
            {
                auto& queueFamilyProperties = queueProperties[i];
                if (queueFamilyProperties.queueFlags & vk::QueueFlagBits::eCompute &&
                    !(queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics))
                {
                    indices.compute = i;
                    break;
                }
            }
        }

        // Look for dedicated transfer queue
        if (flags & vk::QueueFlagBits::eTransfer)
        {
            for (int i = 0; i < queueProperties.size(); ++i)
            {
                auto& queueFamilyProperties = queueProperties[i];
                if (queueFamilyProperties.queueFlags & vk::QueueFlagBits::eTransfer &&
                    !(queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics) &&
                    !(queueFamilyProperties.queueFlags & vk::QueueFlagBits::eCompute))
                {
                    indices.transfer = i;
                    break;
                }
            }
        }

        for (int i = 0; i < queueProperties.size(); ++i)
        {
            if (flags & vk::QueueFlagBits::eTransfer && indices.transfer == -1)
            {
                if (queueProperties[i].queueFlags & vk::QueueFlagBits::eTransfer)
                    indices.transfer = i;
            }
            if (flags & vk::QueueFlagBits::eCompute && indices.compute == -1)
            {
                if (queueProperties[i].queueFlags & vk::QueueFlagBits::eCompute)
                    indices.compute = i;
            }
            if (flags & vk::QueueFlagBits::eGraphics)
            {
                if (queueProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
                    indices.graphics = i;
            }
        }

        return indices;
    }
}
