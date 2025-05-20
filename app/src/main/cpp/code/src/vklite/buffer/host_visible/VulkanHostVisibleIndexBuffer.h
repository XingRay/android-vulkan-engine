//
// Created by leixing on 2025/3/13.
//

#pragma once

#include <vector>
#include "VulkanHostVisibleBuffer.h"

namespace vklite {

    class VulkanHostVisibleIndexBuffer {
    private:
        VulkanHostVisibleBuffer mIndexBuffer;
        uint32_t mIndicesCount;

    public:
        VulkanHostVisibleIndexBuffer(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize);

        ~VulkanHostVisibleIndexBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceSize &getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        uint32_t getIndicesCount() const;

        void update(const std::vector<uint32_t>& indices);
    };

} // engine
