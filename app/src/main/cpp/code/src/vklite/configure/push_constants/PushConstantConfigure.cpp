//
// Created by leixing on 2025/1/17.
//

#include "PushConstantConfigure.h"

namespace vklite {

    PushConstantConfigure::PushConstantConfigure(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits)
            : mSize(size), mOffset(offset), mStageFlagBits(stageFlagBits) {

    }

    PushConstantConfigure::~PushConstantConfigure() = default;

    uint32_t PushConstantConfigure::getSize() const {
        return mSize;
    }

    uint32_t PushConstantConfigure::getOffset() const {
        return mOffset;
    }

    vk::ShaderStageFlagBits PushConstantConfigure::getStageFlagBits() const {
        return mStageFlagBits;
    }

    vk::PushConstantRange PushConstantConfigure::createPushConstantRange() const {
        if (mSize == 0) {
            throw std::runtime_error("PushConstantConfigure::createPushConstantRange(), mSize == 0 ");
        }
        vk::PushConstantRange pushConstantRange{};
        pushConstantRange
                .setStageFlags(mStageFlagBits)
                .setOffset(mOffset)
                .setSize(mSize);
        return pushConstantRange;
    }
} // engine