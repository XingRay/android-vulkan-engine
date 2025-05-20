//
// Created by leixing on 2025/3/13.
//

#include "VulkanDeviceLocalUniformBuffer.h"

namespace vklite {

    VulkanDeviceLocalUniformBuffer::VulkanDeviceLocalUniformBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize)
            : mUniformBuffer(vulkanDevice, bufferSize, vk::BufferUsageFlagBits::eUniformBuffer),
              mStagingBuffer(vulkanDevice, bufferSize) {

    }

    VulkanDeviceLocalUniformBuffer::~VulkanDeviceLocalUniformBuffer() = default;

    vk::DeviceSize VulkanDeviceLocalUniformBuffer::getSize() const {
        return mUniformBuffer.getBufferSize();
    }

    const vk::Buffer &VulkanDeviceLocalUniformBuffer::getBuffer() const {
        return mUniformBuffer.getBuffer();
    }

    const vk::DeviceMemory &VulkanDeviceLocalUniformBuffer::getDeviceMemory() const {
        return mUniformBuffer.getDeviceMemory();
    }

    void VulkanDeviceLocalUniformBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mUniformBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void VulkanDeviceLocalUniformBuffer::update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mUniformBuffer.copyFrom(vulkanCommandPool, mStagingBuffer.getBuffer());
    }

    std::vector<vk::DescriptorBufferInfo> VulkanDeviceLocalUniformBuffer::createDescriptorBufferInfos() {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mUniformBuffer.getBuffer())
                .setOffset(0)
                .setRange(mUniformBuffer.getBufferSize());

        std::vector<vk::DescriptorBufferInfo> descriptorBufferInfos = {descriptorBufferInfo};
        return descriptorBufferInfos;
    }

} // engine