//
// Created by leixing on 2025/1/26.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/sampler/DefaultSampler.h"
#include "engine/vulkan_wrapper/platform/android/AndroidHardwareBuffer.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidHardwareBufferYcbcrConversion.h"
#include "engine/vulkan_wrapper/platform/android/VulkanAndroidHardwareBufferImage.h"
#include "engine/vulkan_wrapper/sampler/SamplerInterface.h"

namespace engine {

    class VulkanAndroidHardwareBufferSampler : public SamplerInterface {
    private:
        const VulkanDevice &mVulkanDevice;
        vk::Sampler mSampler;

    public:

        VulkanAndroidHardwareBufferSampler(const VulkanDevice &vulkanDevice,
                                           const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~VulkanAndroidHardwareBufferSampler() override;

        [[nodiscard]]
        const vk::Sampler &getSampler() const override;

    };

} // engine
