//
// Created by leixing on 2025/3/28.
//

#include "VulkanImageDescriptorBinding.h"

namespace engine {
    VulkanImageDescriptorBinding::VulkanImageDescriptorBinding() {

    }

    VulkanImageDescriptorBinding::~VulkanImageDescriptorBinding() {

    }

    vk::WriteDescriptorSet VulkanImageDescriptorBinding::createWriteDescriptorSet(const vk::DescriptorSet &descriptorSet) const {
        vk::WriteDescriptorSet writeDescriptorSet{};

        vk::DescriptorImageInfo samplerDescriptorImageInfo;
        samplerDescriptorImageInfo
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(mVulkanImageView->getImageView())
                .setSampler(mVulkanImageView->getSampler());

        std::array<vk::DescriptorImageInfo, 1> samplerDescriptorImageInfos = {samplerDescriptorImageInfo};

        writeDescriptorSet
                .setDstSet(descriptorSet)
                .setDstBinding(mBinding)
                .setDstArrayElement(mDescriptorOffset)
                .setDescriptorCount(mDescriptorRange)
                .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                .setImageInfo(samplerDescriptorImageInfos);

        return writeDescriptorSet;
    }
} // engine