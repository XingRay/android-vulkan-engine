//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanImageInterface.h"

namespace engine {

    class VulkanImageBuilderInterface {
    public:
        VulkanImageBuilderInterface();

        virtual ~VulkanImageBuilderInterface() = 0;

        virtual std::unique_ptr<VulkanImageInterface> build(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize) = 0;
    };

} // engine
