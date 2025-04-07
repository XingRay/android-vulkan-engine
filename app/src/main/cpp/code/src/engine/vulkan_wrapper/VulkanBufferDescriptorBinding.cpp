//
// Created by leixing on 2025/3/28.
//

#include "engine/vulkan_wrapper/VulkanBufferDescriptorBinding.h"

namespace engine {

    VulkanBufferDescriptorBinding::VulkanBufferDescriptorBinding(vk::DescriptorType descriptorType, uint32_t descriptorOffset, uint32_t descriptorRange)
            : mDescriptorType(descriptorType), mDescriptorOffset(descriptorOffset), mDescriptorRange(descriptorRange) {}

    VulkanBufferDescriptorBinding::~VulkanBufferDescriptorBinding() = default;

    const std::unique_ptr<VulkanBufferView> &VulkanBufferDescriptorBinding::getVulkanBufferView() const {
        return mVulkanBufferView;
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

    VulkanBufferDescriptorBinding &VulkanBufferDescriptorBinding::setBufferView(std::unique_ptr<VulkanBufferView> &&vulkanBufferView) {
        mVulkanBufferView = std::move(vulkanBufferView);
        return *this;
    }

//    vk::WriteDescriptorSet VulkanBufferDescriptorBinding::createWriteDescriptorSet(const vk::DescriptorSet &descriptorSet, uint32_t binding) const {
//        vk::WriteDescriptorSet writeDescriptorSet{};
//
//        vk::DescriptorBufferInfo descriptorBufferInfo{};
//        descriptorBufferInfo
//                .setBuffer(mVulkanBufferView->getVulkanBuffer()->getBuffer())
//                .setOffset(mVulkanBufferView->getOffset())
//                .setRange(mVulkanBufferView->getRange());
//
//        std::array<vk::DescriptorBufferInfo, 1> descriptorBufferInfos = {descriptorBufferInfo};
//
//        writeDescriptorSet
//                .setDstSet(descriptorSet)
//                .setDstBinding(binding)
//                .setDstArrayElement(mDescriptorOffset)
//                .setDescriptorCount(mDescriptorRange)
//                .setDescriptorType(mDescriptorType)
//                .setBufferInfo(descriptorBufferInfos);
//
//        return writeDescriptorSet;
//    }

    [[nodiscard]]
    vk::DescriptorBufferInfo VulkanBufferDescriptorBinding::createDescriptorBufferInfo() const {
        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mVulkanBufferView->getVulkanBuffer()->getBuffer())
                .setOffset(mVulkanBufferView->getOffset())
                .setRange(mVulkanBufferView->getRange());
        return descriptorBufferInfo;
    }

} // engine