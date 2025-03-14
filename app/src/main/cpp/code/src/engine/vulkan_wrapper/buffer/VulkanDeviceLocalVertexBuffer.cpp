//
// Created by leixing on 2025/3/13.
//

#include "VulkanDeviceLocalVertexBuffer.h"

namespace engine {
    VulkanDeviceLocalVertexBuffer::VulkanDeviceLocalVertexBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize)
            : mVertexBuffer(vulkanDevice, bufferSize, vk::BufferUsageFlagBits::eVertexBuffer),
              mStagingBuffer(vulkanDevice, bufferSize){


    }

    VulkanDeviceLocalVertexBuffer::~VulkanDeviceLocalVertexBuffer() =default;

    const vk::Buffer &VulkanDeviceLocalVertexBuffer::getBuffer() const {
        return mVertexBuffer.getBuffer();
    }

    const vk::DeviceMemory &VulkanDeviceLocalVertexBuffer::getDeviceMemory() const {
        return mVertexBuffer.getDeviceMemory();
    }

    void VulkanDeviceLocalVertexBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mVertexBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void VulkanDeviceLocalVertexBuffer::update(const VulkanCommandPool &vulkanCommandPool, void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mVertexBuffer.copyFrom(vulkanCommandPool, mStagingBuffer.getBuffer());
    }
} // engine