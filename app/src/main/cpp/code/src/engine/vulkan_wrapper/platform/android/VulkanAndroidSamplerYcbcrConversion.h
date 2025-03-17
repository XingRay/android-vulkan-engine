//
// Created by leixing on 2025/3/15.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "engine/vulkan_wrapper/VulkanDevice.h"

namespace engine {

    class VulkanAndroidSamplerYcbcrConversion {
    private:
        const VulkanDevice& mVulkanDevice;

        vk::SamplerYcbcrConversion mSamplerYcbcrConversion;

    public:
        VulkanAndroidSamplerYcbcrConversion(const VulkanDevice &vulkanDevice,
                                            const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo);

        ~VulkanAndroidSamplerYcbcrConversion();

        [[nodiscard]]
        const vk::SamplerYcbcrConversion& getSamplerYcbcrConversion()const;
    };

} // engine
