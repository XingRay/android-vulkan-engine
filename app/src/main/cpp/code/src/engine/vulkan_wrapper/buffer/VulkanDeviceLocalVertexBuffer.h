//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "engine/vulkan_wrapper/buffer/VulkanDeviceLocalBuffer.h"
#include "engine/vulkan_wrapper/buffer/VulkanStagingBuffer.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

namespace engine {

    class VulkanDeviceLocalVertexBuffer {
    private:
        VulkanDeviceLocalBuffer mVertexBuffer;
        VulkanStagingBuffer mStagingBuffer;

    public:
        VulkanDeviceLocalVertexBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanDeviceLocalVertexBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size);

        void update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size);

    };

} // engine
