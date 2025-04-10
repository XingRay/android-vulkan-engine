//
// Created by leixing on 2025/3/18.
//

#include "VulkanDescriptorBufferInfo.h"

namespace engine {

    VulkanDescriptorBufferInfo::VulkanDescriptorBufferInfo(const std::shared_ptr<VulkanBufferInterface> &vulkanBuffer, uint32_t offset, uint32_t range)
            : mVulkanBuffer(vulkanBuffer), mOffset(offset), mRange(range) {}

    VulkanDescriptorBufferInfo::VulkanDescriptorBufferInfo(const std::shared_ptr<VulkanBufferInterface> &buffer, uint32_t range)
            : VulkanDescriptorBufferInfo(buffer, 0, range) {}

    VulkanDescriptorBufferInfo::VulkanDescriptorBufferInfo(const std::shared_ptr<VulkanBufferInterface> &buffer)
            : VulkanDescriptorBufferInfo(buffer, 0, buffer->getSize()) {}

    VulkanDescriptorBufferInfo::~VulkanDescriptorBufferInfo() = default;

    const std::shared_ptr<VulkanBufferInterface> &VulkanDescriptorBufferInfo::getVulkanBuffer() const {
        return mVulkanBuffer;
    }

    uint32_t VulkanDescriptorBufferInfo::getOffset() const {
        return mOffset;
    }

    uint32_t VulkanDescriptorBufferInfo::getRange() const {
        return mRange;
    }

    void VulkanDescriptorBufferInfo::update(const VulkanCommandPool &commandPool, const void *data, uint32_t size) {
        mVulkanBuffer->update(commandPool, data, size);
    }

    [[nodiscard]]
    vk::DescriptorBufferInfo VulkanDescriptorBufferInfo::createDescriptorBufferInfo() const {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mVulkanBuffer->getBuffer())
                .setOffset(mOffset)
                .setRange(mRange);
        return descriptorBufferInfo;
    }

} // engine