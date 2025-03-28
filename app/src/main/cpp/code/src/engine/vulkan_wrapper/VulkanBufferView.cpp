//
// Created by leixing on 2025/3/18.
//

#include "VulkanBufferView.h"

namespace engine {

    VulkanBufferView::VulkanBufferView(const vk::Buffer &buffer, uint32_t offset, uint32_t range) :
            mBuffer(buffer), mOffset(offset), mRange(range) {

    }

    VulkanBufferView::VulkanBufferView(const vk::Buffer &buffer, uint32_t range) :
            VulkanBufferView(buffer, 0, range) {

    }

    VulkanBufferView::~VulkanBufferView() = default;

    const vk::Buffer &VulkanBufferView::getBuffer() const {
        return mBuffer;
    }

    uint32_t VulkanBufferView::getOffset() const {
        return mOffset;
    }

    uint32_t VulkanBufferView::getRange() const {
        return mRange;
    }

} // engine