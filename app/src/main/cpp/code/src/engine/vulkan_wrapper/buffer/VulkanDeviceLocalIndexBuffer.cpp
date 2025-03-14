//
// Created by leixing on 2025/3/13.
//

#include "VulkanDeviceLocalIndexBuffer.h"

namespace engine {

    VulkanDeviceLocalIndexBuffer::VulkanDeviceLocalIndexBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize)
            : mIndexBuffer(vulkanDevice, bufferSize, vk::BufferUsageFlagBits::eIndexBuffer),
              mStagingBuffer(vulkanDevice, bufferSize) {

    }

    VulkanDeviceLocalIndexBuffer::~VulkanDeviceLocalIndexBuffer() = default;

    [[nodiscard]]
    const vk::Buffer &VulkanDeviceLocalIndexBuffer::getBuffer() const {
        return mIndexBuffer.getBuffer();
    }

    [[nodiscard]]
    const vk::DeviceMemory &VulkanDeviceLocalIndexBuffer::getDeviceMemory() const {
        return mIndexBuffer.getDeviceMemory();
    }

    [[nodiscard]]
    uint32_t VulkanDeviceLocalIndexBuffer::getIndicesCount() const {
        return mIndicesCount;
    }

    void VulkanDeviceLocalIndexBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, std::vector<uint32_t> indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mStagingBuffer.updateBuffer(indices.data(), size);
        mIndexBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void VulkanDeviceLocalIndexBuffer::update(const VulkanCommandPool &vulkanCommandPool, std::vector<uint32_t> indices) {
        size_t size = indices.size() * sizeof(uint32_t);
        mIndicesCount = indices.size();

        mStagingBuffer.updateBuffer(indices.data(), size);
        mIndexBuffer.copyFrom(vulkanCommandPool, mStagingBuffer.getBuffer());
    }

} // engine