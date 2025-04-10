//
// Created by leixing on 2025/4/11.
//

#include "engine/vulkan_wrapper/image/VulkanSamplerBuilder.h"
#include "engine/vulkan_wrapper/image/VulkanSampler.h"

namespace engine {

    VulkanSamplerBuilder::VulkanSamplerBuilder() = default;

    VulkanSamplerBuilder::~VulkanSamplerBuilder() = default;

    std::unique_ptr<VulkanSamplerInterface> VulkanSamplerBuilder::build(const VulkanDevice &vulkanDevice, const VulkanImageInterface &image) {
        return std::make_unique<VulkanSampler>(vulkanDevice, image.getMipLevels());
    }

} // engine