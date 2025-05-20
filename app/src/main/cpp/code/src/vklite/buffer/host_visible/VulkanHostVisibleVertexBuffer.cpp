//
// Created by leixing on 2025/3/13.
//

#include "VulkanHostVisibleVertexBuffer.h"

namespace vklite {
    VulkanHostVisibleVertexBuffer::VulkanHostVisibleVertexBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize)
            : mVertexBuffer(vulkanDevice, bufferSize, vk::BufferUsageFlagBits::eVertexBuffer) {

    }

    VulkanHostVisibleVertexBuffer::~VulkanHostVisibleVertexBuffer() = default;

    const vk::Buffer &VulkanHostVisibleVertexBuffer::getBuffer() const {
        return mVertexBuffer.getBuffer();
    }

    const vk::DeviceSize &VulkanHostVisibleVertexBuffer::getBufferSize() const {
        return mVertexBuffer.getBufferSize();
    }

    const vk::DeviceMemory &VulkanHostVisibleVertexBuffer::getDeviceMemory() const {
        return mVertexBuffer.getDeviceMemory();
    }

    void VulkanHostVisibleVertexBuffer::updateBuffer(const void *data, uint32_t size) {
        mVertexBuffer.updateBuffer(data, size);
    }

} // engine