//
// Created by leixing on 2025/3/10.
//

#include "PushConstant.h"

namespace vklite {
    PushConstant::PushConstant(uint32_t size)
            : mPushConstants(size) {

    }

    PushConstant::~PushConstant() = default;

    void PushConstant::update(const void *data, uint32_t size) {
        std::memcpy(mPushConstants.data(), data, size);
    }

    vk::ShaderStageFlags PushConstant::getStageFlags() const {
        return mStageFlags;
    }

    uint32_t PushConstant::getOffset() const {
        return mOffset;
    }

    uint32_t PushConstant::getSize() const {
        return mSize;
    }

    const void *PushConstant::getData() const {
        return mData;
    }
} // engine