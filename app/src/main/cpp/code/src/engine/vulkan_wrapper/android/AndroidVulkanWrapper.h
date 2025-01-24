//
// Created by leixing on 2025/1/24.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace engine {

    extern PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
    extern PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;

    extern PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
    extern PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;

    extern PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;

    extern bool initialized;

    void initAndroidVulkanWrapper(const vk::Instance &instance);
}


