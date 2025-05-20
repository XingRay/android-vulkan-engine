//
// Created by leixing on 2025/3/18.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"
#include "vklite/buffer/VulkanBufferInterface.h"
#include "vklite/command/VulkanCommandPool.h"

namespace vklite {

    class BufferInfo {
    private:
        std::shared_ptr<VulkanBufferInterface> mVulkanBuffer;
        uint32_t mOffset;
        uint32_t mRange;

    public:
        BufferInfo(const std::shared_ptr<VulkanBufferInterface> &buffer, uint32_t offset, uint32_t range);

        BufferInfo(const std::shared_ptr<VulkanBufferInterface> &buffer, uint32_t range);

        explicit BufferInfo(const std::shared_ptr<VulkanBufferInterface> &buffer);

        ~BufferInfo();

        [[nodiscard]]
        const std::shared_ptr<VulkanBufferInterface> &getVulkanBuffer() const;

        [[nodiscard]]
        uint32_t getOffset() const;

        [[nodiscard]]
        uint32_t getRange() const;

        void update(const VulkanCommandPool &commandPool, const void *data, uint32_t size);

        [[nodiscard]]
        vk::DescriptorBufferInfo createDescriptorBufferInfo() const;
    };

} // engine
