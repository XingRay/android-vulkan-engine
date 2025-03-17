//
// Created by leixing on 2025/1/26.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/buffer/VulkanSampler.h"
#include "engine/vulkan_wrapper/platform/android/AndroidHardwareBuffer.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidSamplerYcbcrConversion.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidHardwareBufferImage.h"

namespace engine {

    class VulkanAndroidHardwareBufferSampler {
    private:
        const VulkanDevice &mVulkanDevice;
        vk::Sampler mSampler;
        VulkanAndroidHardwareBufferImage mImage;

    public:

        VulkanAndroidHardwareBufferSampler(const VulkanDevice &vulkanDevice,
                                           const AndroidHardwareBuffer& androidHardwareBuffer,
                                           const VulkanAndroidSamplerYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~VulkanAndroidHardwareBufferSampler();

        [[nodiscard]]
        vk::Sampler getSampler() const;

        [[nodiscard]]
        std::vector<vk::DescriptorImageInfo> createDescriptorImageInfos();
    };

} // engine
