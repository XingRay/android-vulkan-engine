//
// Created by leixing on 2025/3/18.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"
#include "engine/vulkan_wrapper/VulkanBufferInterface.h"
#include "engine/vulkan_wrapper/VulkanCommandPool.h"

namespace engine {

    class VulkanBufferView {
    private:
        std::shared_ptr<VulkanBufferInterface> mVulkanBuffer;
        uint32_t mOffset;
        uint32_t mRange;

    public:
        VulkanBufferView(const std::shared_ptr<VulkanBufferInterface> &buffer, uint32_t offset, uint32_t range);

        VulkanBufferView(const std::shared_ptr<VulkanBufferInterface> &buffer, uint32_t range);

        explicit VulkanBufferView(const std::shared_ptr<VulkanBufferInterface> &buffer);

        ~VulkanBufferView();

        [[nodiscard]]
        const std::shared_ptr<VulkanBufferInterface> &getVulkanBuffer() const;

        [[nodiscard]]
        uint32_t getOffset() const;

        [[nodiscard]]
        uint32_t getRange() const;

        void update(const VulkanCommandPool& commandPool, const void *data, uint32_t size);
    };

} // engine
