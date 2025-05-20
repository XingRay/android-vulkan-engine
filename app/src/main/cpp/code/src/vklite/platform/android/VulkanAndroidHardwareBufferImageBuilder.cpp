//
// Created by leixing on 2025/4/15.
//

#include "VulkanAndroidHardwareBufferImageBuilder.h"
#include "VulkanAndroidHardwareBufferImage.h"

namespace vklite {

    VulkanAndroidHardwareBufferImageBuilder::VulkanAndroidHardwareBufferImageBuilder(const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                                                                     const AndroidHardwareBuffer &androidHardwareBuffer)
            : mVulkanAndroidSamplerYcbcrConversion(vulkanAndroidSamplerYcbcrConversion),
              mAndroidHardwareBuffer(androidHardwareBuffer) {}

    VulkanAndroidHardwareBufferImageBuilder::~VulkanAndroidHardwareBufferImageBuilder() = default;

    std::unique_ptr<VulkanImageInterface> VulkanAndroidHardwareBufferImageBuilder::build(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height, vk::Format format) {
        return std::make_unique<VulkanAndroidHardwareBufferImage>(vulkanDevice, mAndroidHardwareBuffer, mVulkanAndroidSamplerYcbcrConversion);
    }

} // engine