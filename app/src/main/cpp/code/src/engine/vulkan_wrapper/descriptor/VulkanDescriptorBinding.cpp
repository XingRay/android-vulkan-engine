//
// Created by leixing on 2025/3/28.
//

#include "engine/vulkan_wrapper/descriptor/VulkanDescriptorBinding.h"

namespace engine {

    VulkanDescriptorBinding::VulkanDescriptorBinding(vk::DescriptorType descriptorType,
                                                     uint32_t descriptorOffset,
                                                     uint32_t descriptorRange)
            : mDescriptorType(descriptorType),
              mDescriptorOffset(descriptorOffset),
              mDescriptorRange(descriptorRange),
              mVulkanDescriptorBufferInfo(nullptr),
              mVulkanDescriptorImageInfo(nullptr) {}

    VulkanDescriptorBinding::~VulkanDescriptorBinding() = default;

    vk::DescriptorType VulkanDescriptorBinding::getDescriptorType() const {
        return mDescriptorType;
    }

    uint32_t VulkanDescriptorBinding::getDescriptorOffset() const {
        return mDescriptorOffset;
    }

    uint32_t VulkanDescriptorBinding::getDescriptorRange() const {
        return mDescriptorRange;
    }

    const std::unique_ptr<VulkanDescriptorBufferInfo> &VulkanDescriptorBinding::getVulkanDescriptorBufferInfo() const {
        return mVulkanDescriptorBufferInfo;
    }

    VulkanDescriptorBinding &VulkanDescriptorBinding::setVulkanDescriptorBufferInfo(std::unique_ptr<VulkanDescriptorBufferInfo> &&vulkanDescriptorBufferInfo) {
        mVulkanDescriptorBufferInfo = std::move(vulkanDescriptorBufferInfo);
        return *this;
    }

    const std::unique_ptr<VulkanDescriptorImageInfo> &VulkanDescriptorBinding::getVulkanDescriptorImageInfo() const {
        return mVulkanDescriptorImageInfo;
    }

    VulkanDescriptorBinding &VulkanDescriptorBinding::setVulkanDescriptorImageInfo(std::unique_ptr<VulkanDescriptorImageInfo> &&vulkanDescriptorImageInfo) {
        mVulkanDescriptorImageInfo = std::move(vulkanDescriptorImageInfo);
        return *this;
    }

    VulkanDescriptorBinding &VulkanDescriptorBinding::updateUniformBuffer(const VulkanCommandPool &commandPool, const void *data, uint32_t size) {
        mVulkanDescriptorBufferInfo->update(commandPool, data, size);
        return *this;
    }

} // engine