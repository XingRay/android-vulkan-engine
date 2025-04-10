//
// Created by leixing on 2025/1/7.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

#include "engine/vulkan_wrapper/image/VulkanImage.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanStagingBuffer.h"
#include "engine/vulkan_wrapper/image/VulkanSamplerInterface.h"

namespace engine {

    class VulkanSampler : public VulkanSamplerInterface {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Sampler mSampler;

    public:

        VulkanSampler(const VulkanDevice &vulkanDevice, float maxLoad);

        ~VulkanSampler() override;

        [[nodiscard]]
        const vk::Sampler &getSampler() const override;
    };

} // engine
