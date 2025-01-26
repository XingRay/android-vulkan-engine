//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <android/hardware_buffer.h>

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"
#include "engine/vulkan_wrapper/VulkanBuffer.h"

namespace engine {

    class VulkanHardwareBuffer : public VulkanBuffer {
    private:
        const VulkanInstance &mVulkanInstance;
        const VulkanDevice &mVulkanDevice;

        vk::Sampler mSampler;
        vk::SamplerYcbcrConversion mConversion;

        const VulkanCommandPool &mCommandPool;
        const vk::DescriptorSet &mDescriptorSet;

        size_t mDataSize;
        uint32_t mMipLevels;


        vk::Image mImage;
        vk::DeviceMemory mMemory;
        vk::ImageView mImageView;

    public:
        VulkanHardwareBuffer(const VulkanInstance &vulkanInstance,
                             const VulkanDevice &vulkanDevice,
                             const VulkanCommandPool &commandPool,
                             AHardwareBuffer *hardwareBuffer,
                             const vk::Sampler &sampler,
                             const vk::SamplerYcbcrConversion &conversion,
                             uint32_t binding, uint32_t index, const vk::DescriptorSet &descriptorSet);

        ~VulkanHardwareBuffer() override;

        [[nodiscard]]
        const vk::ImageView &getImageView() const;

        [[nodiscard]]
        const vk::Sampler &getSampler() const;

        void updateBuffer(void *data, uint32_t size) override;
    };

} // engine
