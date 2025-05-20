//
// Created by leixing on 2025/1/16.
//

#include "VulkanDeviceLocalBuffer.h"

#include "vklite/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    VulkanDeviceLocalBuffer::VulkanDeviceLocalBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize, vk::BufferUsageFlags bufferUsageFlagBits)
            : mVulkanDevice(vulkanDevice), mBufferSize(bufferSize) {
        std::tie(mBuffer, mDeviceMemory) = VulkanUtil::createBuffer(vulkanDevice, bufferSize, vk::BufferUsageFlagBits::eTransferDst | bufferUsageFlagBits,
                                                                    vk::MemoryPropertyFlagBits::eDeviceLocal);
    }

    vklite::VulkanDeviceLocalBuffer::~VulkanDeviceLocalBuffer() {
        vk::Device device = mVulkanDevice.getDevice();

        device.unmapMemory(mDeviceMemory);
        device.destroy(mBuffer);
        device.freeMemory(mDeviceMemory);
    }

    const vk::Buffer &VulkanDeviceLocalBuffer::getBuffer() const {
        return mBuffer;
    }

    vk::DeviceSize VulkanDeviceLocalBuffer::getBufferSize() const {
        return mBufferSize;
    }

    const vk::DeviceMemory &VulkanDeviceLocalBuffer::getDeviceMemory() const {
        return mDeviceMemory;
    }

    void VulkanDeviceLocalBuffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize dstOffset, vk::DeviceSize copyDataSize) {
        vk::BufferCopy bufferCopy;
        bufferCopy
                .setSrcOffset(srcOffset)
                .setDstOffset(dstOffset)
                .setSize(copyDataSize);
        commandBuffer.copyBuffer(srcBuffer, mBuffer, bufferCopy);
    }

    void VulkanDeviceLocalBuffer::recordCommandCopyFrom(const vk::CommandBuffer &commandBuffer, vk::Buffer srcBuffer) {
        recordCommandCopyFrom(commandBuffer, srcBuffer, 0, 0, mBufferSize);
    }

    void VulkanDeviceLocalBuffer::copyFrom(const VulkanCommandPool &vulkanCommandPool, vk::Buffer srcBuffer, vk::DeviceSize srcOffset, vk::DeviceSize copyDataSize, vk::DeviceSize dstOffset) {
        vulkanCommandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFrom(commandBuffer, srcBuffer, srcOffset, copyDataSize, dstOffset);
        });
    }

    void VulkanDeviceLocalBuffer::copyFrom(const VulkanCommandPool &vulkanCommandPool, vk::Buffer srcBuffer) {
        vulkanCommandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer) {
            recordCommandCopyFrom(commandBuffer, srcBuffer);
        });
    }
}