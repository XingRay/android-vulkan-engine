//
// Created by leixing on 2025/3/17.
//

#include "engine/vulkan_wrapper/platform/android/VulkanAndroidApi.h"

namespace engine {

    PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
    PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;

    PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
    PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;

    PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;

} // engine