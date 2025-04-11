//
// Created by leixing on 2025/4/11.
//

#include "engine/vulkan_wrapper/image/VulkanImageBuilder.h"
#include "engine/vulkan_wrapper/image/VulkanImage.h"

namespace engine {
    VulkanImageBuilder::VulkanImageBuilder() {

    }

    VulkanImageBuilder::~VulkanImageBuilder() {

    }

    std::unique_ptr<VulkanImageInterface> VulkanImageBuilder::build(const VulkanDevice &vulkanDevice, uint32_t width, uint32_t height, vk::Format format) {
        return std::make_unique<VulkanImage>(vulkanDevice, width, height, format);
    }
} // engine