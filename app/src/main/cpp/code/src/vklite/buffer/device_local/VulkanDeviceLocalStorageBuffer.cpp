//
// Created by leixing on 2024/12/31.
//

#include "VulkanDeviceLocalStorageBuffer.h"
#include "vklite/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    VulkanDeviceLocalStorageBuffer::VulkanDeviceLocalStorageBuffer(const VulkanDevice &vulkanDevice,
                                                                   vk::DeviceSize bufferSize,
                                                                   vk::BufferUsageFlags additionalUsageFlags)
            : mVulkanDevice(vulkanDevice),
              mStagingBuffer(vulkanDevice, bufferSize),
              mStorageBuffer(vulkanDevice, bufferSize, vk::BufferUsageFlagBits::eStorageBuffer | additionalUsageFlags) {
    }

    VulkanDeviceLocalStorageBuffer::~VulkanDeviceLocalStorageBuffer() = default;

    const vk::Buffer &VulkanDeviceLocalStorageBuffer::getBuffer() const {
        return mStorageBuffer.getBuffer();
    }

    const vk::DeviceMemory &VulkanDeviceLocalStorageBuffer::getDeviceMemory() const {
        return mStorageBuffer.getDeviceMemory();
    }

    void VulkanDeviceLocalStorageBuffer::recordCommandUpdate(const vk::CommandBuffer &commandBuffer, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mStorageBuffer.recordCommandCopyFrom(commandBuffer, mStagingBuffer.getBuffer());
    }

    void VulkanDeviceLocalStorageBuffer::update(const VulkanCommandPool &vulkanCommandPool, const void *data, uint32_t size) {
        mStagingBuffer.updateBuffer(data, size);
        mStorageBuffer.copyFrom(vulkanCommandPool, mStagingBuffer.getBuffer());
    }

    std::vector<vk::DescriptorBufferInfo> VulkanDeviceLocalStorageBuffer::createDescriptorBufferInfos() {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mStorageBuffer.getBuffer())
                .setOffset(0)
                .setRange(mStorageBuffer.getBufferSize());

        std::vector<vk::DescriptorBufferInfo> descriptorBufferInfos = {descriptorBufferInfo};
        return descriptorBufferInfos;
    }

} // engine