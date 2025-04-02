//
// Created by leixing on 2025/3/29.
//

#include "engine/vulkan_wrapper/VulkanImageView.h"

namespace engine {

    VulkanImageView::VulkanImageView() = default;

    VulkanImageView::~VulkanImageView() = default;

    const std::shared_ptr<VulkanImageInterface> &VulkanImageView::getVulkanImage() const {
        return mVulkanImage;
    }

    const std::shared_ptr<VulkanSamplerInterface> &VulkanImageView::getVulkanSampler() const {
        return mVulkanSampler;
    }

} // engine