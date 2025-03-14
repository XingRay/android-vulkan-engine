//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <vulkan/vulkan.hpp>

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

namespace engine {

    /**
     * buffer on gpu, and invisible on cpu.
     */
    class VulkanDeviceLocalBuffer {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Buffer mBuffer;
        vk::DeviceSize mBufferSize;
        vk::DeviceMemory mDeviceMemory;

    public:
        VulkanDeviceLocalBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsageFlagBits);

        virtual  ~VulkanDeviceLocalBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceSize &getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset);

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer);

        void copyFrom(const VulkanCommandPool &vulkanCommandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset);

        void copyFrom(const VulkanCommandPool &vulkanCommandPool, vk::Buffer srcBuffer);
    };

} // engine
