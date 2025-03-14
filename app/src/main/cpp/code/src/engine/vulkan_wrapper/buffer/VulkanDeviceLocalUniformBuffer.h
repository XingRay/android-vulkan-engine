//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "engine/vulkan_wrapper/buffer/VulkanDeviceLocalBuffer.h"
#include "engine/vulkan_wrapper/buffer/VulkanStagingBuffer.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

namespace engine {

    class VulkanDeviceLocalUniformBuffer {
    private:
        VulkanDeviceLocalBuffer mUniformBuffer;
        VulkanStagingBuffer mStagingBuffer;

    public:
        VulkanDeviceLocalUniformBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanDeviceLocalUniformBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandUpdate(const vk::CommandBuffer& commandBuffer, void *data, uint32_t size);

        void update(const VulkanCommandPool& vulkanCommandPool, void *data, uint32_t size);

        std::vector<vk::DescriptorBufferInfo> createDescriptorBufferInfos();
    };

} // engine
