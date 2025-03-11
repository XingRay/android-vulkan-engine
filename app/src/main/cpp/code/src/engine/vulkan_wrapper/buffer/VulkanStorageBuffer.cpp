//
// Created by leixing on 2024/12/31.
//

#include "VulkanStorageBuffer.h"
#include "engine/VulkanUtil.h"
#include "engine/Log.h"

namespace engine {
    VulkanStorageBuffer::VulkanStorageBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &vulkanCommandPool,
                                             vk::DeviceSize bufferSize, uint32_t binding, uint32_t index)
            : mDevice(vulkanDevice), mCommandPool(vulkanCommandPool), mBufferSize(bufferSize), VulkanBuffer(binding, VulkanBufferType::NORMAL, index) {
        LOG_D("VulkanUniformBuffer::VulkanUniformBuffer#bufferSize: %lu", bufferSize);

        mVulkanStagingBuffer = std::make_unique<VulkanStagingBuffer>(vulkanDevice, bufferSize);

        std::tie(mBuffer, mDeviceMemory) = VulkanUtil::createBuffer(vulkanDevice, bufferSize,
                                                                    vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                                                                    vk::MemoryPropertyFlagBits::eDeviceLocal);
        mMappedMemoryPointer = vulkanDevice.getDevice().mapMemory(mDeviceMemory, 0, bufferSize, vk::MemoryMapFlags{});
    }

    VulkanStorageBuffer::~VulkanStorageBuffer() {
        const vk::Device device = mDevice.getDevice();
        device.unmapMemory(mDeviceMemory);
        device.freeMemory(mDeviceMemory);
        device.destroy(mBuffer);
        mMappedMemoryPointer = nullptr;
    }

    const vk::Buffer &VulkanStorageBuffer::getUniformBuffer() const {
        return mBuffer;
    }

    const vk::DeviceMemory &VulkanStorageBuffer::getUniformBufferMemory() const {
        return mDeviceMemory;
    }

    uint32_t VulkanStorageBuffer::getBufferSize() const {
        return mBufferSize;
    }

    uint32_t VulkanStorageBuffer::getOffset() const {
        return mOffset;
    }

    void VulkanStorageBuffer::updateBuffer(void *data, uint32_t size) {
        mVulkanStagingBuffer->updateBuffer(data, size);
        mCommandPool.submitOneTimeCommand([&](const vk::CommandBuffer &commandBuffer){
            VulkanUtil::recordCopyBufferCommand(commandBuffer, mVulkanStagingBuffer->getBuffer(), mBuffer, mBufferSize);
        });
    }

} // engine