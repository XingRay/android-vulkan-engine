//
// Created by leixing on 2025/3/6.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanDevice.h"

namespace engine {

    class VulkanStagingBuffer {
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

        vk::Buffer mBuffer;
        vk::DeviceMemory mDeviceMemory;
        void *mMappedMemoryPointer;

    public:
        VulkanStagingBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanStagingBuffer();

        const vk::Buffer &getBuffer() const;

        void updateBuffer(void *data, uint32_t size);
    };

} // engine
