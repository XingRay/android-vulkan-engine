//
// Created by leixing on 2024/12/31.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

#include "engine/vulkan_wrapper/buffer/VulkanHostVisibleBuffer.h"
#include "VulkanStagingBuffer.h"

namespace engine {

    class VulkanStorageBuffer : public VulkanHostVisibleBuffer {
    private:
        const VulkanDevice &mDevice;
        /**
         * 指定缓冲区的范围（以字节为单位）。
         */
        uint32_t mBufferSize;

        /**
         *  指定缓冲区的起始偏移量（以字节为单位）。
         */
        uint32_t mOffset = 0;

        std::unique_ptr<VulkanStagingBuffer> mVulkanStagingBuffer;

        vk::Buffer mBuffer;
        vk::DeviceMemory mDeviceMemory;
        void *mMappedMemoryPointer;

        const VulkanCommandPool &mCommandPool;

    public:
        VulkanStorageBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &vulkanCommandPool,
                            vk::DeviceSize bufferSize, uint32_t binding, uint32_t index);

        ~VulkanStorageBuffer() override;

        [[nodiscard]]
        const vk::Buffer &getUniformBuffer() const;

        [[nodiscard]]
        const vk::DeviceMemory &getUniformBufferMemory() const;

        [[nodiscard]]
        uint32_t getBufferSize() const;

        [[nodiscard]]
        uint32_t getOffset() const;

        void updateBuffer(void *data, uint32_t size) override;
    };

} // engine
