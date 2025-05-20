//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "VulkanHostVisibleBuffer.h"

namespace vklite {

    class VulkanHostVisibleUniformBuffer {
    private:
        VulkanHostVisibleBuffer mUniformBuffer;

    public:
        VulkanHostVisibleUniformBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanHostVisibleUniformBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceSize &getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void updateBuffer(const void *data, uint32_t size);
    };

} // engine
