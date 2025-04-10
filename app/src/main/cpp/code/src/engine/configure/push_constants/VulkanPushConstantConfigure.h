//
// Created by leixing on 2025/1/17.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace engine {

    class VulkanPushConstantConfigure {
    private:
        uint32_t mSize;
        uint32_t mOffset;
        vk::ShaderStageFlagBits mStageFlagBits;

    public:
        VulkanPushConstantConfigure(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits);

        ~VulkanPushConstantConfigure();

        uint32_t getSize() const;

        uint32_t getOffset() const;

        vk::ShaderStageFlagBits getStageFlagBits() const;

        vk::PushConstantRange createPushConstantRange() const;
    };

} // engine
