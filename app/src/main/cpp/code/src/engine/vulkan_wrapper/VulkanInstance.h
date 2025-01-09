//
// Created by leixing on 2024/12/22.
//

#pragma once

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif

#include <string>

#include <vulkan/vulkan.hpp>

#include "common/StringListSelector.h"

namespace engine {

    class VulkanInstance {
    private:
        vk::Instance mInstance;
        vk::DebugReportCallbackEXT mDebugReportCallback;
        std::vector<const char *> mEnabledInstanceExtensionNames;
        std::vector<const char *> mEnabledLayerNames;

        vk::DebugUtilsMessengerEXT mDebugMessenger;

    public:
        VulkanInstance(const std::string& applicationName,
                       uint32_t applicationVersion,
                       const std::string& engineName,
                       uint32_t engineVersion,
                       const common::StringListSelector &extensionsSelector,
                       const common::StringListSelector &layersSelector);

        ~VulkanInstance();

        [[nodiscard]]
        const vk::Instance &getInstance() const {
            return mInstance;
        }

        [[nodiscard]]
        const std::vector<const char *> &getEnabledExtensions() const {
            return mEnabledInstanceExtensionNames;
        }

        [[nodiscard]]
        const std::vector<const char *> &getEnabledLayers() const {
            return mEnabledLayerNames;
        }

    private:
        void setupDebugCallback();

        void setupDebugMessenger();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallbackImpl(
                VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT objectType,
                uint64_t object,
                size_t location,
                int32_t messageCode,
                const char *pLayerPrefix,
                const char *pMessage,
                void *pUserData);

        void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo);

        vk::Result CreateDebugUtilsMessengerEXT(vk::Instance instance,
                                                const vk::DebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                const vk::AllocationCallbacks *pAllocator,
                                                vk::DebugUtilsMessengerEXT *pDebugMessenger);
    };
}
