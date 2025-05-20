//
// Created by leixing on 2025/3/15.
//

#include "VulkanAndroidInstancePlugin.h"
#include "VulkanAndroidApi.h"

namespace vklite {

    VulkanAndroidInstancePlugin::VulkanAndroidInstancePlugin() = default;

    VulkanAndroidInstancePlugin::~VulkanAndroidInstancePlugin() = default;

    void VulkanAndroidInstancePlugin::onVulkanInstanceCreated(const VulkanInstance &vulkanInstance) {
        const vk::Instance &instance = vulkanInstance.getInstance();

        vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID) vkGetInstanceProcAddr(instance, "vkGetAndroidHardwareBufferPropertiesANDROID");
        vkBindImageMemory2KHR = (PFN_vkBindImageMemory2KHR) vkGetInstanceProcAddr(instance, "vkBindImageMemory2KHR");

        vkCreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion) vkGetInstanceProcAddr(instance, "vkCreateSamplerYcbcrConversion");
        vkDestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion) vkGetInstanceProcAddr(instance, "vkDestroySamplerYcbcrConversion");

        vkGetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR) vkGetInstanceProcAddr(instance, "vkGetImageMemoryRequirements2KHR");
    }
} // engine