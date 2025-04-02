//
// Created by leixing on 2025/3/28.
//

#include "VulkanImageDescriptorBinding.h"

namespace engine {

    VulkanImageDescriptorBinding::VulkanImageDescriptorBinding(std::unique_ptr<VulkanImageView> &&vulkanImageView,
                                                               vk::DescriptorType descriptorType,
                                                               uint32_t descriptorOffset,
                                                               uint32_t descriptorRange)
            : mVulkanImageView(std::move(vulkanImageView)),
              mDescriptorType(descriptorType),
              mDescriptorOffset(descriptorOffset),
              mDescriptorRange(descriptorRange) {

    }

    VulkanImageDescriptorBinding::~VulkanImageDescriptorBinding() = default;

    const std::unique_ptr<VulkanImageView> &VulkanImageDescriptorBinding::getVulkanImageView() const {
        return mVulkanImageView;
    }

    VulkanImageDescriptorBinding &VulkanImageDescriptorBinding::setImageView(std::unique_ptr<VulkanImageView> &&vulkanImageView) {
        mVulkanImageView = std::move(vulkanImageView);
        return *this;
    }

    vk::WriteDescriptorSet VulkanImageDescriptorBinding::createWriteDescriptorSet(const vk::DescriptorSet &descriptorSet, uint32_t binding) const {
        vk::WriteDescriptorSet writeDescriptorSet{};

        vk::DescriptorImageInfo samplerDescriptorImageInfo;
        samplerDescriptorImageInfo
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(mVulkanImageView->getVulkanImage()->getImageView())
                .setSampler(mVulkanImageView->getVulkanSampler()->getSampler());

        std::array<vk::DescriptorImageInfo, 1> samplerDescriptorImageInfos = {samplerDescriptorImageInfo};

        writeDescriptorSet
                .setDstSet(descriptorSet)
                .setDstBinding(binding)
                .setDstArrayElement(mDescriptorOffset)
                .setDescriptorCount(mDescriptorRange)
                .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                .setImageInfo(samplerDescriptorImageInfos);

        return writeDescriptorSet;
    }
} // engine