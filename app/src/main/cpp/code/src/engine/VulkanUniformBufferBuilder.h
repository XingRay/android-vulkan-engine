//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/buffer/device_local/VulkanDeviceLocalUniformBuffer.h"
#include "engine/VulkanBufferBuilder.h"

namespace engine {

    class VulkanUniformBufferBuilder : public VulkanBufferBuilder {
    public:

        VulkanUniformBufferBuilder();

        ~VulkanUniformBufferBuilder() override;

        std::unique_ptr<VulkanBufferInterface> build(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize) override;
    };

} // engine
