//
// Created by leixing on 2025/4/9.
//

#include "VulkanDescriptorImageInfo.h"

namespace engine {

    VulkanDescriptorImageInfo::VulkanDescriptorImageInfo(std::unique_ptr<VulkanImageInterface> &&vulkanImage,
                                                         std::unique_ptr<VulkanSamplerInterface> &&vulkanSampler)
            : mVulkanImage(std::move(vulkanImage)),
              mVulkanSampler(std::move(vulkanSampler)) {}

    VulkanDescriptorImageInfo::~VulkanDescriptorImageInfo() = default;

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