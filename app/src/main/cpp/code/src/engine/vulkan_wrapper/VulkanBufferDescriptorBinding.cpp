//
// Created by leixing on 2025/3/28.
//

#include "VulkanBufferDescriptorBinding.h"

namespace engine {

    VulkanBufferDescriptorBinding::VulkanBufferDescriptorBinding() = default;

    VulkanBufferDescriptorBinding::~VulkanBufferDescriptorBinding() = default;

    const std::unique_ptr<VulkanBufferView> &VulkanBufferDescriptorBinding::getVulkanBufferView() const {
        return mVulkanBufferView;
    }

    VulkanBufferDescriptorBinding &VulkanBufferDescriptorBinding::setBufferView(std::unique_ptr<VulkanBufferView> &&vulkanBufferView) {
        mVulkanBufferView = std::move(vulkanBufferView);
        return *this;
    }

    vk::WriteDescriptorSet VulkanBufferDescriptorBinding::createWriteDescriptorSet(const vk::DescriptorSet &descriptorSet, uint32_t binding) const {
        vk::WriteDescriptorSet writeDescriptorSet{};

        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mVulkanBufferView->getBuffer())
                .setOffset(mVulkanBufferView->getOffset())
                .setRange(mVulkanBufferView->getRange());

        std::array<vk::DescriptorBufferInfo, 1> descriptorBufferInfos = {descriptorBufferInfo};

        writeDescriptorSet
                .setDstSet(descriptorSet)
                .setDstBinding(binding)
                .setDstArrayElement(mDescriptorOffset)
                .setDescriptorCount(mDescriptorRange)
                .setDescriptorType(mDescriptorType)
                .setBufferInfo(descriptorBufferInfos);

        return writeDescriptorSet;
    }

} // engine