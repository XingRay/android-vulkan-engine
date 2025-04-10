//
// Created by leixing on 2025/3/28.
//

#include "VulkanImageDescriptorBinding.h"

namespace engine {

    VulkanImageDescriptorBinding::VulkanImageDescriptorBinding(vk::DescriptorType descriptorType,
                                                               uint32_t descriptorOffset,
                                                               uint32_t descriptorRange)
            : mDescriptorType(descriptorType),
              mDescriptorOffset(descriptorOffset),
              mDescriptorRange(descriptorRange),
              mVulkanDescriptorImageInfo(nullptr) {

    }

    VulkanImageDescriptorBinding::~VulkanImageDescriptorBinding() = default;

    const std::unique_ptr<VulkanDescriptorImageInfo> &VulkanImageDescriptorBinding::getVulkanDescriptorImageInfo() const {
        return mVulkanDescriptorImageInfo;
    }

    vk::DescriptorType VulkanImageDescriptorBinding::getDescriptorType() const {
        return mDescriptorType;
    }

    uint32_t VulkanImageDescriptorBinding::getDescriptorOffset() const {
        return mDescriptorOffset;
    }

    uint32_t VulkanImageDescriptorBinding::getDescriptorRange() const {
        return mDescriptorRange;
    }

    VulkanImageDescriptorBinding &VulkanImageDescriptorBinding::setVulkanDescriptorImageInfo(std::unique_ptr<VulkanDescriptorImageInfo> &&imageInfo) {
        mVulkanDescriptorImageInfo = std::move(imageInfo);
        return *this;
    }

} // engine