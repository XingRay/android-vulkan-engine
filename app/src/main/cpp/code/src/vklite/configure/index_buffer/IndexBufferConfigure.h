//
// Created by leixing on 2025/3/22.
//

#pragma once

#include <vector>

#include "vklite/buffer/device_local/VulkanDeviceLocalIndexBuffer.h"

namespace vklite {

    class IndexBufferConfigure {
    private:
        std::shared_ptr<VulkanDeviceLocalIndexBuffer> mIndexBuffer;

        uint32_t mCreateBufferCapacity;
        std::vector<uint32_t> mCreateBufferIndices;

    public:
        IndexBufferConfigure();

        ~IndexBufferConfigure();

        IndexBufferConfigure &setIndexBuffer(uint32_t capacity);

        IndexBufferConfigure &setIndexBuffer(uint32_t capacity, std::vector<uint32_t> &&indices);

        IndexBufferConfigure &setIndexBuffer(std::vector<uint32_t> &&indices);

        IndexBufferConfigure &setIndexBuffer(const std::shared_ptr<VulkanDeviceLocalIndexBuffer>& indexBuffer);

        [[nodiscard]]
        std::shared_ptr<VulkanDeviceLocalIndexBuffer> createIndexBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const;
    };

} // engine
