//
// Created by leixing on 2025/3/6.
//

#pragma once

#include "engine/vulkan_wrapper/buffer/VulkanHostVisibleBuffer.h"

namespace engine {

    class VulkanStagingBuffer {
    private:
        VulkanHostVisibleBuffer mVulkanHostVisibleBuffer;

    public:
        VulkanStagingBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanStagingBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        void updateBuffer(void *data, uint32_t size);
    };

} // engine
