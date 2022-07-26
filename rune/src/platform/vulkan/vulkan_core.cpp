// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "vulkan_core.hpp"

#include "vulkan_utils.hpp"
#include "rune/macros.hpp"
#include "rune/defines.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace Rune
{
    struct VulkanData
    {
        vk::Instance instance;
        vk::DebugUtilsMessengerEXT debugMessenger;

        vk::PhysicalDevice physicalDevice;
        QueueFamilyIndices queueFamilyIndices;
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

        vk::Device device;
        vk::Queue graphicsQueue;
        vk::Queue computeQueue;
    };

    static Owned<VulkanData> s_data;

    static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                              const VkDebugUtilsMessageTypeFlagsEXT types,
                                                              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                              void* pUserData)
    {
        if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            CORE_LOG_ERROR("Vulkan: {}", pCallbackData->pMessage);
        if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            CORE_LOG_WARN("Vulkan: {}", pCallbackData->pMessage);

        // NOTE: Should always return false
        return false;
    }

    void Vulkan::init(const std::string& appName)
    {
        s_data = CreateOwned<VulkanData>();

        initInstance(appName);
        pickPhysicalDevice();

        vk::PhysicalDeviceFeatures features{};
        initLogicalDevice(features);
    }

    void Vulkan::cleanup()
    {
        s_data->device.destroy();
        s_data->instance.destroy(s_data->debugMessenger);
        s_data->instance.destroy();

        s_data = nullptr;
    }

    void Vulkan::initInstance(const std::string& appName)
    {
        vk::ApplicationInfo appInfo{};
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.pEngineName = "Rune";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pApplicationName = appName.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);

        std::vector<const char*> extensions;
        std::vector<const char*> layers;

        // Init dynamic dispatch loader
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        vk::InstanceCreateInfo instanceCreateInfo{};

#ifdef RUNE_DEBUG
        constexpr auto VALIDATION_LAYER_NAME = "VK_LAYER_KHRONOS_validation";
        if (vulkanIsLayerSupported(VALIDATION_LAYER_NAME))
            layers.push_back(VALIDATION_LAYER_NAME);

        if (vulkanIsExtensionSupported(VK_EXT_DEBUG_UTILS_EXTENSION_NAME))
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        debugCreateInfo.messageSeverity =
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
        debugCreateInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation;
        debugCreateInfo.pfnUserCallback = &vulkanDebugCallback;

        // Passing to instance create info enabled validation for instance creation & destruction
        instanceCreateInfo.pNext = &debugCreateInfo;
#endif

        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
        instanceCreateInfo.enabledLayerCount = layers.size();
        instanceCreateInfo.ppEnabledLayerNames = layers.data();

        s_data->instance = vk::createInstance(instanceCreateInfo);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(s_data->instance);

#ifdef RUNE_DEBUG
        // Create the actual debug messenger
        s_data->debugMessenger = s_data->instance.createDebugUtilsMessengerEXT(debugCreateInfo);
#endif
    }

    void Vulkan::pickPhysicalDevice()
    {
        int bestIndex = 0;
        int bestScore = -1;
        auto physicalDevices = s_data->instance.enumeratePhysicalDevices();
        for (int i = 0; i < physicalDevices.size(); ++i)
        {
            const auto& physicalDevice = physicalDevices[i];
            const auto& properties = physicalDevice.getProperties();
            const auto& features = physicalDevice.getFeatures();

            int score = 0;

            if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
                score += 1000;

            score += properties.limits.maxImageDimension2D;

            if (features.tessellationShader)
                score += 50;

            if (features.geometryShader)
                score += 50;

            if (features.samplerAnisotropy)
                score += 100;

            if (score > bestScore)
            {
                bestIndex = i;
                bestScore = score;
            }
        }

        auto physicalDevice = physicalDevices[bestIndex];
        auto properties = physicalDevice.getProperties();
        s_data->physicalDevice = physicalDevice;

        CORE_LOG_INFO("GPU: {}", properties.deviceName);
        CORE_LOG_TRACE("  Score: {}", bestScore);
        CORE_LOG_INFO("  Driver: {}.{}.{}",
                      VK_VERSION_MAJOR(properties.driverVersion),
                      VK_VERSION_MINOR(properties.driverVersion),
                      VK_VERSION_PATCH(properties.driverVersion));
        CORE_LOG_INFO("  Vulkan Version: {}.{}.{}",
                      VK_VERSION_MAJOR(properties.apiVersion),
                      VK_VERSION_MINOR(properties.apiVersion),
                      VK_VERSION_PATCH(properties.apiVersion));

        // Queue Families
        constexpr auto requestedQueueTypes = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer;
        auto queueFamilyIndices = vulkanGetQueueFamilyIndices(s_data->physicalDevice, requestedQueueTypes);

        static constexpr float DEFAULT_QUEUE_PRIORITY(0.0f);

        // Graphics queue
        if (requestedQueueTypes & vk::QueueFlagBits::eGraphics)
        {
            vk::DeviceQueueCreateInfo queueInfo{};
            queueInfo.queueFamilyIndex = queueFamilyIndices.graphics;
            queueInfo.queueCount = 1;
            queueInfo.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;
            s_data->queueCreateInfos.push_back(queueInfo);
        }
        // Dedicated compute queue
        if (requestedQueueTypes & vk::QueueFlagBits::eCompute)
        {
            if (queueFamilyIndices.compute != queueFamilyIndices.graphics)
            {
                vk::DeviceQueueCreateInfo queueInfo{};
                queueInfo.queueFamilyIndex = queueFamilyIndices.compute;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;
                s_data->queueCreateInfos.push_back(queueInfo);
            }
        }
        // Dedicated transfer queue
        if (requestedQueueTypes & vk::QueueFlagBits::eTransfer)
        {
            if (queueFamilyIndices.transfer != queueFamilyIndices.graphics && queueFamilyIndices.transfer != queueFamilyIndices.compute)
            {
                vk::DeviceQueueCreateInfo queueInfo{};
                queueInfo.queueFamilyIndex = queueFamilyIndices.transfer;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY;
                s_data->queueCreateInfos.push_back(queueInfo);
            }
        }

        s_data->queueFamilyIndices = queueFamilyIndices;
    }

    void Vulkan::initLogicalDevice(const vk::PhysicalDeviceFeatures& deviceFeatures)
    {
        std::vector<const char*> extensions;
        if (vulkanIsExtensionSupported(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
            extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        vk::DeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.queueCreateInfoCount = s_data->queueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = s_data->queueCreateInfos.data();
        deviceCreateInfo.enabledExtensionCount = extensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = extensions.data();

        s_data->device = s_data->physicalDevice.createDevice(deviceCreateInfo);

        s_data->graphicsQueue = s_data->device.getQueue(s_data->queueFamilyIndices.graphics, 0);
        s_data->computeQueue = s_data->device.getQueue(s_data->queueFamilyIndices.compute, 0);
    }
}
