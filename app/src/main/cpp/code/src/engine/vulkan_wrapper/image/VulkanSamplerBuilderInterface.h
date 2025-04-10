//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/image/VulkanSamplerInterface.h"
#include "engine/vulkan_wrapper/image/VulkanImageInterface.h"

namespace engine {

    class VulkanSamplerBuilderInterface {
    public:
        VulkanSamplerBuilderInterface();

        virtual ~VulkanSamplerBuilderInterface() = 0;

        virtual std::unique_ptr<VulkanSamplerInterface> build(const VulkanDevice &vulkanDevice, const VulkanImageInterface &image) = 0;
    };

} // engine
