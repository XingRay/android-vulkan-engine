//
// Created by leixing on 2025/4/8.
//

#include "VulkanDescriptorImageInfoConfigure.h"

namespace engine {

    VulkanDescriptorImageInfoConfigure::VulkanDescriptorImageInfoConfigure(
            std::unique_ptr<VulkanImageBuilderInterface> &&vulkanImageBuilder,
            std::unique_ptr<VulkanSamplerBuilderInterface> &&vulkanSamplerBuilder,
            std::unique_ptr<engine::ImageInterface> &&image)
            : mVulkanImageBuilder(std::move(vulkanImageBuilder)),
              mVulkanSamplerBuilder(std::move(vulkanSamplerBuilder)),
              mImage(std::move(image)) {}

    VulkanDescriptorImageInfoConfigure::~VulkanDescriptorImageInfoConfigure() = default;

} // engine