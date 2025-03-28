//
// Created by leixing on 2025/3/28.
//

#include "VulkanBufferDescriptorBinding.h"

namespace engine {
    VulkanBufferDescriptorBinding::VulkanBufferDescriptorBinding() {

    }

    VulkanBufferDescriptorBinding::~VulkanBufferDescriptorBinding() {

    }

    vk::WriteDescriptorSet VulkanBufferDescriptorBinding::createWriteDescriptorSet(const vk::DescriptorSet &descriptorSet) const {
        vk::WriteDescriptorSet writeDescriptorSet{};

        vk::DescriptorBufferInfo descriptorBufferInfo{};
        descriptorBufferInfo
                .setBuffer(mVulkanBufferView->getBuffer())
                .setOffset(mVulkanBufferView->getOffset())
                .setRange(mVulkanBufferView->getRange());

        std::array<vk::DescriptorBufferInfo, 1> descriptorBufferInfos = {descriptorBufferInfo};

        writeDescriptorSet
                .setDstSet(descriptorSet)
                .setDstBinding(mBinding)
                .setDstArrayElement(mDescriptorOffset)
                .setDescriptorCount(mDescriptorRange)
                .setDescriptorType(mDescriptorType)
                .setBufferInfo(descriptorBufferInfos);

        return writeDescriptorSet;
    }
} // engine