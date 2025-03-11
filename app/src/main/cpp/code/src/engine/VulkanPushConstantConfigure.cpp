//
// Created by leixing on 2025/1/17.
//

#include "VulkanPushConstantConfigure.h"

namespace engine {

    VulkanPushConstantConfigure::VulkanPushConstantConfigure(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits)
            : mSize(size), mOffset(offset), mStageFlagBits(stageFlagBits) {

    }

    VulkanPushConstantConfigure::~VulkanPushConstantConfigure() = default;

    uint32_t VulkanPushConstantConfigure::getSize() const {
        return mSize;
    }

    uint32_t VulkanPushConstantConfigure::getOffset() const {
        return mOffset;
    }

    vk::ShaderStageFlagBits VulkanPushConstantConfigure::getStageFlagBits() const {
        return mStageFlagBits;
    }

    vk::PushConstantRange VulkanPushConstantConfigure::createPushConstantRange() const {
        if (mSize == 0) {
            throw std::runtime_error("VulkanPushConstantConfigure::createPushConstantRange(), mSize == 0 ");
        }
        vk::PushConstantRange pushConstantRange{};
        pushConstantRange
                .setStageFlags(mStageFlagBits)
                .setOffset(mOffset)
                .setSize(mSize);
        return pushConstantRange;
    }
} // engine