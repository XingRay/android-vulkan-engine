//
// Created by leixing on 2025/4/4.
//

#include "VulkanUniformBufferBuilder.h"

namespace vklite {

    VulkanUniformBufferBuilder::VulkanUniformBufferBuilder() = default;

    VulkanUniformBufferBuilder::~VulkanUniformBufferBuilder() = default;

    std::unique_ptr<VulkanBufferInterface> VulkanUniformBufferBuilder::build(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize) {
        VulkanDeviceLocalUniformBuffer buffer(vulkanDevice, bufferSize);
        return std::make_unique<VulkanDeviceLocalUniformBuffer>(vulkanDevice, bufferSize);
    }

} // engine