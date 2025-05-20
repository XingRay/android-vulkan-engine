//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <cstdint>
#include <vector>

#include "vulkan/vulkan.hpp"

namespace vklite {

    class PushConstant {
    private:
        std::vector<uint8_t> mPushConstants;

        vk::ShaderStageFlags mStageFlags;
        uint32_t mOffset;
        uint32_t mSize;
        const void *mData;

    public:
        explicit PushConstant(uint32_t size);

        ~PushConstant();

        void update(const void *data, uint32_t size);

        [[nodiscard]]
        vk::ShaderStageFlags getStageFlags() const;

        [[nodiscard]]
        uint32_t getOffset() const;

        [[nodiscard]]
        uint32_t getSize() const;

        [[nodiscard]]
        const void *getData() const;
    };

} // engine
