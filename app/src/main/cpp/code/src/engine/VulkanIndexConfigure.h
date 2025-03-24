//
// Created by leixing on 2025/3/22.
//

#pragma once

#include <vector>

#include "engine/vulkan_wrapper/buffer/VulkanDeviceLocalIndexBuffer.h"

namespace engine {

    class VulkanIndexConfigure {
    private:
        std::shared_ptr<VulkanDeviceLocalIndexBuffer> mIndexBuffer;

        uint32_t mCreateBufferCapacity;
        std::vector<uint32_t> mCreateBufferIndices;

    public:
        VulkanIndexConfigure();

        ~VulkanIndexConfigure();

        VulkanIndexConfigure &setIndexBuffer(uint32_t capacity);

        VulkanIndexConfigure &setIndexBuffer(uint32_t capacity, std::vector<uint32_t> &&indices);

        VulkanIndexConfigure &setIndexBuffer(std::vector<uint32_t> &&indices);

        VulkanIndexConfigure &setIndexBuffer(const std::shared_ptr<VulkanDeviceLocalIndexBuffer>& indexBuffer);

        [[nodiscard]]
        std::shared_ptr<VulkanDeviceLocalIndexBuffer> createVertexBuffer(const VulkanDevice &vulkanDevice, const VulkanCommandPool &commandPool) const;
    };

} // engine
