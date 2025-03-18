//
// Created by leixing on 2025/1/16.
//

#include "VulkanHostVisibleBuffer.h"

#include "engine/VulkanUtil.h"
#include "engine/Log.h"

namespace engine {

    VulkanHostVisibleBuffer::VulkanHostVisibleBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsageFlagBits)
            : mVulkanDevice(vulkanDevice), mBufferSize(bufferSize) {
        std::tie(mBuffer, mDeviceMemory) = VulkanUtil::createBuffer(vulkanDevice, bufferSize, bufferUsageFlagBits,
                                                                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        mMappedMemoryPointer = vulkanDevice.getDevice().mapMemory(mDeviceMemory, 0, bufferSize, vk::MemoryMapFlags{});
    }

    engine::VulkanHostVisibleBuffer::~VulkanHostVisibleBuffer() {
        vk::Device device = mVulkanDevice.getDevice();

        device.unmapMemory(mDeviceMemory);
        device.destroy(mBuffer);
        device.freeMemory(mDeviceMemory);
        mMappedMemoryPointer = nullptr;
    }

    const vk::Buffer &VulkanHostVisibleBuffer::getBuffer() const {
        return mBuffer;
    }

    const vk::DeviceSize &VulkanHostVisibleBuffer::getBufferSize() const {
        return mBufferSize;
    }

    const vk::DeviceMemory &VulkanHostVisibleBuffer::getDeviceMemory() const {
        return mDeviceMemory;
    }

    void VulkanHostVisibleBuffer::updateBuffer(const void *data, uint32_t size) {
        if (mMappedMemoryPointer == nullptr) {
            LOG_E("staging buffer memory is not mapped!");
            return;
        }
        if (data == nullptr) {
            LOG_E("Input data is null!");
            return;
        }
        if (size > mBufferSize) {
            LOG_E("Data size (%u) exceeds buffer size (%d)!", size, (uint32_t) mBufferSize);
            return;
        }

        memcpy(mMappedMemoryPointer, data, size);
    }

}