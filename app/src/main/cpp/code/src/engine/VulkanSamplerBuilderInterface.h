//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanSamplerInterface.h"

namespace engine {

    class VulkanSamplerBuilderInterface {
    public:
        VulkanSamplerBuilderInterface();

        virtual ~VulkanSamplerBuilderInterface() = 0;

        virtual std::unique_ptr<VulkanSamplerInterface> build(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize) = 0;
    };

} // engine
