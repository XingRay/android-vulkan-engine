//
// Created by leixing on 2025/4/15.
//

#include "VulkanAndroidHardwareBufferSamplerBuilder.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidHardwareBufferSampler.h"

namespace engine {

    VulkanAndroidHardwareBufferSamplerBuilder::VulkanAndroidHardwareBufferSamplerBuilder(const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion)
            : mVulkanAndroidSamplerYcbcrConversion(vulkanAndroidSamplerYcbcrConversion) {}

    VulkanAndroidHardwareBufferSamplerBuilder::~VulkanAndroidHardwareBufferSamplerBuilder() = default;

    std::unique_ptr<SamplerInterface> VulkanAndroidHardwareBufferSamplerBuilder::build(const VulkanDevice &vulkanDevice) {
        return std::make_unique<VulkanAndroidHardwareBufferSampler>(vulkanDevice, mVulkanAndroidSamplerYcbcrConversion);
    }

} // engine