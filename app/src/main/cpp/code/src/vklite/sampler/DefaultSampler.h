//
// Created by leixing on 2025/1/7.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/VulkanDevice.h"
#include "vklite/command/VulkanCommandPool.h"

#include "vklite/image/VulkanImage.h"
#include "SamplerInterface.h"

namespace vklite {

    class DefaultSampler : public SamplerInterface {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Sampler mSampler;

    public:

        DefaultSampler(const VulkanDevice &vulkanDevice, float maxLoad);

        ~DefaultSampler() override;

        [[nodiscard]]
        const vk::Sampler &getSampler() const override;
    };

} // engine
