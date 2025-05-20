//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <vulkan/vulkan.hpp>

#include "vklite/device/VulkanDevice.h"
#include "vklite/command/VulkanCommandPool.h"

namespace vklite {

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
        VulkanDeviceLocalBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize, vk::BufferUsageFlags bufferUsageFlagBits);

        virtual  ~VulkanDeviceLocalBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        vk::DeviceSize getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize);

        void recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer);

        void copyFrom(const VulkanCommandPool &vulkanCommandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset);

        void copyFrom(const VulkanCommandPool &vulkanCommandPool, vk::Buffer srcBuffer);
    };

} // engine
