//
// Created by leixing on 2025/1/7.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

#include "engine/vulkan_wrapper/image/VulkanImage.h"
#include "SamplerInterface.h"

namespace engine {

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
