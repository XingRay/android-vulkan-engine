//
// Created by leixing on 2025/1/17.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace vklite {

    class PushConstantConfigure {
    private:
        uint32_t mSize;
        uint32_t mOffset;
        vk::ShaderStageFlagBits mStageFlagBits;

    public:
        PushConstantConfigure(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits);

        ~PushConstantConfigure();

        uint32_t getSize() const;

        uint32_t getOffset() const;

        vk::ShaderStageFlagBits getStageFlagBits() const;

        vk::PushConstantRange createPushConstantRange() const;
    };

} // engine
