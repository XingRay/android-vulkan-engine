//
// Created by leixing on 2025/3/18.
//

#include "BufferInfo.h"

namespace vklite {

    BufferInfo::BufferInfo(const std::shared_ptr<VulkanBufferInterface> &vulkanBuffer, uint32_t offset, uint32_t range)
            : mVulkanBuffer(vulkanBuffer), mOffset(offset), mRange(range) {}

    BufferInfo::BufferInfo(const std::shared_ptr<VulkanBufferInterface> &buffer, uint32_t range)
            : BufferInfo(buffer, 0, range) {}

    BufferInfo::BufferInfo(const std::shared_ptr<VulkanBufferInterface> &buffer)
            : BufferInfo(buffer, 0, buffer->getSize()) {}

    BufferInfo::~BufferInfo() = default;

    const std::shared_ptr<VulkanBufferInterface> &BufferInfo::getVulkanBuffer() const {
        return mVulkanBuffer;
    }

    uint32_t BufferInfo::getOffset() const {
        return mOffset;
    }

    uint32_t BufferInfo::getRange() const {
        return mRange;
    }

    void BufferInfo::update(const VulkanCommandPool &commandPool, const void *data, uint32_t size) {
        mVulkanBuffer->update(commandPool, data, size);
    }

    [[nodiscard]]
    vk::DescriptorBufferInfo BufferInfo::createDescriptorBufferInfo() const {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mVulkanBuffer->getBuffer())
                .setOffset(mOffset)
                .setRange(mRange);
        return descriptorBufferInfo;
    }

} // engine