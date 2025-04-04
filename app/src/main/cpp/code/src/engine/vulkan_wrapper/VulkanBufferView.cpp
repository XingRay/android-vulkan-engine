//
// Created by leixing on 2025/3/18.
//

#include "engine/vulkan_wrapper/VulkanBufferView.h"

namespace engine {

    VulkanBufferView::VulkanBufferView(const std::shared_ptr<VulkanBufferInterface> &vulkanBuffer, uint32_t offset, uint32_t range)
            : mVulkanBuffer(vulkanBuffer), mOffset(offset), mRange(range) {}

    VulkanBufferView::VulkanBufferView(const std::shared_ptr<VulkanBufferInterface> &buffer, uint32_t range)
            : VulkanBufferView(buffer, 0, range) {}

    VulkanBufferView::VulkanBufferView(const std::shared_ptr<VulkanBufferInterface> &buffer)
            : VulkanBufferView(buffer, 0, buffer->getSize()) {}

    VulkanBufferView::~VulkanBufferView() = default;

    const std::shared_ptr<VulkanBufferInterface> &VulkanBufferView::getVulkanBuffer() const {
        return mVulkanBuffer;
    }

    uint32_t VulkanBufferView::getOffset() const {
        return mOffset;
    }

    uint32_t VulkanBufferView::getRange() const {
        return mRange;
    }

} // engine