//
// Created by leixing on 2025/3/15.
//

#pragma once

#include "vulkan/vulkan.hpp"

#include "vklite/device/VulkanDevice.h"

namespace vklite {

    class VulkanAndroidHardwareBufferYcbcrConversion {
    private:
        const VulkanDevice& mVulkanDevice;

        vk::SamplerYcbcrConversion mSamplerYcbcrConversion;

    public:
        VulkanAndroidHardwareBufferYcbcrConversion(const VulkanDevice &vulkanDevice,
                                                   const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo);

        ~VulkanAndroidHardwareBufferYcbcrConversion();

        [[nodiscard]]
        const vk::SamplerYcbcrConversion& getSamplerYcbcrConversion()const;
    };

} // engine
