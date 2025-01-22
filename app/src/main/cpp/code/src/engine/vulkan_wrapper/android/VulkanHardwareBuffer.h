//
// Created by leixing on 2025/1/19.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include <android/hardware_buffer.h>

#include "engine/vulkan_wrapper/VulkanInstance.h"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanBuffer.h"

namespace engine {

    class VulkanHardwareBuffer : public VulkanBuffer {
    private:
        const VulkanInstance &mVulkanInstance;
        const VulkanDevice &mVulkanDevice;

        size_t mDataSize = 0;

        vk::Image mImage;
        vk::DeviceMemory mMemory;
        vk::ImageView mImageView;

        vk::SamplerYcbcrConversion mConversion;
        vk::Sampler mSampler;

    public:
        VulkanHardwareBuffer(const VulkanInstance &vulkanInstance, const VulkanDevice &vulkanDevice, AHardwareBuffer *hardwareBuffer,
                             uint32_t binding, uint32_t index);

        ~VulkanHardwareBuffer() override;

        const vk::ImageView &getTextureImageView() const;

        const vk::Sampler &getTextureSampler() const;

        void updateBuffer(void *data, uint32_t size) override;
    };

} // engine
