//
// Created by leixing on 2025/3/13.
//

#include "VulkanHostVisibleUniformBuffer.h"

namespace vklite {
    VulkanHostVisibleUniformBuffer::VulkanHostVisibleUniformBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize)
            : mUniformBuffer(vulkanDevice, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer) {

    }

    VulkanHostVisibleUniformBuffer::~VulkanHostVisibleUniformBuffer() = default;

    const vk::Buffer &VulkanHostVisibleUniformBuffer::getBuffer() const {
        return mUniformBuffer.getBuffer();
    }

    const vk::DeviceSize &VulkanHostVisibleUniformBuffer::getBufferSize() const {
        return mUniformBuffer.getBufferSize();
    }

    const vk::DeviceMemory &VulkanHostVisibleUniformBuffer::getDeviceMemory() const {
        return mUniformBuffer.getDeviceMemory();
    }

    void VulkanHostVisibleUniformBuffer::updateBuffer(const void *data, uint32_t size) {
        mUniformBuffer.updateBuffer(data, size);
    }
} // engine