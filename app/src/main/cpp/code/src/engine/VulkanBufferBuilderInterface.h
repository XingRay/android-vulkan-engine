//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "engine/vulkan_wrapper/VulkanDevice.h"
#include "engine/vulkan_wrapper/VulkanBufferInterface.h"

namespace engine {

    class VulkanBufferBuilderInterface {
    public:
        VulkanBufferBuilderInterface();

        virtual ~VulkanBufferBuilderInterface() = 0;

        virtual std::unique_ptr<VulkanBufferInterface> build(const VulkanDevice &vulkanDevice, vk::DeviceSize bufferSize) = 0;
    };

} // engine
