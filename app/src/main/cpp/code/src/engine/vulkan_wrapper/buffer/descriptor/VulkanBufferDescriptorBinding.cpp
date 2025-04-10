//
// Created by leixing on 2025/3/28.
//

#include "VulkanBufferDescriptorBinding.h"

namespace engine {

    VulkanBufferDescriptorBinding::VulkanBufferDescriptorBinding(vk::DescriptorType descriptorType,
                                                                 uint32_t descriptorOffset,
                                                                 uint32_t descriptorRange)
            : mDescriptorType(descriptorType),
              mDescriptorOffset(descriptorOffset),
              mDescriptorRange(descriptorRange),
              mVulkanDescriptorBufferInfo(nullptr) {}

    VulkanBufferDescriptorBinding::~VulkanBufferDescriptorBinding() = default;

    const std::unique_ptr<VulkanDescriptorBufferInfo> &VulkanBufferDescriptorBinding::getVulkanDescriptorBufferInfo() const {
        return mVulkanDescriptorBufferInfo;
    }

    vk::DescriptorType VulkanBufferDescriptorBinding::getDescriptorType() const {
        return mDescriptorType;
    }

    uint32_t VulkanBufferDescriptorBinding::getDescriptorOffset() const {
        return mDescriptorOffset;
    }

    uint32_t VulkanBufferDescriptorBinding::getDescriptorRange() const {
        return mDescriptorRange;
    }

    VulkanBufferDescriptorBinding &VulkanBufferDescriptorBinding::setBufferInfo(std::unique_ptr<VulkanDescriptorBufferInfo> &&vulkanDescriptorBufferInfo) {
        mVulkanDescriptorBufferInfo = std::move(vulkanDescriptorBufferInfo);
        return *this;
    }

} // engine