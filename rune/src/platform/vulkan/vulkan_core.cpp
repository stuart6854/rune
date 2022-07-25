// # Copyright © Stuart Millman <stu.millman15@gmail.com>

#include "pch.hpp"
#include "vulkan_core.hpp"

#include "vulkan_utils.hpp"
#include "rune/macros.hpp"
#include "rune/defines.hpp"

namespace Rune
{
    struct VulkanData
    {
        vk::Instance instance;
        vk::DebugUtilsMessengerEXT debugMessenger;
    };

    static Owned<VulkanData> s_data;

    static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                              VkDebugUtilsMessageTypeFlagsEXT types,
                                                              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                              void* pUserData)
    {
        std::string typeStr = "";
        if (types & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
            typeStr = "General";
        else if (types & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
            typeStr = "Validation";
        else if (types & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
            typeStr = "Performance";

        if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
            LOG_ERROR("Vulkan-{}:\n{}", typeStr, pCallbackData->pMessage);
        if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            LOG_WARN("Vulkan-{}:\n{}", typeStr, pCallbackData->pMessage);

        // NOTE: Should always return false
        return false;
    }

    void Vulkan::init(const std::string& appName)
    {
        s_data = CreateOwned<VulkanData>();

        initInstance(appName);
    }

    void Vulkan::cleanup()
    {
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

        s_data->instance = vk::createInstance(instanceCreateInfo);

#ifdef RUNE_DEBUG
        // Create the actual debug messenger
        s_data->debugMessenger = s_data->instance.createDebugUtilsMessengerEXT(debugCreateInfo);
#endif
    }
}
