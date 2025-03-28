//
// Created by leixing on 2025/3/18.
//

#pragma once

#include <cstdint>

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanBufferView {
    private:
        const vk::Buffer &mBuffer;
        uint32_t mOffset;
        uint32_t mRange;

    public:
        VulkanBufferView(const vk::Buffer &buffer, uint32_t offset, uint32_t range);

        VulkanBufferView(const vk::Buffer &buffer, uint32_t range);

        ~VulkanBufferView();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        uint32_t getOffset() const;

        [[nodiscard]]
        uint32_t getRange() const;

    };

} // engine
