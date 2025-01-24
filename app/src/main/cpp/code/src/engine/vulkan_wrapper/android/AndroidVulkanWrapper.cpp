//
// Created by leixing on 2025/1/24.
//

#include "AndroidVulkanWrapper.h"

namespace engine {

    PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
    PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;

    PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
    PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;

    PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;

    bool initialized = false;

    void initAndroidVulkanWrapper(const vk::Instance &instance) {

        vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID) vkGetInstanceProcAddr(instance, "vkGetAndroidHardwareBufferPropertiesANDROID");
        vkBindImageMemory2KHR = (PFN_vkBindImageMemory2KHR) vkGetInstanceProcAddr(instance, "vkBindImageMemory2KHR");

        vkCreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion) vkGetInstanceProcAddr(instance, "vkCreateSamplerYcbcrConversion");
        vkDestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion) vkGetInstanceProcAddr(instance, "vkDestroySamplerYcbcrConversion");

        vkGetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR) vkGetInstanceProcAddr(instance, "vkGetImageMemoryRequirements2KHR");

        initialized = true;
    }
}
