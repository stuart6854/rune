// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#pragma once

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
    };
}