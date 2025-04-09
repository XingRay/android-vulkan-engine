//
// Created by leixing on 2025/4/9.
//

#include "engine/vulkan_wrapper/VulkanDescriptorImageInfo.h"

namespace engine {

    VulkanDescriptorImageInfo::VulkanDescriptorImageInfo() {

    }

    VulkanDescriptorImageInfo::~VulkanDescriptorImageInfo() {

    }

    [[nodiscard]]
    vk::DescriptorImageInfo VulkanDescriptorImageInfo::createDescriptorImageInfo() const {
        vk::DescriptorImageInfo descriptorImageInfo{};
        descriptorImageInfo
                .setSampler(mVulkanSampler->getSampler())
                .setImageView(mVulkanImage->getImageView())
                        // todo: set by field
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

        return descriptorImageInfo;
    }

} // engine