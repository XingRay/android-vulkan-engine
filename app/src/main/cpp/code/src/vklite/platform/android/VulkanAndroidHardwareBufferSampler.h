//
// Created by leixing on 2025/1/26.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/VulkanDevice.h"
#include "vklite/sampler/DefaultSampler.h"
#include "AndroidHardwareBuffer.h"
#include "VulkanAndroidHardwareBufferYcbcrConversion.h"
#include "VulkanAndroidHardwareBufferImage.h"
#include "vklite/sampler/SamplerInterface.h"

namespace vklite {

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
