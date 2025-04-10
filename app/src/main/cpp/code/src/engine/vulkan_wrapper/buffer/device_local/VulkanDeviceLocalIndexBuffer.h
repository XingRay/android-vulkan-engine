//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "VulkanDeviceLocalBuffer.h"
#include "engine/vulkan_wrapper/buffer/host_visible/VulkanStagingBuffer.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

namespace engine {

    class VulkanDeviceLocalIndexBuffer {
    private:
        VulkanDeviceLocalBuffer mIndexBuffer;
        uint32_t mIndicesCount;

        VulkanStagingBuffer mStagingBuffer;

    public:
        VulkanDeviceLocalIndexBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanDeviceLocalIndexBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        uint32_t getIndicesCount() const;

        void recordCommandUpdate(const vk::CommandBuffer& commandBuffer, const std::vector<uint32_t>& indices);

        void update(const VulkanCommandPool& vulkanCommandPool, const std::vector<uint32_t>& indices);
    };

} // engine
