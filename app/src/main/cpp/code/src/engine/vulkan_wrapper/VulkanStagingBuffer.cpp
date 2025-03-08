//
// Created by leixing on 2025/3/6.
//

#include "VulkanStagingBuffer.h"
#include "engine/VulkanUtil.h"
#include "engine/Log.h"

namespace engine {
    VulkanStagingBuffer::VulkanStagingBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize)
            : mDevice(vulkanDevice), mBufferSize(bufferSize) {
        std::tie(mBuffer, mDeviceMemory) = VulkanUtil::createBuffer(vulkanDevice, bufferSize,
                                                                    vk::BufferUsageFlagBits::eTransferSrc,
                                                                    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        mMappedMemoryPointer = vulkanDevice.getDevice().mapMemory(mDeviceMemory, 0, bufferSize, vk::MemoryMapFlags{});
    }

    VulkanStagingBuffer::~VulkanStagingBuffer() {
        vk::Device device = mDevice.getDevice();

        device.unmapMemory(mDeviceMemory);
        device.destroy(mBuffer);
        device.freeMemory(mDeviceMemory);
        mMappedMemoryPointer = nullptr;
    }

    const vk::Buffer &VulkanStagingBuffer::getBuffer() const {
        return mBuffer;
    }

    void VulkanStagingBuffer::updateBuffer(void *data, uint32_t size){
        if (mMappedMemoryPointer == nullptr) {
            LOG_E("staging buffer memory is not mapped!");
            return;
        }
        if (data == nullptr) {
            LOG_E("Input data is null!");
            return;
        }
        if (size > mBufferSize) {
            LOG_E("Data size (%u) exceeds buffer size (%d)!", size, mBufferSize);
            return;
        }

        memcpy(mMappedMemoryPointer, data, size);
    }

} // engine