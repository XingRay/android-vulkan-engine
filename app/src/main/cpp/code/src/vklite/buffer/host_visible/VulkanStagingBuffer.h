//
// Created by leixing on 2025/3/6.
//

#pragma once

#include "VulkanHostVisibleBuffer.h"

namespace vklite {

    class VulkanStagingBuffer {
    private:
        VulkanHostVisibleBuffer mVulkanHostVisibleBuffer;

    public:
        VulkanStagingBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanStagingBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        void updateBuffer(const void *data, uint32_t size);
    };

} // engine
