//
// Created by leixing on 2025/4/9.
//

#include "ImageInfo.h"

namespace vklite {

    ImageInfo::ImageInfo(std::unique_ptr<VulkanImageInterface> &&vulkanImage,
                                                         std::unique_ptr<SamplerInterface> &&vulkanSampler)
            : mVulkanImage(std::move(vulkanImage)),
              mVulkanSampler(std::move(vulkanSampler)) {}

    ImageInfo::~ImageInfo() = default;

    const std::unique_ptr<VulkanImageInterface> &ImageInfo::getVulkanImage() const {
        return mVulkanImage;
    }

    const std::unique_ptr<SamplerInterface> &ImageInfo::getVulkanSampler() const {
        return mVulkanSampler;
    }

    [[nodiscard]]
    vk::DescriptorImageInfo ImageInfo::createDescriptorImageInfo() const {
        vk::DescriptorImageInfo descriptorImageInfo{};
        descriptorImageInfo
                .setSampler(mVulkanSampler->getSampler())
                .setImageView(mVulkanImage->getImageView())
                        // todo: set by field
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

        return descriptorImageInfo;
    }

} // engine