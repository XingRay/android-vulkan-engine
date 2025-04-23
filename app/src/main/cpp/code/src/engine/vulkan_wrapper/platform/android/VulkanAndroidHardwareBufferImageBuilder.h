//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/image/VulkanImageBuilderInterface.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidHardwareBufferYcbcrConversion.h"
#include "engine/vulkan_wrapper/platform/android/AndroidHardwareBuffer.h"

namespace engine {

    class VulkanAndroidHardwareBufferImageBuilder : public VulkanImageBuilderInterface {
    private:
        const VulkanAndroidHardwareBufferYcbcrConversion &mVulkanAndroidSamplerYcbcrConversion;
        const AndroidHardwareBuffer &mAndroidHardwareBuffer;

    public:
        VulkanAndroidHardwareBufferImageBuilder(const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                                const AndroidHardwareBuffer &androidHardwareBuffer);

        ~VulkanAndroidHardwareBufferImageBuilder() override;

        std::unique_ptr<VulkanImageInterface> build(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height, vk::Format format) override;
    };

} // engine
