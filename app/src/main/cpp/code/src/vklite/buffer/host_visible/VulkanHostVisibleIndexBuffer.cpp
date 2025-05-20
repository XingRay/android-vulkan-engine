//
// Created by leixing on 2025/3/13.
//

#include "VulkanHostVisibleIndexBuffer.h"

namespace vklite {
    VulkanHostVisibleIndexBuffer::VulkanHostVisibleIndexBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize)
            : mIndexBuffer(vulkanDevice, bufferSize, vk::BufferUsageFlagBits::eIndexBuffer) {

    }

    VulkanHostVisibleIndexBuffer::~VulkanHostVisibleIndexBuffer() = default;

    [[nodiscard]]
    const vk::Buffer &VulkanHostVisibleIndexBuffer::getBuffer() const {
        return mIndexBuffer.getBuffer();
    }

    [[nodiscard]]
    const vk::DeviceSize &VulkanHostVisibleIndexBuffer::getBufferSize() const {
        return mIndexBuffer.getBufferSize();
    }

    [[nodiscard]]
    const vk::DeviceMemory &VulkanHostVisibleIndexBuffer::getDeviceMemory() const {
        return mIndexBuffer.getDeviceMemory();
    }

    uint32_t VulkanHostVisibleIndexBuffer::getIndicesCount() const {
        return mIndicesCount;
    }

    void VulkanHostVisibleIndexBuffer::update(const std::vector<uint32_t>& indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mIndexBuffer.updateBuffer(indices.data(), size);
    }

} // engine