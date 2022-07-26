// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

namespace Rune
{
    class Vulkan
    {
    public:
        static void init(const std::string& appName);
        static void cleanup();

    private:
        static void initInstance(const std::string& appName);
        static void pickPhysicalDevice();
        static void initLogicalDevice(const vk::PhysicalDeviceFeatures& deviceFeatures);
    };
}