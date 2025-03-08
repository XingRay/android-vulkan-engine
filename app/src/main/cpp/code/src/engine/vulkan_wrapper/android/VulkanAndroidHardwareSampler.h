//
// Created by leixing on 2025/1/26.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanSampler.h"

namespace engine {

    class VulkanAndroidHardwareSampler : public VulkanSampler {
    private:
        const VulkanDevice &mVulkanDevice;
        vk::Sampler mSampler;
        vk::SamplerYcbcrConversion mConversion;

    public:
//        VulkanAndroidHardwareSampler(const VulkanDevice &vulkanDevice, AHardwareBuffer *hardwareBuffer);

        VulkanAndroidHardwareSampler(const VulkanDevice &vulkanDevice,
                                     const vk::AndroidHardwareBufferFormatPropertiesANDROID &formatInfo);

        ~VulkanAndroidHardwareSampler() override;

        [[nodiscard]]
        vk::SamplerYcbcrConversion getConversion() const override;

        [[nodiscard]]
        vk::Sampler getSampler() const override;
    };

} // engine
