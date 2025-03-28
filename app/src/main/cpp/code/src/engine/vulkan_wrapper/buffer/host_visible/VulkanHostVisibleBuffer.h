//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <vulkan/vulkan.hpp>

#include "engine/vulkan_wrapper/VulkanDevice.h"

namespace engine {

    /**
     * buffer visible on cpu
     */
    class VulkanHostVisibleBuffer {
    private:
        const VulkanDevice &mVulkanDevice;

        vk::Buffer mBuffer;
        vk::DeviceSize mBufferSize;
        vk::DeviceMemory mDeviceMemory;
        void *mMappedMemoryPointer;

    public:
        VulkanHostVisibleBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsageFlagBits);

        virtual  ~VulkanHostVisibleBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceSize &getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void updateBuffer(const void *data, uint32_t size);
    };

} // engine
